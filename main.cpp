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
        << "  -a --all               Equivalent to: -h -l -S -s -r -d -V -A -I\n"
        << "  -h --file-header       Display the ELF file header\n"
        << "  -l --program-headers   Display the program headers\n"
        << "  --segments          An alias for --program-headers\n"
        << "  -S --section-headers   Display the sections' header\n"
        << "  --sections          An alias for --section-headers\n"
        << "  -g --section-groups    Display the section groups\n"
        << "  -t --section-details   Display the section details\n"
        << "  -e --headers           Equivalent to: -h -l -S\n"
        << "  -s --syms              Display the symbol table\n"
        << "  --symbols           An alias for --syms\n"
        << "  --dyn-syms             Display the dynamic symbol table\n"
        << "  -n --notes             Display the core notes (if present)\n"
        << "  -r --relocs            Display the relocations (if present)\n"
        << "  -u --unwind            Display the unwind info (if present)\n"
        << "  -d --dynamic           Display the dynamic section (if present)\n"
        << "  -V --version-info      Display the version sections (if present)\n"
        << "  -A --arch-specific     Display architecture specific information (if any)\n"
        << "  -c --archive-index     Display the symbol/file index in an archive\n"
        << "  -D --use-dynamic       Use the dynamic section info when displaying symbols\n"
        << "  -x --hex-dump=<number|name>\n"
        << "                         Dump the contents of section <number|name> as bytes\n"
        << "  -p --string-dump=<number|name>\n"
        << "                         Dump the contents of section <number|name> as strings\n"
        << "  -R --relocated-dump=<number|name>\n"     
        << "                         Dump the contents of section <number|name> as relocated bytes\n"
        << "  -z --decompress        Decompress section before dumping it\n"             
        << "  -w[lLiaprmfFsoRtUuTgAckK] or\n"
        << "  --debug-dump[=rawline,=decodedline,=info,=abbrev,=pubnames,=aranges,=macro,=frames,\n"
        << "               =frames-interp,=str,=loc,=Ranges,=pubtypes,\n"
        << "               =gdb_index,=trace_info,=trace_abbrev,=trace_aranges,\n"
        << "               =addr,=cu_index,=links,=follow-links]\n"     
        << "                         Display the contents of DWARF debug sections\n"    
        << "  --dwarf-depth=N        Do not display DIEs at depth N or greater\n"                 
        << "  --dwarf-start=N        Display DIEs starting with N, at the same depth or deeper\n"
        << "  --ctf=<number|name>    Display CTF info from section <number|name>\n"
        << "  --ctf-parent=<number|name>\n"               
        << "                         Use section <number|name> as the CTF parent\n"
        << "  --ctf-symbols=<number|name>\n"
        << "                         Use section <number|name> as the CTF external symtab\n"   
        << "  --ctf-strings=<number|name>\n"
        << "                         Use section <number|name> as the CTF external strtab\n"
        << "  -I --histogram         Display histogram of bucket list lengths\n"               
        << "  -W --wide              Allow output width to exceed 80 characters\n"
        << "  @<file>                Read options from <file>\n"
        << "  -H --help              Display this information\n"                
        << "  -v --version           Display the version number of readelf\n";
}

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
            relocs = true;
            dynamic = true;
            version_info = true;
            arch_specific = true;
            histogram = true;
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
        case 'g':
            section_groups = true;
            break;  
        case 't':
            section_details = true;
            break; 
        case 'e':
            file_header = true;
            program_headers = true;
            sections_headers = true;
            break;
        case 's':
            syms = true;
            break;
        case 'n':
            notes = true;
            break;
        case 'r':
            relocs = true;
            break;
        case 'u':
            unwind = true;
            break;
        case 'd':
            dynamic = true;
            break;
        case 'V':
            version_info = true;
            break;
        case 'A':
            arch_specific = true;
            break;
        case 'c':
            archive_index = true;
            break;
        case 'D':
            use_dynamic = true;
            break;
        case 'I':
            histogram = true;
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
        disPlayHeader(header);
        
    } else {
        for (int i = optind; i < argc; ++i) {
            string filepath = argv[optind];
            cout << "文件:" << filepath << endl;
            Elf32_Ehdr header;
        }
    }
    return 0;
}
