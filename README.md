# Portable Memory-Mapped File library
libpmmf - [Portable Memory-Mapped File](https://en.wikipedia.org/wiki/Memory-mapped_file) library for C++

## Build:

Build pmmf library for Linux (required [GCC](https://gcc.gnu.org/), [CMake](https://cmake.org/download/), [Make](https://www.gnu.org/software/make/)):

```bash
git clone git@github.com:gbytegear/libpmmf.git
cd ./libpmmf/build
cmake . -G "Unix Makefiles" . -DCMAKE_BUILD_TYPE=Release
make pmmf
```

Result - `libpmmf/build/libpmmf.a`

Build pmmf library for Windows (required [MSys2](https://www.msys2.org/) step 1-5, [MinGW](https://www.msys2.org/) step 6-7, [CMake](https://cmake.org/), [Make](https://packages.msys2.org/package/make)):

```bash
git clone git@github.com:gbytegear/libpmmf.git
cd ./libpmmf/build
cmake . -G "MSYS Makefiles" . -DCMAKE_BUILD_TYPE=Release
make pmmf
```

Result - `libpmmf/build/libpmmf.a`

## Usage:

Link with your project:

```bash
g++ <your_project_source_file> -L<path_to_libpmmf.a> -I<path_to_libpmmf/include> -lpmmf
```

A simple example of outputting a text file to the console, modifying it, and then outputting it again to the console:

```cpp
#include "include/mmf.hxx"
#include <iostream>
#include <cstring> // Required for memcpy on Linux

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cout << "Required file paths!" << std::endl;
    return -1;
  }

  for(auto arg = argv + 1; argc != 1; (--argc, ++arg)) {
    std::cout << "Try to open \"" << *arg << '"' << std::endl;

    pmmf::MemoryMappedFile mapped_file(*arg); // <---------------------------------------------- Open file
    if(!mapped_file.isFileOpen()) { // <-------------------------------------------------------- Check file is open
      std::cout << "Can't open \"" << *arg << "\" file" << std::endl;
      continue;
    } else if(!mapped_file.isFileMapped()) { // <------------------------------------------------ Check file is mapped
      std::cout << "Can't map \"" << *arg << "\" file" << std::endl;
      continue;
    }

    std::cout << "File \"" << *arg << "\":" << std::endl
              << mapped_file.getPageStart<char>() << std::endl; // <----------------------------- Get pointer to start of file mapped area as char*

    std::memcpy(mapped_file.getPageStart<char>(), "Hello world " , strlen("Hello world ")); //<-- Set "Hello World" to start of file

    if(!mapped_file.flush()) { // <-------------------------------------------------------------- Write changes to file
      std::cout << "Can't flush \"" << *arg << "\" file" << std::endl;
    }

    std::cout << "Changed File \"" << *arg << "\":" << std::endl
              << mapped_file.getPageStart<char>() << std::endl; // <----------------------------- Get pointer to start of file mapped area as char*
  }

  return 0;
}
```

An example of copying the contents of one file to another:

```cpp
#include "include/mmf.hxx"

#include <iostream>
#include <cstring> // Required for memcpy on Linux


int main(int argc, char* argv[]) {

  using namespace pmmf;

  if (argc < 3) {
    std::cout << "Required 2 file paths!" << std::endl;
    return -1;
  }

  MemoryMappedFile source_file(argv[1], Protocol::read, MapFlag::priv);// <---------------------------------------------- Open first file
  if(!source_file.isFileOpen()) { // <----------------------------------------------------------------------------------- Check file is open
    std::cout << "Can't open \"" << argv[1] << "\" file (source)" << std::endl;
    return -1;
  } else if(!source_file.isFileMapped()) { // <-------------------------------------------------------------------------- Check file is mapped
    std::cout << "Can't map \"" << argv[1] << "\" file (source)" << std::endl;
    return -1;
  }

  // Open second file, with mapped area size `source_file.getFileSize()`, in rw-mode, trunk file on open
  MemoryMappedFile target_file(argv[2], source_file.getFileSize(), Protocol((int)Protocol::read | (int)Protocol::write | (int)Protocol::open_trunc), MapFlag::priv, 0);
  if(!target_file.isFileOpen()) { // <----------------------------------------------------------------------------------- Check file is open
    std::cout << "Can't open \"" << argv[2] << "\" file (target)" << std::endl;
    return -1;
  } else if(!target_file.isFileMapped()) { // <-------------------------------------------------------------------------- Check file is mapped
    std::cout << "Can't map \"" << argv[2] << "\" file (target)" << std::endl;
    return -1;
  }

  memcpy(target_file.getPageStart<void>(), source_file.getPageStart<void>(), source_file.getFileSize()); // <------------ Copy data from first file to second file
  if(!target_file.flush()) { // <---------------------------------------------------------------------------------------- Write changes to file
    std::cout << "Can't flush \"" << argv[2] << "\" file" << std::endl;
    return -1;
  }

  return 0;
}
```
