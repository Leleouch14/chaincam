hi i am testing this from another machine to check if ssh is working.


# Video to frames(mpeg)
    we use ffmpeg to slice the Video
    `ffmpeg -i sample.mp4 -f mjpeg - | /cprogram`
    here - means stdout 
    | feeds it to the cprogram's stdin

    so ffmpeg can not pipe the frames/mjpeg directly to the source code(ig m dumb).
    it pipes it to the complied executable 

    current we need to have the video and compiled file in the same directory(fuh naw) and then manually run the ffmpeg to stream it all in the buffer

    how do we input smt in fcking c
    we ofc make our function, compile our code, and then run the compiled file with wtv is next its an argument
    main(arg1, arg2) ---> cam.c
    therefore ./cam vid.mp4--> cam is compiled.
    vid.mp4 is arg1
    
# parser
    yea this was kinda um confusing to say the least
    so here we needed a buffer-safe frame boundary to ensure the mjpeg are sepereated 
    we needed to septerate them to ensure hashing later on 
    we did not want to use external lib like idk opencv so yea
    and jpegs alr come with magic number 0xd8 for start and 0xd9 for end and ofc 0xff for cmd so we used
    We implemented a prev_was_ff flag outside the loop to catch 0xFF 0xD8 (Start) and 0xFF 0xD9 (End) markers—even when a 2-byte marker is split right across the boundary edge of two separate 4 KB buffer reads.

# hashing
    now we r using public-domain single-file implementations of sha256 hash
    hash formula 
    $$\text{hash}i = \text{SHA256}(\text{hash}{i-1} \parallel \text{BE64}(\text{frame\_id}) \parallel \text{BE64}(\text{timestamp\_ns}) \parallel \text{frame\_bytes})$$

