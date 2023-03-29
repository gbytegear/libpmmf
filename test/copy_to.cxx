#include "include/mmf.hxx"
#include <iostream>

#include <cstring>


auto main(int argc, char* argv[]) -> int {

  using namespace pmmf;

  if (argc < 3) {
    std::cout << "Required 2 file paths!" << std::endl;
    return -1;
  }

  MemoryMappedFile source_file(argv[1], Protocol::read, MapFlag::priv);
  if(!source_file.isFileOpen()) {
    std::cout << "Can't open \"" << argv[1] << "\" file (source)" << std::endl;
    return -1;
  } else if(!source_file.isFileMapped()) {
    std::cout << "Can't map \"" << argv[1] << "\" file (source)" << std::endl;
    return -1;
  }

  MemoryMappedFile target_file(argv[2], source_file.getFileSize(), Protocol((int)Protocol::read | (int)Protocol::write), MapFlag::priv, 0);
  if(!target_file.isFileOpen()) {
    std::cout << "Can't open \"" << argv[2] << "\" file (target)" << std::endl;
    return -1;
  } else if(!target_file.isFileMapped()) {
    std::cout << "Can't map \"" << argv[2] << "\" file (target)" << std::endl;
    return -1;
  }

  memcpy(target_file.getPageStart<void>(), source_file.getPageStart<void>(), source_file.getFileSize());
  if(!target_file.flush()) {
    std::cout << "Can't flush \"" << argv[2] << "\" file" << std::endl;
    return -1;
  }

  return 0;
}
