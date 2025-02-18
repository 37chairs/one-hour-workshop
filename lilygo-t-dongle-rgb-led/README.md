# Lilygo T Dongle RGB LED Control

This is a simple example of how to control the RGB LED on the Lilygo T Dongle 
using the ESP-IDF framework.

## Install esp-idf framework

Don't breeze past this.  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html

```bash
mkdir -p ~/esp
cd ~/esp
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git
./install.sh esp32,esp32s3,esp32c6,esp32c3
```

## Manually install esp-idf-lib component

```bash
% cd components
% git clone --recursive https://github.com/UncleRus/esp-idf-lib.git
% cd ../
% idf.py reconfigure
```

## Building

```bash
% idf.py set-target esp32s3
% idf.py reconfigure
% idf.py build
% idf.py flash
```

Note: If initial build fails, check that esp-idf-lib cloned fully, then set-target, reconfigure, and build. 

## Loading ESP-IDF env later on


```bash
% source ~/esp/esp-idf/export.sh

Checking "python3" ...
Python 3.12.3
"python3" has been detected
Activating ESP-IDF 5.4
* Checking python version ... 3.12.3
* Checking python dependencies ... OK
* Deactivating the current ESP-IDF environment (if any) ... OK
* Establishing a new ESP-IDF environment ... OK
* Identifying shell ... bash
* Detecting outdated tools in system ... OK - no outdated tools found
* Shell completion ... Autocompletion code generated

Done! You can now compile ESP-IDF projects.
Go to the project directory and run:

  idf.py build
```

## Rough Changelog

2025-02-16 - Simplified some code, updating README.md with more detail
2025-02-10 - Project builds and flashes to Lilygo T Dongle S3, LED cycles RGB
