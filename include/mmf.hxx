#ifndef MMF_H
#define MMF_H

#include <cstddef>
#include <string>

#include "mmap_wrapper.hxx"

namespace pmmf {

class MemoryMappedFile {
protected:
  FileDescriptor file_descriptor = INVALID_FILE_DESCRIPTOR;
  void* memory_page_start = nullptr;
  std::size_t mapped_size = 0;

  void clearData();

  static FileDescriptor openFile(std::string file_path, Protocol prot);

public:
  MemoryMappedFile(MemoryMappedFile&& other)
    : file_descriptor(other.file_descriptor),
      memory_page_start(other.memory_page_start),
      mapped_size(other.mapped_size) { other.clearData(); }

  MemoryMappedFile(std::string file_path,
                   std::size_t map_size,
                   Protocol prot,
                   MapFlag flags,
                   std::size_t offset);

  MemoryMappedFile(std::string file_path,
                   Protocol prot,
                   MapFlag flags);

  MemoryMappedFile(std::string file_path);

  ~MemoryMappedFile();

  inline bool isFileOpen() const { return file_descriptor != INVALID_FILE_DESCRIPTOR; }
  inline bool isFileMapped() const { return memory_page_start; }

  inline FileDescriptor getFileDescriptor() const { return file_descriptor; }
  inline std::size_t getMappedSize() const { return mapped_size; }
  std::size_t getFileSize() const;
  template<typename T = void> inline T* getPageStart() const { return reinterpret_cast<T*>(memory_page_start); }
  template<typename T> inline T* get(std::size_t index, std::size_t byte_offset) const {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(memory_page_start) + byte_offset) + index;
  }

  bool flush();
};

}

#undef OS_DEP

#endif //MMF_H
