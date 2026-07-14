#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <elf.h>

void readELFHeader(std::ifstream &file, Elf64_Ehdr& header);

void disPlayHeader(const Elf64_Ehdr& header);