#include <getopt.h>
#include <cstring>

#include "elf_parser.h"
#include <string.h>

using namespace std;

int main(int argc, char* argv[]) {
    int opt;
    bool all = false;
    bool file_header = false;
    bool program_headers = false;
    bool sections_headers = false;
    bool section_groups = false;
    bool section_details = false;
    bool headers = false;
    bool syms = false;
    bool dyn_syms = false;
    bool notes = false;
    bool relocs = false;
    bool unwind = false;
    bool dynamic = false;
    bool version_info = false;
    bool arch_specific = false;
    bool archive_index = false;
    bool use_dynamic = false;
    bool histogram = false;
    struct option long_options [] = 
    {
        {"all", no_argument, nullptr, 'a'},
        {"file-header", no_argument, nullptr, 'h'},
        {"program-headers", no_argument, nullptr, 'l'},
        {"segments", no_argument, nullptr, 'l'},
        {"section-headers", no_argument, nullptr, 'S'},
        {"sections", no_argument, nullptr, 'S'},
        {"section-groups", no_argument, nullptr, 'g'},
        {"section-details", no_argument, nullptr, 't'},
        {"headers", no_argument, nullptr, 'e'},
        {"syms", no_argument, nullptr, 's'},
        {"symbols", no_argument, nullptr, 's'},
        {"dyn-syms", no_argument, nullptr, 0},
        {"notes", no_argument, nullptr, 'n'},
        {"relocs", no_argument, nullptr, 'r'},
        {"unwind", no_argument, nullptr, 'u'},
        {"dynamic", no_argument, nullptr, 'd'},
        {"version-info", no_argument, nullptr, 'V'},
        {"arch-specific", no_argument, nullptr, 'A'},
        {"archive-index", no_argument, nullptr, 'c'},
        {"use-dynamic", no_argument, nullptr, 'D'},
        {"histogram", no_argument, nullptr, 'I'},
        {nullptr, 0, nullptr, 0},
    };
    
    while((opt = getopt_long(argc, argv, "ahlSgtesnrudVAcD", long_options, nullptr)) != -1) {
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
            relocs = true;
            dynamic = true;
            version_info = true;
            arch_specific = true;
            histogram = true;
            break;
        default:
            break;
        }
    }

    Elf32_Ehdr header;


    return 0;
}

void show_help() {
    cout << "用法: " << "elfparser" << " <options> elf-文件\n"
              << "Options:\n"
              << "  -a  --all       Parse entire ELF file\n"
              << "  -h  --header    Parse ELF header\n"
              << "  -l  --programs  List all program headers\n"
              << "  -S  --sections  List all section headers\n"
              << "  -H  --Help      Display this infomation\n";

                
}