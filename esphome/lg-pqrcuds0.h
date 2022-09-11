// SPDX-FileCopyrightText: 2021 Diego Elio Pettenò
//
// SPDX-License-Identifier: 0BSD

#include "esphome.h"

class LgHvac : public climate::Climate, public PollingComponent
{
public:
    LgHvac(esphome::uart::UARTComponent *wall_bus, esphome::sensor::Sensor *reference_temperature) : PollingComponent(4000)
    {
        this->wall_bus_ = wall_bus;
        this->reference_temperature_ = reference_temperature;
    }

    void setup() override
    {
        auto restore = this->restore_state_();
        if (restore.has_value())
        {
            restore->apply(this);
        }
        else
        {
            this->mode = climate::CLIMATE_MODE_FAN_ONLY;
            this->target_temperature_high = 24;
            this->target_temperature_low = 18;
            this->fan_mode = climate::CLIMATE_FAN_MEDIUM;
            this->publish_state();
        }

        uint8_t input_buffer[12];
        this->wall_bus_->read_array(input_buffer, 12);
    }

    float
    get_setup_priority() const override
    {
        return esphome::setup_priority::BUS;
    }

    void control(const climate::ClimateCall &call) override
    {
        if (call.get_mode().has_value())
            this->mode = *call.get_mode();
        if (call.get_target_temperature_high().has_value())
            this->target_temperature_high = *call.get_target_temperature_high();
        if (call.get_target_temperature_low().has_value())
            this->target_temperature_low = *call.get_target_temperature_low();
        if (call.get_fan_mode().has_value())
            this->fan_mode = *call.get_fan_mode();
        this->publish_state();
    }

    climate::ClimateTraits traits() override
    {
        auto traits = climate::ClimateTraits();
        traits.set_supported_modes({
            climate::CLIMATE_MODE_OFF,
            climate::CLIMATE_MODE_HEAT_COOL,
            climate::CLIMATE_MODE_COOL,
            climate::CLIMATE_MODE_HEAT,
            climate::CLIMATE_MODE_DRY,
            climate::CLIMATE_MODE_FAN_ONLY,
        });
        traits.set_supported_fan_modes({
            climate::CLIMATE_FAN_OFF,
            climate::CLIMATE_FAN_LOW,
            climate::CLIMATE_FAN_MEDIUM,
            climate::CLIMATE_FAN_HIGH,
        });
        traits.set_supports_two_point_target_temperature(true);
        traits.set_supports_current_temperature(true);
        traits.set_supports_action(true);
        return traits;
    }

