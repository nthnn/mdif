mkdir -p ../../dist
gcc -o ../../dist/mdif_png mdif_png.cpp ../../src/mdif.cpp -lpng -I../../src