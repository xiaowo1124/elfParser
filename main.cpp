#include <getopt.h>
#include <cstring>

#include "elf_parser.h"
#include <string.h>
#include <vector>

using namespace std;

void show_help() {
    cout<< "用法:readelf <选项> elf-文件\n"
        << "  显示关于 ELF 格式文件内容的信息\n"
        << "  Options are:\n"
        << "  -a --all               Equivalent to: -h -l -S -s -r -d\n"
        << "  -h --file-header       Display the ELF file header\n"
        << "  -l --program-headers   Display the program headers\n"
        << "  --segments          An alias for --program-headers\n"
        << "  -S --section-headers   Display the sections' header\n"
        << "  --sections          An alias for --section-headers\n"
        << "  -e --headers           Equivalent to: -h -l -S\n"
        << "  -s --syms              Display the symbol table\n"
        << "  --symbols           An alias for --syms\n"
        << "  -d --dynamic           Display the dynamic section (if present)\n"
        << "  -D --use-dynamic       Use the dynamic section info when displaying symbols\n"
        << "  -H --help              Display this information\n";
}

int main(int argc, char* argv[]) {
    int opt;
    bool all = false;
    bool file_header = false;
    bool program_headers = false;
    bool sections_headers = false;
    bool relocs = false;
    bool headers = false;
    bool syms = false;
    bool dyn_syms = false;
    bool dynamic = false;
    bool help = false;
    struct option long_options [] = 
    {
        {"all", no_argument, nullptr, 'a'},
        {"file-header", no_argument, nullptr, 'h'},
        {"program-headers", no_argument, nullptr, 'l'},
        {"segments", no_argument, nullptr, 'l'},
        {"section-headers", no_argument, nullptr, 'S'},
        {"sections", no_argument, nullptr, 'S'},
        {"section-groups", no_argument, nullptr, 'g'},
        {"headers", no_argument, nullptr, 'e'},
        {"syms", no_argument, nullptr, 's'},
        {"symbols", no_argument, nullptr, 's'},
        {"relocs", no_argument, nullptr, 'r'},
        {"dynamic", no_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'H'},
        {nullptr, 0, nullptr, 0},
    };
    
    while((opt = getopt_long(argc, argv, "ahlSgtesnrudVAcDIH", long_options, nullptr)) != -1) {
        if (!opt) {
            if (strcmp(long_options[optind-1].name, "dyn-syms") == 0) {
                dyn_syms = true;
            }
        }
        switch (opt)
        {
        case 'a':
            file_header = true;
            program_headers = true;
            sections_headers = true;
            syms = true;
            dynamic = true;
            dyn_syms = true;
            relocs = true;
            break;
        case 'h':
            file_header = true;
            break;
        case 'l':
            program_headers = true;
            break;
        case 'S':
            sections_headers = true;
            break; 
        case 'e':
            file_header = true;
            program_headers = true;
            sections_headers = true;
            break;
        case 'r':
            relocs = true;
            break;
        case 's':
            syms = true;
            break;
        case 'H':
            help = true;
            break;
        default:
            help = true;
            break;
        }
    }

    if (help) {
        show_help();
        return 0;
    }

    if (optind >= argc) {
        cout << "elfparser: 错误: 无事可做\n";
        show_help();
        return 0;
    }
    if (optind + 1 == argc) {
        string filePath = argv[optind];
        Elf64_Ehdr header;
        ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "elfParser: 错误: '" << filePath <<"': 没有这个文件，请检查文件是否存在或者文件路径是否正确" <<  std::endl;
            return 0;
        }
        readELFHeader(file, header);
        std::vector<Elf64_Shdr> sHeader(header.e_shnum);
        std::vector<Elf64_Phdr> pHeader(header.e_phnum);
        readELFSecitons(file, header, sHeader);
        std::vector<char> shstrtab(sHeader[header.e_shstrndx].sh_size);
        readshstrtab(file, header, sHeader, shstrtab);
        readELFProgramHeader(file, header, pHeader);
        if (file_header) {
            disPlayHeader(header);
        }
        if (sections_headers) {
            disPlaySHeader(sHeader, shstrtab);
        }
        if (program_headers) {
            diplayProgramHeader(pHeader, file, sHeader, shstrtab);
        }
        if (dynamic) {
            displayDynamic(file, sHeader);
        }
        if (relocs) {
            displayRelocation(file, header, sHeader, shstrtab);
        }
        if (syms) {
            displayDynsym(file, header, sHeader, shstrtab);
        }
    } 
    return 0;
}
