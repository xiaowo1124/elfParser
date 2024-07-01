#pragma once
#include <vector>

#include "../elf_header/elf_header.h"

void readELFSecitons(std::ifstream& file, Elf64_Ehdr& header, Elf64_Shdr& sHeader);

void disPlayHeader(std::vector<Elf64_Shdr> sectionHeaders,std::vector<char> shstrtab);

void printSectionHeader(const Elf64_Shdr& section, const std::string& name);