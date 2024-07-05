mkdir -p ../../dist
x86_64-w64-mingw32-gcc -Os -o ../../dist/mdif_viewer_win.exe -mwindows -I../../src ../../src/mdif.cpp mdif_viewer.cpp
