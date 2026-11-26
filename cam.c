#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lib/sha256.h"

typedef struct __attribute((packed)) {
  uint64_t frame_id;
    uint32_t length;
    uint8_t hash[32];
} Framerecord;


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

    FILE *footage = fopen("footage.mjpeg", "wb");
    FILE *records = fopen("records.bin", "wb");
    
    //genesis hash -> origin -> first hash
    uint8_t prev_hash[32];
    memset(prev_hash, 0, 32);

    //frame accumulation thingy
    size_t max_frame_size = 2 * 1024 * 1024;
    uint8_t *frame_buffer = malloc(max_frame_size);
    size_t frame_len = 0;

    uint8_t buffer[4096];
    size_t byte_read;
    while((byte_read = fread(buffer, 1, sizeof(buffer), pipe)) > 0){
        for (size_t i = 0; i < byte_read; i++) {
            uint8_t current_byte = buffer[i];

            //start of img
            if(prev_was_ff && current_byte == 0xD8){
                in_frame = 1;
                frame_len = 0;
                frame_buffer[frame_len++] = 0xFF;
                frame_buffer[frame_len++] = 0xD8;
                // printf("Frame %lu started\n", frame_count);
            }
            //accumulate frame
            else if (in_frame){
                if (frame_len < max_frame_size){
                    frame_buffer[frame_len++] = current_byte;
                }
            }

            if(prev_was_ff && current_byte == 0xD9){
                in_frame = 0;
                //read hashing pls i copied ts
                // --- HASH CHAINING ---
                SHA256_CTX ctx;
                sha256_init(&ctx);
                    // Feed previous hash
                sha256_update(&ctx, prev_hash, 32);
                    // Feed frame ID
                sha256_update(&ctx, (uint8_t *)&frame_count, sizeof(frame_count));
                    // Feed raw frame bytes from our bucket
                sha256_update(&ctx, frame_buffer, frame_len);
                uint8_t current_hash[32];
                sha256_final(&ctx, current_hash);
                    // Write raw frame to footage.mjpeg
                fwrite(frame_buffer, 1, frame_len, footage);
                    // Write record to records.bin using standard fwrite
                Framerecord rec;
                rec.frame_id = frame_count;
                rec.length = (uint32_t)frame_len;
                memcpy(rec.hash, current_hash, 32);
                fwrite(&rec, sizeof(Framerecord), 1, records);
                    // Update prev_hash for the next frame's chain
                memcpy(prev_hash, current_hash, 32);
                frame_count++;
                //printf("frame %lu ended\n", frame_count);
            }
            prev_was_ff = (current_byte == 0xFF); 
            //this is a single line if-else statement. if true prev_was_ff = 1 else 0
        }
    }
    free(frame_buffer); //hmm i wonder how much time i spent here
    fclose(footage);
    fclose(records);
    pclose(pipe);
    printf("Total frames detected: %lu\n", frame_count);
    return 0;
}