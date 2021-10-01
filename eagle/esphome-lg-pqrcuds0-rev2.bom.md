<!--
SPDX-FileCopyrightText: 2021 Diego Elio Pettenò

SPDX-License-Identifier: 0BSD
-->

# Bill Of Material for esphome-lg-pqrcuds0-rev1

Note: approximate cost applied to based on Digikey on 2021-08-01.

Prices are read in GBP before VAT, and for a single item.

| Quantity | Code               | Cost Per Unit | Total Cost |
| -------- | ------------------ | ------------- | ---------- |
| 1        | S5B-ZR(LF)(SN)     | £0.21         | £0.21      |
| 2        | 1N4148             | £0.07         | £0.14      |
| 1        | TL2230EEF140       | £0.64         | £0.64      |
| 1        | TLIN1027DRQ1       | £0.76         | £0.76      |
| 2        | BZT03C27-TAP       | £0.47         | £0.94      |
| 1        | FG28X5R1H105KRT06  | £0.27         | £0.27      |
| 1        | K221K10X7RF5UH5    | £0.14         | £0.14      |
| 1        | 4.7kΩ resistor     | £0.0034       | £0.0034    |
| 1        | 1kΩ resistor       | £0.00345      | £0.00345   |
| 1        | LM2574-3.3YN       | £1.22         | £1.22      |
| 1        | SB150-E3           | £0.29         | £0.29      |
| 1        | A759EK226M1JAAE054 | £0.39         | £0.39      |
| 1        | 860020474012       | £0.16         | £0.16      |
| 1        | AIAP-02-331K       | £0.49         | £0.49      |
| 2        | PPTC191LFBN-RC     | £0.84         | £1.68      |
| 1        | ESP32-DEVKITC-32D  | £7.16         | £7.16      |

Total: £14.50

## Cost Saving Ideas

The BZT03C27-TAP pair could be replaced with an integrated TVS diode like the
MMBZ27VALT3G (though surface mount) for £0.16.

The ESP32-DEVKIT-32D could be replaced with a WROOM module and the correct
passives, which would probably bring down the main cost by £5 or so between
the module itself, and the sockets.

The LM2574-3.3YN could be replaced with a linear regulator, given that the
panel no longer needs to be supplied in parallel.
