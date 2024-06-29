#include "../elf_header/elf_header.h"

int main() {
    const char* filePath = "../test/hello1";
    readELFHeader(filePath);
    return 0;
}
