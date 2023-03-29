#include "include/mmf.hxx"
#include <iostream>
#include <cstring>
#include <map>
#include <cctype>
#include <cstdlib>


char* copyStringSafe(const char* dest, std::size_t dest_buffer_size, const char* src) {
  if(!dest || !src) return;
  --dest_buffer_size; // reserve for null-termination;
  const char* dest_it = dest;
  for(; *src != '\0' && dest_buffer_size != 0; (++src, ++dest_it, --dest_buffer_size))
    *dest_it = *src;
  *(dest_it + 1) = '\0';
  return dest;
}


#pragma pack(push, 1)

class DataEntry {
  char name[21];
  char last_name[21];
  char phone_number[19];
  char email[255];
public:
  DataEntry(const char* name, const char* last_name, const char* phone_number, const char* email) {
    copyStringSafe(this->name, 21, name);
    copyStringSafe(this->last_name, 21, last_name);
    copyStringSafe(this->phone_number, 19, phone_number);
    copyStringSafe(this->email, 21, email);
  }

  const char* getName() const { return name; }
  const char* getLastName() const { return last_name; }
  const char* getPhoneNumber() const { return phone_number; }
  const char* getEmail() const { return email; }
};

#pragma pack(pop)


class CLI {
  enum class CLIState {
    enter_command = 0x00,
    quit = 0x01,
    print = 0x02,
    enter_index = 0x03,
    add = 0x04,
    enter_name = 0x05,
    enter_last_name = 0x06,
    enter_phone_number = 0x07,
    enter_email = 0x08,
    change = 0x09,
    empty = 0xFF
  };

  std::size_t& entry_count;
  DataEntry* entry_array;

  CLIState function_state = CLIState::empty;
  CLIState state = CLIState::enter_command;
  std::string command;
  std::string::iterator cmd_it;

  std::string name;
  std::string last_name;
  std::string phone_number;
  std::string email;

  std::size_t index;


  void print(std::size_t index) {
    if(index >= entry_count) {
      std::cout << "Out of ragnge!" << std::endl;
      return;
    }

    DataEntry& entry = entry_array[index];
    std::cout << "Name: " << entry.getName() << ';'<< std::endl
              << "Last name: " << entry.getLastName() << ';' << std::endl
              << "Phone number: " << entry.getPhoneNumber() << ';' << std::endl
              << "Email: " << entry.getEmail() << ';' << std::endl;

  }




  const char* getCLIPrompt(CLIState state) {
    switch(state) {
    case CLIState::enter_command: return "Enter command: ";
    case CLIState::quit: return "";
    case CLIState::print: return "";
    case CLIState::enter_index: return "Index > ";
    case CLIState::add: return "";
    case CLIState::enter_name: return "Name > ";
    case CLIState::enter_last_name: return "Last name > ";
    case CLIState::enter_phone_number: return "Phone number > ";
    case CLIState::enter_email: return "Email > ";
    case CLIState::change: return "";
    }
  }

  static bool isWhiteSpaceOrNull(char ch) {
    return isspace(ch) || ch = '\0';
  }

  static std::string::iterator& skipWhiteSpace(std::string::iterator& it) {
    while (isspace(*it)) ++it;
    return it;
  }

  static std::string::iterator& skipBeforeWhiteSpace(std::string::iterator& it) {
    while (!isspace(*it)) ++it;
    return it;
  }

  static CLIState parseCommand(std::string::iterator& it) {
    static const std::map<std::string, CLIState> state_map {
      {"quit", CLIState::quit},
      {"print", CLIState::print},
      {"add", CLIState::add},
      {"change", CLIState::change}
    };

    if(*skipWhiteSpace(it) == '\0') return CLIState::enter_command;

    std::string::iterator word_begin = it;
    std::string word(word_begin, skipBeforeWhiteSpace(it));
    auto maybe_command = state_map.find(word);
    if(maybe_command == state_map.cend()) {
      std::cerr << "Invalid command \"" << word << '"' << std::endl;
      return CLIState::enter_command;
    }

    skipWhiteSpace(it);

    return *maybe_command;
  }

  void inputCommand(CLIState state) {
    std::cout << getCLIPrompt(state);
    std::getline(std::cin, command);
    cmd_it = command.begin();
  }

public:
  CLI(pmmf::MemoryMappedFile& mapped_file)
    : entry_count(*mapped_file.getPageStart<std::size_t>()),
      entry_array(mapped_file.get<DataEntry>(0, sizeof(std::size_t))) {
    std::cout << "Data file contains " << entry_count << " entry(s).\n";
  }

  int run() {
    while(1) {
      switch (state) {
      case CLIState::enter_command:
        inputCommand(state);
        state = parseCommand(cmd_it);
      continue;
      case CLIState::quit: return 0;
      case CLIState::print:
        buffer_state = state;
        state = CLIState::enter_index;
      continue;
      case CLIState::enter_index: {
        std::string index_str;
        while(1) {
          if(!getWord(cmd_it, index_str)) {
            inputCommand(CLIState::enter_index);
            continue;
          }

          if(!std::all_of(index_str.begin(), index_str.end(), ::isdigit)) {
            std::cout << "Error: required number argument!" << std::endl;
            inputCommand(CLIState::enter_index);
            continue;
          }

          switch(function_state) {
          case CLIState::print: print(std::strtoull(index_str.data(), index_str.data() + index_str.length(), 10));
          }
        }
      }
      continue;
      case CLIState::add:
        break;
      case CLIState::enter_name:
        break;
      case CLIState::enter_last_name:
        break;
      case CLIState::enter_phone_number:
        break;
      case CLIState::enter_email:
        break;
      case CLIState::change:
        break;
      }
    }
  }

};





bool getWord(std::string::iterator& it, std::string& to) {
  std::string::iterator word_begin = it;
  to = std::string(word_begin, skipBeforeWhiteSpace(it));
  return !to.empty()
}

auto main(int argc, char* argv[]) -> int {
  using namespace pmmf;

  MemoryMappedFile data_file(argv[1], sizeof(std::size_t) + sizeof(DataEntry) * 1024);
  if(!source_file.isFileOpen()) {
    std::cout << "Can't open \"" << argv[1] << "\" file" << std::endl;
    return -1;
  } else if(!source_file.isFileMapped()) {
    std::cout << "Can't map \"" << argv[1] << "\" file" << std::endl;
    return -1;
  }

  return CLI(data_file).run();
}
