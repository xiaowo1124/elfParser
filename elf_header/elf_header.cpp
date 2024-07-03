#include "elf_header.h"
#include <iomanip>


void readELFHeader(std::ifstream& file, Elf64_Ehdr& header) {
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
}

void disPlayHeader(const Elf64_Ehdr& header)
{
    if ((header.e_ident[0] ^ 127) || (header.e_ident[1] ^ 69) || (header.e_ident[2] ^ 76) || (header.e_ident[3] ^ 70)) {
        std::cout<<"这不是一个elf文件,"<<std::endl;
        exit(0);
    }
    // Print ELF header information for demonstration
    std::cout << "ELF头: "<<std::endl;
    std::cout << "  Magic: ";
    for (int i = 0; i < 16; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(header.e_ident[i]) << " ";
    }
    std::cout << std::endl;

    if (header.e_ident[4] == 0) {
        std::cout<< "  类别: 非法类别" << std::endl;
        std::cout<< "该文件为无效类别或未指定类别,可能不是ELF文件或文件内容已被破坏,已停止解析" << std::endl;
        exit(1);
    } else if (header.e_ident[4] == 1) {
        std::cout << "  类别: ELF32" << std::endl;
    } else if (header.e_ident[4] == 2) {
        std::cout << "  类别: ELF64" << std::endl;
    }

    if (header.e_ident[5] == 1) {
        std::cout << "  编码方式: 2进制补码,小端序" << std::endl;
    } else if (header.e_ident[5] == 2) {
        std::cout << "  编码方式: 2进制补码,大端序" << std::endl;
    } else {
        std::cout << "  编码方式: 非法数据编码" << std::endl;
        std::cout << "该文件数据编码方式非法,可能不是ELF文件或文件内容已被破坏,已停止解析" << std::endl;
        exit(1);
    }

    std::cout << "  版本: " << header.e_ident[6] << "(当前)" << std::endl;

    if (header.e_ident[7] == 0) {
        std::cout << "  OS/ABI: UNIX - System V" << std::endl;
    } else if (header.e_ident[7] == 3) {
        std::cout << "  OS/ABI: Linux" << std::endl;
    } else {
        std::cout << "  OS/ABI: 未知" << std::endl;
    }

    std::cout << "  ABI 版本: " << std::dec << static_cast<int>(header.e_ident[8]) << std::endl;

    std::cout << "  类型: ";
    switch (header.e_type)
    {
    case 0:
        std::cout << "未知目标文件格式" << std::endl;
        break;
    case 1:
        std::cout << "可重定位文件" << std::endl;
        break;
    case 2:
        std::cout << "可执行文件" << std::endl;
        break;
    case 3:
        std::cout << "共享目标文件" << std::endl;
        break;
    case 4:
        std::cout << "Core文件(转储格式)" << std::endl;
        break;
    case 0xff00:
        std::cout << "特定处理器文件" << std::endl;
        break; 
    case 0xffff:
        std::cout << "特定处理器文件" << std::endl;
        break;                               
    default:
        break;
    }
    std::cout << "  系统架构: ";
    switch (header.e_machine)
    {
    case 0:
        std::cout << "未指定"<< std::endl;
        break;
    case 1:
        std::cout << "AT&T WE 32100"<< std::endl;
        break;
    case 2:
        std::cout << "SPARC"<< std::endl;
        break;
    case 3:
        std::cout << "Intel 80386"<< std::endl;
        break;
    case 4:
        std::cout << "Motorola 68000"<< std::endl;
        break;
    case 5:
        std::cout << "Motorola 88000"<< std::endl;
        break;
    case 7:
        std::cout << "Intel 80860"<< std::endl;
        break;
    case 8:
        std::cout << "MIPS RS3000"<< std::endl;
        break;
    case 0x3e:
        std::cout << "Advanced Micro Devices X86-64" << std::endl;
        break;      
    default:
        break;
    }
    std::cout << "  版本: " << header.e_version << std::endl;
    std::cout << "  入口点地址: " << "0x" << std::hex << header.e_entry << std::endl;
    std::cout << "  程序头起点: " << std::dec << header.e_phoff << " (bytes into file)" << std::endl;
    std::cout << "  节头起点: " << std::dec << header.e_shoff << " (bytes into file)" << std::endl;
    std::cout << "  标志: " << "0x" << header.e_flags << std::endl;
    std::cout << "  ELF头大小: " << std::dec << header.e_ehsize << " (bytes)" << std::endl;
    std::cout << "  程序头大小: " << std::dec << header.e_phentsize << " (bytes)" << std::endl;
    std::cout << "  程序头数量: " << std::dec << header.e_phnum << std::endl;
    std::cout << "  节头大小: " << std::dec << header.e_shentsize << " (bytes)" << std::endl;
    std::cout << "  节头数量: " << std::dec << header.e_shnum << std::endl;
    std::cout << "  节头字符串表索引: " << std::dec << header.e_shstrndx << std::endl;
    std::cout << std::endl;
}
