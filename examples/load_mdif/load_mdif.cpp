#include <stdio.h>
#include "mdif.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <filename>\r\n", argv[0]);
        return -1;
    }

    mdif_t image;
    if(!mdif_read(argv[1], &image)) {
        printf("Error reading input file.\r\n");
        return -1;
    }

    printf("Image width:\t%d\r\n", image.width);
    printf("Image height:\t%d\r\n", image.height);

    return 0;
}