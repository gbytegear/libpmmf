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
  std::cout << "File \"" << argv[1] << "\" is open" << std::endl;

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

  return 0;
}
