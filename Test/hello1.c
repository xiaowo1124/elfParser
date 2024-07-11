#include <stdio.h>

int my_data __attribute__((section(".my_section"), comdat)) = 42;

int main() {
    printf("my_data = %d\n", my_data);
    return 0;
}

