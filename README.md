# cp2110-c

Copyright (c) 2015 - Gray Cat Labs - https://graycat.io

A basic userspace driver for the CP2110 HID USB-UART IC.

## Dependencies

Requires [hidapi](https://github.com/signal11/hidapi). You can install it on Ubuntu with:

    $ sudo apt-get install libusb-1.0-0-dev libhidapi-dev

On Linux you will also need to either run programs talking to the CP2110 as root, or add the included udev rule:

    $ cd cp2110-c/udev/
    $ sudo cp 50-cp2110.rules /etc/udev/rules.d/

## Todo

* Rename...
* Implement the one time programmable stuff?
* Blocking read?

## License

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
