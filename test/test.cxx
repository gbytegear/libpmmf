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

  MappedFile mapped_file(argv[1], ProtectionMode::rw, MapFlag::shared);
  if(!mapped_file.isFileOpen()) {
    std::cerr << "Can't open \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "File \"" << argv[1] << "\" is open" << std::endl
            << "| File size: " << mapped_file.getFileSize() << " bytes" << std::endl;



  MappedData<Data> second_mapped_array = mapped_file.getMappedData<Data>(sizeof(Data), 2);
  if(!second_mapped_array.isMapped()) {
    std::cerr << "Can't map second area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "Second area of \"" << argv[1] << "\" file is mapped" << std::endl;



  MappedData<Data> first_mapped_array = mapped_file.getMappedData<Data>(0);
  if(!second_mapped_array.isMapped()) {
    std::cerr << "Can't map first area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "First area of \"" << argv[1] << "\" file is mapped" << std::endl;



  Data& page_1_data_1 = *first_mapped_array;
  Data& page_2_data_1 = *second_mapped_array;
  Data& page_2_data_2 = second_mapped_array[1];



  new(&page_1_data_1) Data{"Lorem ipsum dolor sit amet, consectetur efficitur"};
  if(!first_mapped_array.flush()) {
    std::cerr << "Can't flush first area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "First area of \"" << argv[1] << "\" file is flushed" << std::endl
            << "| Recorded data: " << first_mapped_array->cstr << std::endl;



  new(&page_2_data_1) Data{"Lorem ipsum dolor sit amet, consectetur tincidunt"};
  new(&page_2_data_2) Data{"Lorem ipsum dolor sit amet, consectetur porttitor"};
  if(!second_mapped_array.flush()) {
    std::cerr << "Can't flush second area of \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }
  std::cout << "Second area of \"" << argv[1] << "\" file is flushed" << std::endl;


  for(auto& data : second_mapped_array)
    std::cout << "| Recorded data: " << data.cstr << std::endl;



  std::cout << "File size: " << mapped_file.getFileSize() << " bytes" << std::endl;

  return 0;
}
