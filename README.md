# atomix

The purpose of `atomix` is to allow a user to query the atomic data in the
Monte Carlo Radiative Transfer and Ionization code 
[Python](https://github.com/agnwinds/python/).

It was made originally to try and decipher some of the mysterious spectral lines
and edges in the synthetic spectra created by Python for accretion disc winds in
Tidal Disruption Events.

## Features

* Change the atomic data files on the fly
* Inspect the possible bound-bound transitions over a provided wavelength range
* Inspect the possible photoionization edges over a provided wavelength range

## Requirements, Building and Usage

`atomix` uses the [ncurses](https://en.wikipedia.org/wiki/Ncurses) library to
create a basic UI which is navigated using the keyboard.

As of current `atomix` will only work on Linux, this is because it's beyond my
level of sorcery and effort level to currently write a CMake configuration file
for it to compile of macOS, see this [StackOverflow post](https://stackoverflow.com/questions/56622042/clang-on-macos-fails-linking-lmenu-from-ncurses/56623033?noredirect=1#comment99833073_56623033) for an idea why.

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

Here are the current plans for future development.

* Improved and consistent UI design*
* Scrollable lists
* Other atomic features to search, i.e. levels, configurations
* masOS support*

*If we are lucky ;-)
