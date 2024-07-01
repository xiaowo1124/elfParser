#pragma once

#include "../elf_header/elf_header.h"

typedef struct{ 
    ELF32_Word sh_name;
    ELF32_Word sh_type;
    ELF32_Word sh_flags;
    ELF32_Addr sh_addr;
    ELF32_Off sh_offset; 
    ELF32_Word sh_size;
    ELF32_Word sh_link;
    ELF32_Word sh_info;
    ELF32_Word sh_addralign;
    ELF32_Word sh_entsize; 
}Elf32_Shdr;