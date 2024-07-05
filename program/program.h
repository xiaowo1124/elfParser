#pragma once

#include <vector>
#include <iomanip>
#include <map>

#include "../elf_header/elf_header.h"

void readELFProgramHeader(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Phdr> &pHeader);

void diplayProgramHeader(std::vector<Elf64_Phdr> &pHeader, std::ifstream &file);
