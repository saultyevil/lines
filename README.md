# atomix

The purpose of `atomix` is to allow a user to query the atomic data used in the
Monte Carlo Radiative Transfer and Ionization code 
[Python](https://github.com/agnwinds/python/).

It was made originally to try and decipher some of the mysterious spectral lines
and edges in the synthetic spectra created by Python for accretion disc winds in
Tidal Disruption Events.

## Features

* Change the atomic data files on the fly
* Inspect the bound-bound transitions over a provided wavelength range
* Inspect the photoionization edges over a provided wavelength range

## Requirements, Building and Usage

`atomix` uses the [ncurses](https://en.wikipedia.org/wiki/Ncurses) library to
create a basic UI which is navigated using the keyboard.

As of current `atomix` will, or should, work on both linux and macOS. For Linux,
please use the most up-to-date development version of `libncurses5-dev` and
`libncursesw5-dev`. On macOS, install the `ncurses` package in Homebrew (or your
other favourite package manager).

But, if you are on Linux, you can use CMake to build this program, usually as
follows:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

To use `atomix`, one simply has to invoke atomix from the command line.

## TODO

Here are some of the current plans for future development:

* Improved and consistent UI design*
* Scrollable lists
* Other atomic features to search, i.e. levels, configurations

*If we are lucky ;-)
