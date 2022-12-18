# The Bright Star Catalog 5 for C++ projects
This CMake project contains an C++-API to parse the Bright Star Catalog in version 5 according to the 
binary format specification described at http://tdc-www.harvard.edu/catalogs/bsc5.html.
## Usage
Just add this CMake project into yours and include the `bright_star_catalog_parser.h` header file into your source code 
and call the parse function:
```c++
#include "bright_star_catalog_parser.h"
/// ...
BrightStarCatalog::v5::ParsingResult parsingResult = BrightStarCatalog::v5::parse();
```
This function call will parse the supplied binary file, respecting to the native byte order.
At this point it should be noted that only the byte orders **little and big endian are supported** by this function.
However, there is the overloaded function for specifying the path to your own binary file:
```c++
#include "bright_star_catalog_parser.h"
/// ...
BrightStarCatalog::v5::ParsingResult parsingResult = BrightStarCatalog::v5::parse("yourFilepath.bin");
```
---
Interesting pull requests are always welcome :)