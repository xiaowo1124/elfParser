#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>

typedef uint32_t ELF32_Addr;
typedef uint16_t ELF32_Half;
typedef uint64_t ELF32_Off; 
typedef int32_t ELF32_SWord; 
typedef uint32_t ELF32_Word;

#define EI_NIDENT 16

typedef struct {
    unsigned char   e_ident[EI_NIDENT];
    ELF32_Half      e_type;
    ELF32_Half      e_machine;
    ELF32_Word      e_version;
    ELF32_Addr      e_entry;
    ELF32_Off       e_phoff;
    ELF32_Off       e_shoff;
    ELF32_Word      e_flags;
    ELF32_Half      e_ehsize;
    ELF32_Half      e_phentsize;
    ELF32_Half      e_phnum;
    ELF32_Half      e_shentsize;
    ELF32_Half      e_shnum;
    ELF32_Half      e_shstrndx;
} Elf32_Ehdr;

void readELFHeader(std::ifstream& file, Elf32_Ehdr& header);

void disPlayHeader(const Elf32_Ehdr& header);