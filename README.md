# CVBindings

A handful of C bindings for OpenCV 3.0

## Installation of CVBindings binary on the Raspberry Pi

```
$ wget https://github.com/MeasureTheFuture/CVBindings/releases/download/3.4.1/cvbindings_3.4.1_armhf.deb
$ sudo dpkg -i cvbindings_3.4.1_armhf.deb
```

## Compilation of CVBindings

Install dependencies:

```
$ sudo apt-get update && sudo apt-get upgrade
$ sudo apt-get install build-essential cmake pkg-config
$ sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
$ sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
$ sudo apt-get install libxvidcore-dev libx264-dev
$ sudo apt-get install libgtk2.0-dev libgtk-3-dev
$ sudo apt-get install libatlas-base-dev gfortran
$ wget https://github.com/MeasureTheFuture/Pi-OpenCV/releases/download/3.4.1/opencv_3.4.1_armhf.deb
$ sudo dpkg -i opencv_3.4.1_armhf.deb
```

Compile:
```
$ cmake .
$ make
```

## Create debian package

```
apt-get install dh-make
dpkg-buildpackage -uc -tc -rfakeroot
```

## License

Copyright (C) 2015, Clinton Freeman

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