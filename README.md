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

#pragma pack(push, 1)

struct Data {
  char cstr[50];
};

#pragma pack(pop)

auto main(int argc, char* argv[]) -> int {

  using namespace pmmf;

  if (argc < 2) {
    std::cerr << "Required 1 file paths!" << std::endl;
    return -1;
  }

  Data data_1 = {"Lorem ipsum dolor sit amet, consectetur efficitur"};
  Data data_2 = {"Lorem ipsum dolor sit amet, consectetur tincidunt"};

  MappedFile mapped_file(argv[1]);
  if(!mapped_file.isFileOpen()) {
    std::cerr << "Can't open \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "File \"" << argv[1] << "\" is open" << std::endl
            << "File size: " << mapped_file.getFileSize() << " bytes" << std::endl;

  MappedArray<Data> second_mapped_array = mapped_file.getMappedArray<Data>(sizeof(Data));
  if(!second_mapped_array.isMapped()) {
    std::cerr << "Can't map second area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "Second area of \"" << argv[1] << "\" file is mapped" << std::endl;

  MappedArray<Data> first_mapped_array = mapped_file.getMappedArray<Data>(0);
  if(!second_mapped_array.isMapped()) {
    std::cerr << "Can't map first area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "First area of \"" << argv[1] << "\" file is mapped" << std::endl;

  new(&*first_mapped_array) Data(data_1);
  second_mapped_array.flush();
  std::cout << "First area of \"" << argv[1] << "\" file is flushed" << std::endl;

  new(&*second_mapped_array) Data(data_2);
  second_mapped_array.flush();
  std::cout << "Second area of \"" << argv[1] << "\" file is flushed" << std::endl;

  std::cout << "File size: " << mapped_file.getFileSize() << " bytes" << std::endl;

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
  MemoryMappedFile target_file(argv[2], source_file.getFileSize(), Protocol::rwt, MapFlag::priv, 0);
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
