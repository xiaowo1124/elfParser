#include "elf_header.h"

void readELFHeader(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    ELF_Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!file) {
        std::cerr << "Failed to read ELF header" << std::endl;
        return;
    }

    if ((header.e_ident[0] ^ 127) || (header.e_ident[1] ^ 69) || (header.e_ident[2] ^ 76) || (header.e_ident[3] ^ 70)) {
        std::cout<<"这不是一个elf文件"<<std::endl;
        exit(0);
    } else {
        std::cout<<"elf file"<<std::endl;
    }

    // Print ELF header information for demonstration
    std::cout << "ELF头: "<<std::endl;
    std::cout << "  Magic: ";
    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << static_cast<int>(header.e_ident[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "  Type: " << header.e_type << std::endl;
    std::cout << "  Machine: " << header.e_machine << std::endl;
    std::cout << "  Version: " << header.e_version << std::endl;
    std::cout << "  Entry point address: " << header.e_entry << std::endl;
    std::cout << "  Start of program headers: " << header.e_phoff << " (bytes into file)" << std::endl;
    std::cout << "  Start of section headers: " << header.e_shoff << " (bytes into file)" << std::endl;
    std::cout << "  Flags: " << header.e_flags << std::endl;
    std::cout << "  Size of this header: " << header.e_ehsize << " (bytes)" << std::endl;
    std::cout << "  Size of program headers: " << header.e_phentsize << " (bytes)" << std::endl;
    std::cout << "  Number of program headers: " << header.e_phnum << std::endl;
    std::cout << "  Size of section headers: " << header.e_shentsize << " (bytes)" << std::endl;
    std::cout << "  Number of section headers: " << header.e_shnum << std::endl;
    std::cout << "  Section header string table index: " << header.e_shstrndx << std::endl;
}