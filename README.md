# Botoks

Botoks is a batteryless timekeeping sensor that operates intermittently
harvesting ambient energy.  Botoks's hardware and software components enable IoT
applications that require precise and intermittency-safe timekeeping.  Both
hardware design and software stack are open-source.

The *cascaded hierarchical remanence timekeeper* (CHRT) embedded on Botoks is a
novel remanence timekeeper architecture.  In principle, capacitive remanence
timekeepers are simple RC circuits whose energy level is converted into time.
To know more about them and the CHRT, refer to our paper (to be made public) and
[the docs][docs].

The sensor is based on an MSP430 ultra-low-power, FRAM-enabled microcontroller
(MSP430FR5994), and currently the software stack only supports this MCU.

<!--=========================================================================-->

## Project Layout

```
apps/               ## Applications' source code
bin/                ## Compiled applications
config/             ## Various configuration files
docs/               ## Source documentation
external/           ## MSP430 base drivers
libs/               ## Core Botoks's libraries
platforms/          ## GPIO pin mappings
scripts/            ## Botoks's calibration scripts
```

<!--=========================================================================-->

## Usage

Botoks's code can be built installing the MSP430-GCC toolchain, or using a
preconfigured Docker container.  Opting for the Docker container has the
advantage of not having to install the toolchain, but requires you to mount the
directory of the project into the container.

Executables can be uploaded and debugged using UniFlash or Code Composer Studio
(CCS).  The former method is easier to script, whilst the latter gives you
access to a graphical IDE.

The following versions of CMake, MSP430-GCC and UniFlash were tested, but other
versions might work as well.

<!-- For artifact: -->
<!-- CMake 3.15.5 -->
<!-- MSP430-GCC 6.1.1.0 -->
<!-- MSP430-GCC Support Files 1.208 -->
<!-- MSPDebug 0.25 -->

| Software   | Version |                      |
|:-----------|:--------|:---------------------|
| CMake      | 3.13    | [download][cmake]    |
| MSP430-GCC | 8.3.0   | [download][mspgcc]   |
| UniFlash   | 5.2.0   | [download][uniflash] |

### Building with the Docker container

You can use the `build_with_docker.sh` script to build all applications inside a
Docker container.  Install `docker` on your machine and start/enable the
`docker` daemon.  Moreover, make sure that you can `docker` commands **without**
root privileges (check [here][docker-no-root]).  The script uses [this Docker
image][docker-image] to build applications inside a Docker container
pre-configured with CMake and the MSP430-GCC toolchain.

You can pass `-t <target>` to the script to specify a target for `make`.  For
instance, to build all projects and install the generated executables in the
`bin/` folder of this repository, run

```bash
$ ./build_with_docker.sh -t install
```

### Building without the Docker container

First of all, install CMake and the MSP430-GCC toolchain.  CMake can be
installed using your OS's package manager, though the latest version might not
be available.  In that case, download the binaries using the link above.  As for
the GCC toolchain, you can use the provided `install_toolchain.sh` script to
download and install toolchain and support files.

To install, do

```bash
$ MSP430_GCC_OS=linux64   # can be 'linux32' or 'macos' instead
$ INSTALL_PREFIX=~/ti     # where to install the toolchain
$ ./install_toolchain.sh
```

This will download toolchain and other support files from TI's website, and
install them at `$INSTALL_PREFIX/msp430-gcc`.  Then, assign the environment
variable `MSP430_TOOLCHAIN_PATH` the absolute path to the root directory of the
toolchain, e.g.

```bash
$ export MSP430_TOOLCHAIN_PATH=~/ti/msp430-gcc
```

Finally, to build all projects, do

```bash
$ git clone https://github.com/TUDSSL/Botoks.git
$ cd Botoks
$ mkdir build && cd build
$ cmake ..
$ make
```

Then run `make install` to build all projects and install the generated
executables in the `bin/` folder of this repository.  To build applications
individually, run `make <app-name>`.

### Running applications

First, connect Botoks to a debugger capable of debugging the MSP430 line of
products.  Then power Botox using the auxiliary power connector on the PCB.  The
voltage provided needs to be between 3.4 and 5V.

To upload an application install [UniFlash][uniflash].  As of now, all scripts
assume UniFlash is installed at `/opt/ti/uniflash`.  From the project's
directory root, run:

```bash
$ ./flash.sh bin/<app-name>.out
```

The serial output can now be monitored using your favorite serial monitor (e.g.,
`picocom`) with a baudrate of 19200.

```bash
$ picocom /dev/ttyACM1 -b 19200 --imap lfcrlf
```

### Calibration

For the calibration procedure please refer to [the docs][docs].  Running the
calibration procedure requires a licensed version of [MATLAB][matlab] and the
serial terminal `picocom`.

<!--=========================================================================-->

## Documentation

The Docker container comes with all the dependencies to build the documentation.
Simply run

```bash
$ ./build_with_docker.sh -t docs
```

and find the HTML documentation in `build/docs/html`.

<!--=========================================================================-->

## Hardware

The hardware is designed in [KiCad][kicad], however, PDF schematics and gerbers
are available in the [hardware][hardware] folder.

<!--=========================================================================-->

[paper]: dl.link
[docs]: #documentation
[cmake]: https://cmake.org/download/
[mspgcc]: http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/latest/index_FDS.html
[uniflash]: http://www.ti.com/tool/UNIFLASH
[docker-no-root]: https://docs.docker.com/install/linux/linux-postinstall/#manage-docker-as-a-non-root-user
[docker-image]: https://hub.docker.com/r/cdelledonne/msp430-gcc
[matlab]: https://www.mathworks.com/products/matlab.html
[kicad]: http://www.kicad-pcb.org/
[hardware]: ./hardware
