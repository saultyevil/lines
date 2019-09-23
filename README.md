The purpose of this program is to query the line transitions between two provided wavelength boundaries.

 Usage:  lines [-h] data wmin wmax

  [-h]: print this message and exit
  data: the path to the Python data master file to use
  wmin: the lower wavelength boundary
  wmax: the upper wavelength boundary

## Building

Use CMake to build this program.

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
