mkdir build
cd build && gcc ../basic_pipeline.c -o basic_pipeline `pkg-config --cflags --libs gstreamer-1.0`