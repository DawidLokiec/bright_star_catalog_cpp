# The Bright Star Catalog 5 for C++ projects
This CMake project contains an C++-API to parse the Bright Star Catalog in version 5 according to the 
binary format described at http://tdc-www.harvard.edu/catalogs/bsc5.html.
## How to include into your project?
This library can be added into your project using git submodules. In the root directory of your project call the
following command: 
```git
git submodule add https://github.com/DawidLokiec/bright_star_catalog_cpp.git lib/bright_star_catalog_cpp
```
This will clone the latest commit of this library into your project.
After this step add this dependency into your CMakeLists.txt as follows:
```cmake
# ...
add_subdirectory(lib/bright_star_catalog_cpp)
target_link_libraries(<target name> bright_star_catalog_cpp)
```
## API-Usage
Just include the `bright_star_catalog_parser.h` header file into your source code and call the parse function:
```c++
#include "bright_star_catalog_parser.h"
// ...
BrightStarCatalog::v5::ParsingResult parsingResult = BrightStarCatalog::v5::parse();
```
This function call will parse the supplied binary file, respecting to the native byte order.
At this point it should be noted that only the byte orders **little and big endian are supported** by this function.
However, there is the overloaded function for specifying the path to your own binary file:
```c++
#include "bright_star_catalog_parser.h"
// ...
BrightStarCatalog::v5::ParsingResult parsingResult = BrightStarCatalog::v5::parse("yourFilepath.bin");
```
---
Interesting pull requests are always welcome.