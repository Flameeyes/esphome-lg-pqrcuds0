<!--
SPDX-FileCopyrightText: 2021 Diego Elio Pettenò

SPDX-License-Identifier: 0BSD
-->

# LG PQRCUDS0 compatible ESPHome Component

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
