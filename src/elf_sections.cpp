
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
}

void readshstrtab(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab){
    // 读取节头字符串表
    file.seekg(sHeader[header.e_shstrndx].sh_offset, std::ios::beg);
    file.read(shstrtab.data(), sHeader[header.e_shstrndx].sh_size);

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
    std::cout << std::left << "对齐" << std::endl;
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
        std::cout << std::left << section.sh_addralign << std::endl;
    }
    std::string ps = R"(Key to Flags: 
    W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
    L (link order), O (extra OS processing required), G (group), T (TLS),
    C (compressed), x (unknown), o (OS specific), E (exclude),
    l (large), p (processor specific))";
    std::cout << ps <<std::endl << std::endl;
    
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

void displayDynamic(std::ifstream &file, std::vector<Elf64_Shdr> &sHeader)
{
    std::map<int, std::string> tagMap = {
        {DT_NULL, "NULL"},
        {DT_NEEDED, "NEEDED"},
        {DT_PLTRELSZ, "PLTRELSZ"},
        {DT_PLTGOT, "PLTGOT"},
        {DT_HASH, "HASH"},
        {DT_STRTAB, "STRTAB"},
        {DT_SYMTAB, "SYMTAB"},
        {DT_RELA, "RELA"},
        {DT_RELASZ, "RELASZ"},
        {DT_RELAENT, "RELAENT"},
        {DT_STRSZ, "STRSZ"},
        {DT_SYMENT, "SYMENT"},
        {DT_INIT, "INIT"},
        {DT_FINI, "FINI"},
        {DT_SONAME, "SONAME"},
        {DT_RPATH, "RPATH"},
        {DT_SYMBOLIC, "SYMBOLIC"},
        {DT_REL, "REL"},
        {DT_RELSZ, "RELSZ"},
        {DT_RELENT, "RELENT"},
        {DT_PLTREL, "PLTREL"},
        {DT_DEBUG, "DEBUG"},
        {DT_TEXTREL, "TEXTREL"},
        {DT_JMPREL, "JMPREL"},
        {DT_BIND_NOW, "BIND_NOW"},
        {DT_INIT_ARRAY, "INIT_ARRAY"},
        {DT_FINI_ARRAY, "FINI_ARRAY"},
        {DT_INIT_ARRAYSZ, "INIT_ARRAYSZ"},
        {DT_FINI_ARRAYSZ, "FINI_ARRAYSZ"},
        {DT_NUM, "NUM"},
        {DT_LOOS, "LOOS"},
        {DT_LOPROC, "LOPROC"},
        {DT_HIPROC, "HIPROC"},
        {DT_PROCNUM, "PROCNUM"},
        {DT_VALRNGLO, "VALRNGLO"},
        {DT_POSFLAG_1, "POSFLAG_1"},
        {DT_SYMINSZ, "SYMINSZ"},
        {DT_SYMINENT, "SYMINENT"},
        {DT_ADDRRNGLO, "ADDRRNGLO"},
        {DT_SYMINFO, "SYMINFO"},
        {DT_VERSYM, "VERSYM"},
        {DT_FLAGS_1, "FLAGS_1"},
        {DT_VERDEF, "VERDEF"},
        {DT_VERDEFNUM, "VERDEFNUM"},
        {DT_VERNEED, "VERNEED"},
        {DT_VERNEEDNUM, "VERNEEDNUM"}
    };

    for (int i = 0; i < sHeader.size(); ++i) {
        if (sHeader[i].sh_type == SHT_DYNAMIC) {
            std::vector<Elf64_Dyn> dyn(sHeader[i].sh_size / sizeof(Elf64_Dyn));
            file.seekg(sHeader[i].sh_offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(dyn.data()), sHeader[i].sh_size);
            int nullpos = 0;
            std::vector<char> strtab(sizeof(Elf64_Dyn));
            for (int j = 0; j < dyn.size(); ++j) {
                if (dyn[j].d_tag == DT_STRTAB) {
                    file.seekg(dyn[j].d_un.d_ptr, std::ios::beg);
                    file.read(strtab.data(), sizeof(dyn[j].d_un.d_val));
                }
                if (dyn[j].d_tag == 0) {
                    nullpos = j;
                    break;
                }
            }

            std::cout << "偏移量为0x" << std::hex << sHeader[i].sh_offset << "的动态区段包含 " << std::dec << nullpos + 1 << "个条目" << std::endl;
            std::cout << "  标记              类型                名称/值" << std::endl;
            for (int j = 0; j < nullpos + 1; ++j) {
                std::cout << " 0x" << std::right << std::setw(16) << std::setfill('0') << std::hex << dyn[j].d_tag << " ";
                std::cout << std::left << std::setw(20) << std::setfill(' ') << (tagMap.count(dyn[j].d_tag)?tagMap[dyn[j].d_tag]:".etc") << std::setw(20) << std::setfill(' ');
                if (dyn[j].d_tag == DT_NEEDED || dyn[j].d_tag == DT_SONAME) {
                    std::cout << std::string(&strtab[dyn[j].d_un.d_val]) << std::endl;
                } else {
                    std::cout << dyn[j].d_un.d_val << std::endl;
                }
            }
        }
    }
    std::cout << std::endl;
}

