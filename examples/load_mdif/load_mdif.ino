#ifndef ARDUINO
#   include <stdio.h>
#else
#   include <SD.h>
#endif

#include <mdif.h>

#ifdef ARDUINO

void setup() {
    Serial.begin(115200);

    if(!SD.begin(10)) {
        Serial.println("SD card initialization failed!");
        while(true);
    }

    mdif_t image;
    mdif_error_t result = mdif_read("/image.mdif", &image);
    if(result != MDIF_ERROR_NONE) {
        Serial.print("Error: ");
        Serial.println(mdif_error_message(result));
        while(true);
    }

    Serial.print("Image width:\t");
    Serial.println(image.width);
    Serial.print("Image height:\t");
    Serial.println(image.height);
}

void loop() {
    delay(1000);
}

#else

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <filename>\r\n", argv[0]);
        return -1;
    }

    mdif_t image;
    mdif_error_t result = mdif_read(argv[1], &image);
    if(result != MDIF_ERROR_NONE) {
        printf("Error: %s\r\n", mdif_error_message(result));
        return -1;
    }

    printf("Image width:\t%d\r\n", image.width);
    printf("Image height:\t%d\r\n", image.height);

    return 0;
}

#endif