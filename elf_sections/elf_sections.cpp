#include "elf_sections.h"

void readELFSecitons(std::ifstream &file, Elf64_Ehdr &header, Elf64_Shdr &sHeader)
{
    // 检查文件是否包含节头表
    if (header.e_shoff == 0 || header.e_shnum == 0) {
        std::cerr << "No section headers in this ELF file\n";
        return;
    }

    // 将指针移动到节头表的起始位置
    file.seekg(header.e_shoff, std::ios::beg);

    // 读取节头表
    std::vector<Elf64_Shdr> sectionHeaders(header.e_shnum);
    file.read(reinterpret_cast<char*>(sectionHeaders.data()), header.e_shnum * header.e_shentsize);
    if (!file) {
        std::cerr << "Error reading section headers\n";
        return;
    }

    // 读取节头字符串表
    const Elf64_Shdr& shstrtabHeader = sectionHeaders[header.e_shstrndx];
    std::vector<char> shstrtab(shstrtabHeader.sh_size);
    file.seekg(shstrtabHeader.sh_offset, std::ios::beg);
    file.read(shstrtab.data(), shstrtabHeader.sh_size);
    if (!file) {
        std::cerr << "Error reading section header string table\n";
        return;
    }
    disPlayHeader(sectionHeaders,shstrtab);

}

void disPlayHeader(std::vector<Elf64_Shdr> sectionHeaders,std::vector<char> shstrtab)
{
    // 打印节信息
    for (size_t i = 0; i < sectionHeaders.size(); ++i) {
        const Elf64_Shdr& section = sectionHeaders[i];
        std::string sectionName(&shstrtab[section.sh_name]);
        printSectionHeader(section, sectionName);
    }

}

void printSectionHeader(const Elf64_Shdr& section, const std::string& name) {
    std::cout << "Section: " << name << std::endl;
    std::cout << "  Type:             " << section.sh_type << std::endl;
    std::cout << "  Address:          0x" << std::hex << section.sh_addr << std::dec << std::endl;
    std::cout << "  Offset:           " << section.sh_offset << " (bytes into file)" << std::endl;
    std::cout << "  Size:             " << section.sh_size << " (bytes)" << std::endl;
    std::cout << "  Link:             " << section.sh_link << std::endl;
    std::cout << "  Info:             " << section.sh_info << std::endl;
    std::cout << "  Address alignment: " << section.sh_addralign << std::endl;
    std::cout << "  Entry size:       " << section.sh_entsize << " (bytes)" << std::endl;
}