std::string get_dynstr_name(std::ifstream &file,Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, Elf64_Word dynstrndx)
{
    for (int i = 0; i < header.e_shnum; ++i)
    {
        if (sHeader[i].sh_type == SHT_STRTAB)
        {
            std::vector<char> strtab(sHeader[i].sh_size);
            file.seekg(sHeader[i].sh_offset, std::ios::beg);
            file.read(strtab.data(), sHeader[i].sh_size);
            std::string dynstr(&strtab[dynstrndx]);
            return dynstr;
        }
    }
    return "";
}

void displayRelocation(std::ifstream &file,Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab)
{
    std::map<Elf32_Word, std::string> relTypeMap = {
        {R_ARM_NONE, "R_ARM_NONE"},
        {R_ARM_RELATIVE, "R_ARM_RELATIVE"},
        {R_ARM_GLOB_DAT, "R_ARM_GLOB_DAT"},
        {R_ARM_JUMP_SLOT, "R_ARM_JUMP_SLOT"},
        {R_386_PC32, "R_386_PC32"},
        {R_386_GOTPC, "R_386_GOTPC"},
        {R_X86_64_RELATIVE, "R_X86_64_RELATIVE"},
        {R_X86_64_GLOB_DAT, "R_X86_64_GLOB_DAT"},
        {R_X86_64_JUMP_SLOT, "R_X86_64_JUMP_SLOT"}
    };

    for (int i = 0; i < header.e_shnum; ++i) {
        if (sHeader[i].sh_type == SHT_RELA) {
            std::cout << "偏移量为0x" << std::hex << sHeader[i].sh_offset << "的重定位节" << std::string(&shstrtab[sHeader[i].sh_name]) << "包含" << std::dec << sHeader[i].sh_size / sizeof(Elf64_Rel) << "个条目" << std::endl;
            std::cout << "  偏移量          信息           类型           符号值        符号名称 + 加数" << std::endl;

            std::vector<Elf64_Rela> rel(sHeader[i].sh_size / sizeof(Elf64_Rela));
            file.seekg(sHeader[i].sh_offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(rel.data()), sHeader[i].sh_size);

            std::vector<Elf64_Sym> Sym(sHeader[sHeader[i].sh_link].sh_size / sHeader[sHeader[i].sh_link].sh_entsize);
            file.seekg(sHeader[sHeader[i].sh_link].sh_offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(Sym.data()), sHeader[sHeader[i].sh_link].sh_size);

            for (int j = 0; j < rel.size(); ++j) {
                Elf64_Addr r_offset = rel[j].r_offset;    
                Elf64_Word r_info = rel[j].r_info;      
                Elf64_Word r_sym = ELF64_R_SYM(r_info);
                Elf64_Word r_type = ELF64_R_TYPE(r_info);
                Elf64_Addr r_value = Sym[r_sym].st_value;
				Elf64_Word r_name = Sym[r_sym>Sym.size()?0:r_sym].st_name;
                if (get_dynstr_name(file, header, sHeader, r_name) == "") {
                    std::cout << std::right << std::hex << std::setw(12) << std::setfill('0') << r_offset << " ";
                    std::cout << std::right << std::setw(12) << std::setfill('0') << r_info << " ";
                    std::cout << relTypeMap[r_type] << " " << r_name << std::endl;
                } else {
                    std::cout << std::right << std::hex << std::setw(12) << std::setfill('0') << r_offset << " ";
                    std::cout << std::right << std::setw(12) << std::setfill('0') << r_info << " ";
                    std::cout << relTypeMap[r_type] << " " << r_name << std::endl;
                }
            }
        }
    }
}

