
#include "elf_sections.h"

void readELFSecitons(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader)
{
    // 检查文件是否包含节头表
    if (header.e_shoff == 0 || header.e_shnum == 0) {
        std::cerr << "该文件内不包含节头表\n";
        return;
    }

    // 将指针移动到节头表的起始位置
    file.seekg(header.e_shoff, std::ios::beg);

    // 读取节头表

    file.read(reinterpret_cast<char*>(sHeader.data()), header.e_shnum * header.e_shentsize);
    if (!file) {
        std::cerr << "读取节区头部表错误\n";
        return;
    }

    // 读取节头字符串表
    const Elf64_Shdr& shstrtabHeader = sHeader[header.e_shstrndx];
    std::vector<char> shstrtab(shstrtabHeader.sh_size);
    file.seekg(shstrtabHeader.sh_offset, std::ios::beg);
    file.read(shstrtab.data(), shstrtabHeader.sh_size);
    if (!file) {
        std::cerr << "读取节区头部字符表错误\n";
        return;
    }
    disPlaySHeader(sHeader, shstrtab);
}

void disPlaySHeader(std::vector<Elf64_Shdr> &sectionHeaders, std::vector<char> &shstrtab)
{
    std::map<Elf64_Word, std::string> typeMap = {
        {0, "NULL"},
        {1, "PROGBITS"},
        {2, "SYMTAB"},
        {3, "STRTAB"},
        {4, "RELA"},
        {5, "HASH"},
        {6, "DYNAMIC"},
        {7, "NOTE"},
        {8, "NOBITS"},
        {9, "REL"},
        {10, "SHLIB"},
        {11, "DYNSYM"},
        {0x70000000, "LOPROC"},
        {0x7fffffff, "HIPROC"},
        {0x80000000, "LOUSER"},
        {0X8FFFFFFF, "HIUSER"}
    };
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
    std::cout << std::left << std::setfill(' ') << "对齐" << std::endl;
    for (size_t i = 0; i < sectionHeaders.size(); ++i) {
        const Elf64_Shdr& section = sectionHeaders[i];
        std::string sectionName(&shstrtab[section.sh_name]);
        std::cout << "  [" << std::right << std::setw(2) << std::setfill(' ') <<std::dec << i << "] ";
        std::cout << std::left << std::setw(20) << std::setfill(' ') << sectionName;
        std::cout << std::left << std::setw(20) << std::setfill(' ') << typeMap[section.sh_type];
        std::cout << std::right << std::setw(16) << std::setfill('0') << std::hex << section.sh_addr << "    ";
        std::cout << std::setw(10) << std::setfill('0') << std::hex << section.sh_offset << std::endl;
        std::cout << std::setw(7) << std::setfill(' ') << ' ';
        std::cout << std::setw(16) << std::setfill('0') << std::hex << section.sh_size << "    ";
        std::cout << std::setw(16) << std::setfill('0') << std::hex << section.sh_entsize << "    ";
        std::cout << std::left << std::setw(10) << std::setfill(' ') << getFlag(section.sh_flags);
        std::cout << std::left << std::setw(10) << std::setfill(' ') << std::dec << section.sh_link;
        std::cout << std::left << std::setw(10) << std::setfill(' ') << section.sh_info;
        std::cout << std::left << std::setw(10) << std::setfill(' ') << section.sh_addralign << std::endl;
    }
    std::string ps = R"(Key to Flags: 
    W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
    L (link order), O (extra OS processing required), G (group), T (TLS),
    C (compressed), x (unknown), o (OS specific), E (exclude),
    l (large), p (processor specific)\n)";
    std::cout << ps <<std::endl;
    
}


std::string getFlag(Elf64_Xword flags) {
    std::map<Elf64_Xword, char> flagMap = {
        {0x1, 'W'},
        {0x2, 'A'},
        {0x4, 'X'},
        {0x10, 'M'},
        {0x20, 'S'},
        {0x40, 'I'},
        {0x80, 'L'},
        {0x100, 'O'},
        {0x200, 'G'},
        {0x400, 'T'},
        {0x800, 'C'},
        {0x1000, 'x'},
        {0x2000, 'o'},
        {0x4000, 'E'},
        {0x8000, 'l'},
        {0x10000, 'p'}
    };
    std::string res = "";
    for (const auto iter:flagMap) {
        if (flags & iter.first) {
            res += iter.second;
        }
    }
    return res;
}


