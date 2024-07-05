mkdir -p ../../dist
gcc -o ../../dist/mdif_jpg mdif_jpg.cpp ../../src/mdif.cpp -ljpeg -I../../src