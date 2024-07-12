#pragma once

#include <vector>
#include <iomanip>
#include <map>

#include "../elf_header/elf_header.h"

void readELFSecitons(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader);

void readshstrtab(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab);

void disPlaySHeader(std::vector<Elf64_Shdr> &sectionHeaders, std::vector<char> &shstrtab);

std::string getFlag(Elf64_Xword flags);

void displayDynamic(std::ifstream &file, std::vector<Elf64_Shdr> &sHeader);

std::string get_dynstr_name(std::ifstream &file,Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, Elf64_Word dynstrndx);

void displayRelocation(std::ifstream &file, Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab);

void displayDynsym(std::ifstream &file,Elf64_Ehdr &header, std::vector<Elf64_Shdr> &sHeader, std::vector<char> &shstrtab);

