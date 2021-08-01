<!--
SPDX-FileCopyrightText: 2021 Diego Elio Pettenò

SPDX-License-Identifier: 0BSD
-->

# Bill Of Material for esphome-lg-pqrcuds0-rev1

Note: approximate cost applied to based on Digikey on 2021-08-01.

Prices are read in GBP before VAT, and for a single item.

 | Quantity | Code              | Cost Per Unit | Total Cost |
 | -------- | ----------------- | ------------- | ---------- |
 | 2        | S5B-ZR(LF)(SN)    | £0.21         | £0.42      |
 | 2        | 1N4148            | £0.07         | £0.14      |
 | 1        | OS202013MT8QN1    | £0.60         | £0.60      |
 | 2        | TLIN1027DRQ1      | £0.76         | £1.52      |
 | 1        | 74HC00N           | £0.35         | £0.35      |
 | 4        | BZT03C27-TAP      | £0.47         | £1.88      |
 | 2        | FG28X5R1H105KRT06 | £0.27         | £0.54      |
 | 2        | K221K10X7RF5UH5   | £0.14         | £0.28      |
 | 2        | 4.7kΩ resistor    | £0.0034       | £0.0064    |
 | 1        | 1kΩ resistor      | £0.00345      | £0.00345   |
 | 1        | LM2574-3.3YN      | £1.22         | £1.22      |
 | 1        | SB150-E3          | £0.29         | £0.29      |
 | 1        | FG22X7R1E226MRT06 | £1.17         | £1.17      |
 | 1        | 860020474012      | £0.16         | £0.16      |
 | 1        | AIAP-02-331K      | £0.49         | £0.49      |
 | 2        | PPTC191LFBN-RC    | £0.84         | £1.68      |
 | 1        | ESP32-DEVKITC-32D | £7.16         | £7.16      |

Total: £18.84

## Cost Saving Ideas

FG22X7R1E226MRT06 could be replaced with a polymer capacitor such as
A759EK226M1JAAE054 for £0.39.

BZT03C27-TAP pairs could be replaced with an integrated TVS diode like the
MMBZ27VALT3G (though surface mount) for £0.16 each.

The ESP32-DEVKIT-32D could be replaced with a WROOM module and the correct
passives, which would probably bring down the main cost by £5 or so between
the module itself, and the sockets.
