#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("yeah went wrong :( ");
        return 1;
    }
    //parser state variables: gotta be everywhere(global)
    int prev_was_ff = 0;
    int in_frame = 0;
    unsigned long frame_count = 0;

    //we writin the cmd 
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "ffmpeg -i \"%s\" -f mjpeg -", argv[1]);
    FILE *pipe = popen(cmd, "r");

    uint8_t buffer[4096];
    size_t byte_read;
    while((byte_read = fread(buffer, 1, sizeof(buffer), pipe)) > 0){
        for (size_t i = 0; i < byte_read; i++) {
            uint8_t current_byte = buffer[i];

            //start of img
            if(prev_was_ff && current_byte == 0xD8){
                in_frame = 1;
                frame_count++;
                // printf("Frame %lu started\n", frame_count);
            }
            if(prev_was_ff && current_byte == 0xD9){
                in_frame = 0;
                //printf("frame %lu ended\n", frame_count);
            }
            prev_was_ff = (current_byte == 0xFF); 
            //this is a single line if-else statement. if true prev_was_ff = 1 else 0
        }
    }
    pclose(pipe);
    printf("Total frames detected: %lu\n", frame_count);
    return 0;
}