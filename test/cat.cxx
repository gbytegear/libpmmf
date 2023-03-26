#include "include/mmf.hxx"
#include <iostream>

#include <cstring>


auto main(int argc, char* argv[]) -> int {

  if (argc < 2) {
    std::cout << "Required file paths!" << std::endl;
    return -1;
  }

  for(auto arg = argv + 1; argc != 1; (--argc, ++arg)) {
    std::cout << "Try to open \"" << *arg << '"' << std::endl;
    pmmf::MemoryMappedFile mapped_file(*arg);
    if(!mapped_file.isFileOpen()) {
      std::cout << "Can't open \"" << *arg << "\" file" << std::endl;
      continue;
    } else if(!mapped_file.isFileMapped()) {
      std::cout << "Can't map \"" << *arg << "\" file" << std::endl;
      continue;
    }

    std::cout << "File \"" << *arg << "\":" << std::endl
              << mapped_file.getPageStart<char>() << std::endl;

    std::memcpy(mapped_file.getPageStart<char>(), "Hello world " , strlen("Hello world "));

    if(!mapped_file.flush()) {
      std::cout << "Can't flush \"" << *arg << "\" file" << std::endl;
    }

    std::cout << "Changed File \"" << *arg << "\":" << std::endl
              << mapped_file.getPageStart<char>() << std::endl;
  }




  return 0;
}
