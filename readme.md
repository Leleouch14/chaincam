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
    