#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("yeah went wrong :( ");
        return 1;
    }
    //we writin the cmd 
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "ffmpeg -i \"%s\" -f mjpeg -", argv[1]);
    FILE *pipe = popen(cmd, "r");

    uint8_t buffer[4096];
    size_t byte_read;
    while((byte_read = fread(buffer, 1, sizeof(buffer), pipe)) > 0){
        printf("Read %zu bytes\n", byte_read);
    }
    pclose(pipe);
    return 0;
}