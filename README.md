<!--
SPDX-FileCopyrightText: 2021 Diego Elio Pettenò

SPDX-License-Identifier: 0BSD
-->

# LG PQRCUDS0 compatible ESPHome Component

![The custom board on the wall](images/board-on-wall.jpg)

This repository contains the source code and the EAGLE design files for using
[ESPHome](https://esphome.io/) to control a LG air conditioning unit that uses
the PQRCUDS0 control panel.

The source code and an example configuration package are in the `esphome/`
directory, while the `eagle/` directory has the PCB design files.

Note that this project is neither endorsed, nor sponsored, by either of LG
or the ESPHome project.

You can find details of the implementation and some of the choices made in the
design on my blog: https://flameeyes.blog/tag/lg/

See also:

* Python utilities to emulate the panel and engine at
    <https://github.com/Flameeyes/lg-PQRCUDS0-emu>
* [Saleae Logic](https://www.saleae.com/) high-level analyzer for the
  protocol, at <https://github.com/Flameeyes/saleae-extension-LG-PQRCUDS0>

## Board Design Notes

The boards you will find in this repository have been designed by me under
assumptions tha have been found lacking some nuances, and as such may not
make entire sense.

### Panel Bus

The second LINbus transceiver is meant to be connected to the original panel
(PQRCUDS0), keeping it from crashing. This was originally intended as a way to
quickly override the settings without using Home Assistant, while maintaining
the additional temperature sensing capability offered by the panel itself.

While this is possible, I have not implemented support for it in the custom
component at all, nor tested this in the actual field. The temperature sensor
on the panel tends to be of very little use, due to its limited range.

### ESP32 Cutoff

The 2P2T switch at the top of the board is meant to be used to cut off the
custom board logic from the LINbus, while maintaining the ESPHome code running
to be able to retrieve the temperature sensing of the panel and HVAC engine.

This turned out to be less than useful, as the panel has trouble synchronizing
with the state of the engine when detached from one to the other, causing it
to remain in an undefined state for a significant time following it.

A more useful implementation would ensure that the panel would stay powered
off while the ESP32 has control over the bus, but that also requires powering
the whole board off when switching.

## License

Both the custom component and the EAGLE design files are released under the
permissive 0BSD license. This means you can reuse them pretty much as you may
want to.

Do note that ESPHome libraries are licensed under GNU GPLv3, so combining them
with logic in the components is still subject to the full license.
