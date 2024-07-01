#include "elf_header/elf_header.h"
#include "elf_sections/elf_sections.h"

int main(int argc, char* argv[]) {
    const char* filePath = "Test/hello1";
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "elfParser: 错误: '" << filePath <<"': 没有这个文件，请检查文件是否存在或者文件路径是否正确" <<  std::endl;
        return 0;
    }
    Elf64_Ehdr header;
    Elf64_Shdr sHeader;
    readELFHeader(file, header);
    disPlayHeader(header);
    readELFSecitons(file, header, sHeader);
    return 0;
}
