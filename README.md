# OSG

The osG library aims to simplify the firmware programming,
by absrtacting the low-level API into a standard interface.
The advantages are:
 * Code reusability: since the API are platform-independent,
   the application can be easily ported between different boards
 * Programming easyness: programmers can focus on application logic instead
   of board-bsp specific issues and API
 * Increased productivity: low-level details are encapsulateed into the API,
   avoiding to programmers to waste time to understand how the specific board
   works

# Compiling

For compiling:
* Keil: there are few ready-to-work examples. Please refer to them to use such an IDE
* CMake: coming soon

# License

Copyright 2018-2019 GIMASI
* http://www.gimasi.ch
* info@gimasi.ch

osG has a dual license scheme:
 * **GPL v3**: the library is freely available under GPL v3 (or newer) license.
   A copy of the GPL v3 license is available [here](doc/GPL.txt)
 * **Commercial**: the library is available also with a commercial license,
   which frees users from GPL restrictions. Please contact [GIMASI](mailto:info@gimasi.ch)
   for pricing and information.

# Documentation

* [Doxygen API reference](http://gimasi.github.io/osG)
