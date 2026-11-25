#include <stdio.h>
//This opens a file and checks if any error is there or not
int main(void)
{
    FILE *file = fopen("test.txt", "rb");
    if (file == NULL) {
        perror("Could not open test.txt");
        return 1;
    }
    unsigned long long count = 0;
    int byte = fgetc(file);
    while (byte != EOF) {
        if (count == 0) {
            printf("First byte: 0x%02X\n", (unsigned int)byte);
        }
        count++;
        byte = fgetc(file);
    }
    if (ferror(file)) {
        perror("Could not read test.txt");
        fclose(file);
        return 1;
    }
    printf("Total bytes: %llu\n", count);
    fclose(file);
    return 0;
}