void displayDynsym(std::ifstream &file,Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab) {
    for (int i = 0; i < header.e_shnum; ++i) {
        if (sHeader[i].sh_type == SHT_DYNSYM) {
            std::vector<Elf64_Sym> sym(sHeader[i].sh_size / sHeader[i].sh_entsize);
            file.seekg(sHeader[i].sh_offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(sym.data()), sHeader[i].sh_size);
            std::cout << "符号表'.dynsym'包含 " <<  std::dec << sHeader[i].sh_size / sHeader[i].sh_entsize <<"个条目:\n";
            std::cout << "   Num:    Value          Size    Type    Bind   Vis      Ndx Name" << std::endl;
            for (int j = 0; j < sym.size(); ++j) {
                Elf64_Word    st_name = sym[j].st_name;       /* name - index into string table */
                Elf64_Addr    st_value = sym[j].st_value;     /* symbol value */
                Elf64_Word    st_size = sym[j].st_size;       /* symbol size */
                unsigned char st_info = sym[j].st_info;       /* type and binding */
                unsigned char st_other = sym[j].st_other;     /* 0 - no defined meaning */
                Elf64_Half    st_shndx = sym[j].st_shndx;     /* section header index */

				unsigned char bind = ELF64_ST_BIND(st_info);
				unsigned char type = ELF64_ST_TYPE(st_info);
				std::string st_bind;
				std::string st_type;
				switch (bind)
				{
					case 0:
						st_bind = "LOCAL";
						break;
					case 1:
						st_bind = "GLOBAL";
						break;
					case 2:
						st_bind = "WEAK";
						break;
					case 3:
						st_bind = "NUM";
						break;
					case 4:
						st_bind = "LOOS";
						break;
					case 5:
						st_bind = "HIOS";
						break;
					case 6:
						st_bind = "LOPROC";
						break;
					case 7:
						st_bind = "HIPROC";
						break;
					default:
						st_bind = "OTHER";
				}
				switch (type)
				{
					case 0:
						st_type = "NOTYPE";
						break;
					case 1:
						st_type = "OBJECT";
						break;
					case 2:
						st_type = "FUNC";
						break;
					case 3:
						st_type = "SETION";
						break;
					case 4:
						st_type = "FILE";
						break;
					case 5:
						st_type = "NUM";
						break;
					case 6:
						st_type = "GNU_IFUNC";
						break;
					case 7:
						st_type = "LOOS";
						break;
					case 8:
						st_type = "HIOS";
						break;
					case 9:
						st_type = "LOPROC";
						break;
					case 10:
						st_type = "HIPROC";
						break;
					default:
						st_type = "OTHER";
				}
                std::cout << "     " <<  std::dec <<j << " " << std::setw(16) << std::setfill('0') 
                << std::hex << std::right << st_value << "   " << st_size << " "  << st_type << " " << st_bind 
                << " DEFAULT " << st_shndx << " " << get_dynstr_name(file, header, sHeader, st_name) << std::endl;

            }
        }
        if (sHeader[i].sh_type == SHT_SYMTAB) {
            std::vector<Elf64_Sym> sym(sHeader[i].sh_size / sHeader[i].sh_entsize);
            file.seekg(sHeader[i].sh_offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(sym.data()), sHeader[i].sh_size);
            std::cout << "符号表'.symtab'包含 " <<  std::dec << sHeader[i].sh_size / sHeader[i].sh_entsize <<"个条目:\n";
            std::cout << "   Num:    Value          Size    Type    Bind   Vis      Ndx Name" << std::endl;
            for (int j = 0; j < sym.size(); ++j) {
                Elf64_Word    st_name = sym[j].st_name;       /* name - index into string table */
                Elf64_Addr    st_value = sym[j].st_value;     /* symbol value */
                Elf64_Word    st_size = sym[j].st_size;       /* symbol size */
                unsigned char st_info = sym[j].st_info;       /* type and binding */
                unsigned char st_other = sym[j].st_other;     /* 0 - no defined meaning */
                Elf64_Half    st_shndx = sym[j].st_shndx;     /* section header index */

				unsigned char bind = ELF64_ST_BIND(st_info);
				unsigned char type = ELF64_ST_TYPE(st_info);
				std::string st_bind;
				std::string st_type;
				switch (bind)
				{
					case 0:
						st_bind = "LOCAL";
						break;
					case 1:
						st_bind = "GLOBAL";
						break;
					case 2:
						st_bind = "WEAK";
						break;
					case 3:
						st_bind = "NUM";
						break;
					case 4:
						st_bind = "LOOS";
						break;
					case 5:
						st_bind = "HIOS";
						break;
					case 6:
						st_bind = "LOPROC";
						break;
					case 7:
						st_bind = "HIPROC";
						break;
					default:
						st_bind = "OTHER";
				}
				switch (type)
				{
					case 0:
						st_type = "NOTYPE";
						break;
					case 1:
						st_type = "OBJECT";
						break;
					case 2:
						st_type = "FUNC";
						break;
					case 3:
						st_type = "SETION";
						break;
					case 4:
						st_type = "FILE";
						break;
					case 5:
						st_type = "NUM";
						break;
					case 6:
						st_type = "GNU_IFUNC";
						break;
					case 7:
						st_type = "LOOS";
						break;
					case 8:
						st_type = "HIOS";
						break;
					case 9:
						st_type = "LOPROC";
						break;
					case 10:
						st_type = "HIPROC";
						break;
					default:
						st_type = "OTHER";
				}
                std::cout << "     " << std::dec << j << " " << std::setw(16) << std::setfill('0') 
                << std::hex << std::right << st_value << "   " << st_size << " "  << st_type << " " << st_bind 
                << " DEFAULT " << st_shndx << " " << get_dynstr_name(file, header, sHeader, st_name) << std::endl;

            }
        }
    }
}
