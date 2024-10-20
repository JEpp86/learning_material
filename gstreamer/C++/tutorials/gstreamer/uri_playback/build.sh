mkdir build
cd build && gcc ../uri_playback.c -o uri_playback `pkg-config --cflags --libs gstreamer-1.0`