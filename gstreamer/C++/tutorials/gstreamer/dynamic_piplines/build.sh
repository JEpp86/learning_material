mkdir build
cd build && gcc ../dynamic_piplines.c -o dynamic_piplines `pkg-config --cflags --libs gstreamer-1.0`