    void update() override
    {
        static const uint8_t MODE_COOL = 0x00;
        static const uint8_t MODE_DH = 0x10;
        static const uint8_t MODE_FAN = 0x20;
        static const uint8_t MODE_HEAT = 0x40;

        static const uint8_t RUNNING = 0x04;

        uint8_t buffer[6] = {
            0,
        };

        uint8_t input_buffer[12] = {
            0,
        };

        this->current_temperature = this->reference_temperature_->get_state();

        // This is true iff we want the fan mode to run in "idle" mode.
        bool idle_fan_on = *this->fan_mode != climate::CLIMATE_FAN_OFF;
        float hysteresis = this->action == climate::CLIMATE_ACTION_COOLING ? -this->hysteresis_ : this->hysteresis_;

        switch (this->mode)
        {
        case climate::CLIMATE_MODE_OFF:
            this->action = climate::CLIMATE_ACTION_OFF;
            break;
        case climate::CLIMATE_MODE_COOL:
            if (this->current_temperature >= (this->target_temperature_high + hysteresis))
                this->action = climate::CLIMATE_ACTION_COOLING;
            else
                this->action = idle_fan_on ? climate::CLIMATE_ACTION_FAN : climate::CLIMATE_ACTION_IDLE;
            break;
        case climate::CLIMATE_MODE_HEAT:
            if (this->current_temperature <= (this->target_temperature_low + hysteresis))
                this->action = climate::CLIMATE_ACTION_HEATING;
            else
                this->action = idle_fan_on ? climate::CLIMATE_ACTION_FAN : climate::CLIMATE_ACTION_IDLE;
            break;
        case climate::CLIMATE_MODE_HEAT_COOL:
            if (this->current_temperature <= (this->target_temperature_low + hysteresis))
                this->action = climate::CLIMATE_ACTION_HEATING;
            else if (this->current_temperature >= (this->target_temperature_high + hysteresis))
                this->action = climate::CLIMATE_ACTION_COOLING;
            else
                this->action = idle_fan_on ? climate::CLIMATE_ACTION_FAN : climate::CLIMATE_ACTION_IDLE;
            break;
        case climate::CLIMATE_MODE_DRY:
            this->action = climate::CLIMATE_ACTION_DRYING;
            break;
        case climate::CLIMATE_MODE_FAN_ONLY:
            this->action = *this->fan_mode != climate::CLIMATE_FAN_OFF ? climate::CLIMATE_ACTION_FAN : climate::CLIMATE_ACTION_OFF;
            break;

        default:
            ESP_LOGE("lg_pqrcuds0", "Unsupported climate mode value %d. Turning off.", this->mode);
            this->action = climate::CLIMATE_ACTION_OFF;
        }

        // Provide a default "desired temperature" to give to the engine. We correct it when cooling/heating.
        int desired_temperature = int((this->target_temperature_high + this->target_temperature_low) / 2);

        switch (this->action)
        {
        case climate::CLIMATE_ACTION_OFF:
            buffer[0] &= ~RUNNING;
            break;
        case climate::CLIMATE_ACTION_COOLING:
            buffer[0] |= MODE_COOL | RUNNING;
            desired_temperature = int(this->target_temperature_low);
            break;
        case climate::CLIMATE_ACTION_HEATING:
            buffer[0] |= MODE_HEAT | RUNNING;
            desired_temperature = int(this->target_temperature_high);
            break;
        case climate::CLIMATE_ACTION_IDLE:
            buffer[0] |= this->mode == climate::CLIMATE_MODE_HEAT ? MODE_HEAT : MODE_COOL;
            buffer[0] &= ~RUNNING;
            break;
        case climate::CLIMATE_ACTION_DRYING:
            buffer[0] |= MODE_DH | RUNNING;
            break;
        case climate::CLIMATE_ACTION_FAN:
            buffer[0] |= MODE_FAN | RUNNING;
            break;
        }

        switch (*this->fan_mode)
        {
        case climate::CLIMATE_FAN_OFF:
        case climate::CLIMATE_FAN_LOW:
            buffer[2] |= 0x00;
            break;
        case climate::CLIMATE_FAN_MEDIUM:
            buffer[2] |= 0x10;
            break;
        case climate::CLIMATE_FAN_HIGH:
            buffer[2] |= 0x20;
            break;
        default:
            ESP_LOGE("lg_pqrcuds0", "Unsupported fan mode value %d. Turning off.", *this->fan_mode);
            buffer[2] |= 0x00;
        }

        buffer[1] = (clamp(int(this->current_temperature), 10, 30) - 10) * 2;

        buffer[2] |= (clamp(int(desired_temperature), 0, 30) - 16) & 0x0F;

        if (memcmp(buffer, this->last_buffer_, 5) != 0)
        {
            memcpy(this->last_buffer_, buffer, 5);
            buffer[0] |= 0x01;
        }

        buffer[5] = ((buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4]) & 0xFF) ^ 0x55;

        ESP_LOGD("lg_pqrcuds0", "Sending: %s", format_hex_pretty(buffer, 6).c_str());

        this->wall_bus_->write_array(buffer, 6);
        if (this->wall_bus_->read_array(input_buffer, 12))
        {
            ESP_LOGD("lg_pqrcuds0", "Received: %s", format_hex_pretty(input_buffer, 12).c_str());
        }

        this->publish_state();
    }

protected:
    esphome::uart::UARTComponent *wall_bus_{nullptr};
    esphome::sensor::Sensor *reference_temperature_{nullptr};
    uint8_t last_buffer_[5] = {
        0,
    };

    float hysteresis_ = 0.5;
};
