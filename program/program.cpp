#include "program.h"

void readELFProgramHeader(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Phdr> &pHeader)
{
    // 检查文件是否包含程序头表
    if (header.e_phoff == 0 || header.e_phnum == 0) {
        std::cerr << "该文件内不包含程序头表\n";
        return;
    }

    // 将指针移动到程序头表的起始位置
    file.seekg(header.e_phoff, std::ios::beg);

    // 读取程序头表
    file.read(reinterpret_cast<char*>(pHeader.data()), header.e_phnum * header.e_phentsize);
    if (!file) {
        std::cerr << "读取程序头表错误\n";
        return;
    }
    diplayProgramHeader(pHeader, file);

}

void diplayProgramHeader(std::vector<Elf64_Phdr> &pHeader, std::ifstream &file) {
    std::map<Elf64_Word, std::string> type = {
        {0, "NULL"},
        {1, "LOAD"},
        {2, "DYNAMIC"},
        {3, "INTERP"},
        {4, "NOTE"},
        {5, "SHLIB"},
        {6, "PHDR"},
        {7, "TLS"},
        {8, "NUM"},
        {0x6474e550, "GNU_EH_FRAME"},
        {0x6474e551, "GNU_STACK"},
        {0x6474e552, "GNU_RELRO"},
        {0x6474e553, "GNU_PROPERTY"}
    };

    std::map<Elf64_Word, char> flags = {
        {1, 'E'},
        {2, 'W'},
        {4, 'R'}
    };

    std::cout << "程序头:" << std::endl;
    std::cout << "  " << std::left << std::setw(15) << std::setfill(' ') << "类型";
    std::cout << "  " << std::left << std::setw(19) << std::setfill(' ') << "偏移";
    std::cout << "  " << std::left << std::setw(19) << std::setfill(' ') << "虚拟地址";
    std::cout << "  " << std::left << std::setw(19) << std::setfill(' ') << "物理地址" << std::endl;
    std::cout << std::setw(17) << std::setfill(' ') << " ";
    std::cout << std::left << std::setw(24) << std::setfill(' ') << "段在文件的大小";
    std::cout << "  " << std::left << std::setw(24) << std::setfill(' ') << "段在内存的大小";
    std::cout << "  " << std::left << std::setw(9) << std::setfill(' ') << "标志";
    std::cout << "  " << std::left << std::setw(9) << std::setfill(' ') << "对齐" <<std::endl;

    for (Elf64_Phdr pro:pHeader) {
        std::cout << "  " << std::left << std::setw(15) << std::setfill(' ') << (type.count(pro.p_type)?type[pro.p_type]:"UNKNOWN");
        std::cout << "0x" << std::right << std::setw(16) << std::setfill('0') << std::hex << pro.p_offset << " ";
        std::cout << "0x" << std::right << std::setw(16) << std::setfill('0') << pro.p_vaddr << " ";
        std::cout << "0x" << std::right << std::setw(16) << std::setfill('0') << pro.p_paddr << std::endl;
        std::cout << std::setw(17) << std::setfill(' ') << " ";
        std::cout << "0x" << std::right << std::setw(16) << std::setfill('0') << pro.p_filesz << " ";
        std::cout << "0x" << std::right << std::setw(16) << std::setfill('0') << pro.p_memsz << " ";
        std::cout << ((pro.p_flags&4)?flags[4]:' ') << ((pro.p_flags&2)?flags[2]:' ') << ((pro.p_flags&1)?flags[1]:' ') << "      ";
        std::cout << "0x" << pro.p_align << std::endl;
        if (pro.p_type == 3) {
            // 将指针移动到程序头表的起始位置
            file.seekg(pro.p_offset, std::ios::beg);
            std::vector<char> interp(pro.p_filesz);
            file.read(interp.data(), pro.p_filesz);
            std::string interpStr(interp.data());
            std::cout << "      [Requesting program interpreter: " << interpStr << "]" << std::endl;
        }
    }
}