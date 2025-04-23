A physically-based ray tracer written in C++

How to run:

1) Clone the files to a local folder
2) Download the necessary files from other repositories (Eigen, Happly, STB_IMAGE). You will likely need the entire eigen-3.4.0/ folder, but will only need the header files for Happly and STB_IMAGE. The files for Happly and STB_IMAGE go in the header/ folder. 
    -- Eigen: https://gitlab.com/libeigen/eigen
    -- Happly: https://github.com/nmwsharp/happly/blob/master/happly.h
    -- STB_IMAGE: https://github.com/nothings/stb/blob/master/stb_image.h
3) Modify the Makefile (make sure to change the location of the Eigen Library as needed by modifying the EIGEN variable)
4) Make and run
    -- Changes to some header-only files (e.g. constants.h) will require a full 'make clean' followed by a 'make' for changes to take effect. 
5) The output should show up in output/image.ppm