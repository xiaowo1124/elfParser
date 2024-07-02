#include <iomanip>

#include "elf_sections.h"

void readELFSecitons(std::ifstream &file, Elf64_Ehdr &header, Elf64_Shdr &sHeader)
{
    // 检查文件是否包含节头表
    if (header.e_shoff == 0 || header.e_shnum == 0) {
        std::cerr << "该文件内不包含节头表\n";
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
    std::cout << "节头:" << std::endl;
    std::cout << "  [号] " << std::left << std::setw(22) << std::setfill(' ') << "名称";
    std::cout << std::left << std::setw(22) << std::setfill(' ') << "类型";
    std::cout << std::left << std::setw(22) << std::setfill(' ') << "地址";
    std::cout << std::left << std::setw(10) <<std::setfill(' ') << "偏移量" << std::endl;
    std::cout << std::setw(7) << std::setfill(' ') << ' ';
    std::cout << std::left << std::setw(22) << std::setfill(' ') << "大小";
    std::cout << std::left << std::setw(24) << std::setfill(' ') << "条目大小";
    std::cout << std::left << std::setw(12) << std::setfill(' ') << "旗标";
    std::cout << std::left << std::setw(12) << std::setfill(' ') << "链接";
    std::cout << std::left << std::setw(12) << std::setfill(' ') << "信息";
    std::cout << std::left << std::setw(12) << std::setfill(' ') << "对齐" << std::endl;
    for (size_t i = 0; i < sectionHeaders.size(); ++i) {
        const Elf64_Shdr& section = sectionHeaders[i];
        std::string sectionName(&shstrtab[section.sh_name]);
        std::cout << "  [" << std::right << std::setw(2) << std::setfill(' ') <<std::dec << i << "] ";
        printSectionHeader(section, sectionName);
    }
}

void printSectionHeader(const Elf64_Shdr& section, const std::string& name) {
    std::cout << std::left << std::setw(20) << std::setfill(' ') << name;
    std::cout << std::left << std::setw(20) << std::setfill(' ') << section.sh_type;
    std::cout << std::right << std::setw(16) << std::setfill('0') << std::hex << section.sh_addr << "    ";
    std::cout << std::setw(10) << std::setfill('0') << std::hex << section.sh_offset << std::endl;
    std::cout << std::setw(7) << std::setfill(' ') << ' ';
    std::cout << std::setw(16) << std::setfill('0') << std::hex << section.sh_size << "    ";
    std::cout << std::setw(16) << std::setfill('0') << std::hex << section.sh_entsize << "    ";
    std::cout << std::left << std::setw(10) << std::setfill(' ') << section.sh_flags;
    std::cout << std::left << std::setw(10) << std::setfill(' ') << std::dec << section.sh_link;
    std::cout << std::left << std::setw(10) << std::setfill(' ') << section.sh_info;
    std::cout << std::left << std::setw(10) << std::setfill(' ') << section.sh_addralign << std::endl;
}


