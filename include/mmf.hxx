#ifndef MMF_H
#define MMF_H

#include <cstddef>
#include <string>

#include "mmap_wrapper.hxx"

namespace pmmf {

//! Memory-Mapped file
class MemoryMappedFile {
protected:
  FileDescriptor file_descriptor = INVALID_FILE_DESCRIPTOR;
  void* memory_page_start = nullptr;
  std::size_t mapped_size = 0;

  void clearData();

  static FileDescriptor openFile(std::string file_path, Protocol prot);

public:
  /**
   * @brief MemoryMappedFile - move data from other MemoryMappedFile object
   * @param other - other object of MemoryMappedFile type
   */
  MemoryMappedFile(MemoryMappedFile&& other)
    : file_descriptor(other.file_descriptor),
      memory_page_start(other.memory_page_start),
      mapped_size(other.mapped_size) { other.clearData(); }

  /**
   * @brief MemoryMappedFile - create new MemoryMappedFile object with setting all attributes
   * @param file_path - path to file for memory maping
   * @param map_size - size for memory mapping
   * @param prot - the prot argument describes the desired memory protection mode. See enum 'Protocol' in "mmap_wrapper.hxx" file.
   * @param flags - the flags parameter specifies the type of object to reflect, the reflection options, and whether the reflected data belongs only to this process or can be read by others. See enum 'MapFlag' in "mmap_wrapper.hxx" file
   * @param offset - offset in file before block for memory mapping
   */
  MemoryMappedFile(std::string file_path,
                   std::size_t map_size = 0,
                   Protocol prot = Protocol((int)Protocol::read | (int)Protocol::write),
                   MapFlag flags = MapFlag::priv,
                   std::size_t offset = 0);

  /**
   * @brief MemoryMappedFile - create new MemoryMappedFile object with setting all flags-attributes, default arguments: map_size = getFileSize(), offset = 0
   * @param file_path - path to file for memory maping
   * @param prot - the prot argument describes the desired memory protection mode. See enum 'Protocol' in "mmap_wrapper.hxx" file.
   * @param flags - the flags parameter specifies the type of object to reflect, the reflection options, and whether the reflected data belongs only to this process or can be read by others. See enum 'MapFlag' in "mmap_wrapper.hxx" file
   */
  MemoryMappedFile(std::string file_path,
                   Protocol prot,
                   MapFlag flags);

  /**
   * @brief MemoryMappedFile - create new MemoryMappedFile object with setting file_path, default arguments: map_size = getFileSize(), offset = 0, prot = Protocol((int)Protocol::read | (int)Protocol::write), flags = MapFlag::priv
   * @param file_path
   */
  MemoryMappedFile(std::string file_path);

  ~MemoryMappedFile();

  /**
   * @brief isFileOpen - checks if a file is open
   * @return true - if file is open, false - if file isn't open
   */
  inline bool isFileOpen() const { return file_descriptor != INVALID_FILE_DESCRIPTOR; }

  /**
   * @brief isFileMapped - checks if a file is mapped in memory
   * @return true - if file is mapped in memory, false - if file isn't mapped in memory
   */
  inline bool isFileMapped() const { return memory_page_start; }

  /**
   * @brief getFileDescriptor - grants access to a file descriptor
   * @return file descriptor stored in this object
   */
  inline FileDescriptor getFileDescriptor() const { return file_descriptor; }

  /**
   * @brief getMappedSize -Returns the size of the memory-mapped area of the file
   * @return memory-mapped area of the file
   */
  inline std::size_t getMappedSize() const { return mapped_size; }

  /**
   * @brief getFileSize - Returns the file size
   * @return file size
   */
  std::size_t getFileSize() const;
  template<typename T = void> inline T* getPageStart() const { return reinterpret_cast<T*>(memory_page_start); }
  template<typename T> inline T* get(std::size_t index, std::size_t byte_offset) const {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(memory_page_start) + byte_offset) + index;
  }

  /**
   * @brief flush - synchronizes the contents of a file with its mapped area in memory
   * @return true - if the operation was successful, false - if the operation failed
   */
  bool flush();

  /**
   * @brief operator = - move data of other MemoryMappedFile object to this
   * @param other - temporary MemoryMappedFile object
   * @return reference to this object
   */
  inline MemoryMappedFile& operator = (MemoryMappedFile&& other) {
    this->~MemoryMappedFile();
    return *new(this) MemoryMappedFile(std::move(other));
  }
};

}

#undef OS_DEP

#endif //MMF_H
