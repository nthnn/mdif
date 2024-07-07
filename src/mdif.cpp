/* 
 * Copyright 2024 Nathanne Isip
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef ARDUINO
#   include <SD.h>
#   include <SPI.h>
#else
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "mdif.h"

void mdif_init(mdif_t* image, short width, short height) {
    image->signature[0] = 'N';
    image->signature[1] = 'T';

    image->width = width;
    image->height = height;

    int pixel_count = width * height;
    image->red   = (unsigned char*) malloc(pixel_count);
    image->green = (unsigned char*) malloc(pixel_count);
    image->blue  = (unsigned char*) malloc(pixel_count);
    image->alpha = (unsigned char*) malloc(pixel_count);
}

void mdif_free(mdif_t* image) {
    free(image->red);
    free(image->green);
    free(image->blue);
    free(image->alpha);
}

mdif_error_t mdif_read(const char* filename, mdif_t* image) {
    #ifndef ARDUINO

    FILE *file = fopen(filename, "rb");
    if(!file)
        return MDIF_ERROR_INVALID_FILE_HANDLE;

    if(fread(image->signature, sizeof(char), 2, file) != 2) {
        fclose(file);
        return MDIF_ERROR_READ;
    }

    if(strcmp(image->signature, "NT") != 0) {
        fclose(file);
        return MDIF_ERROR_INVALID_SIGNATURE;
    }

    if(fread(&image->width, sizeof(short), 1, file) != 1) {
        fclose(file);
        return MDIF_ERROR_READ;
    }

    if(fread(&image->height, sizeof(short), 1, file) != 1) {
        fclose(file);
        return MDIF_ERROR_READ;
    }

    if(image->width < 1 || image->width > 1024) {
        fclose(file);
        return MDIF_ERROR_INVALID_WIDTH;
    }

    if(image->height < 1 || image->height > 1024) {
        fclose(file);
        return MDIF_ERROR_INVALID_HEIGHT;
    }

    size_t pixel_count = image->width * image->height;
    image->red      = (unsigned char*) malloc(pixel_count);
    image->blue     = (unsigned char*) malloc(pixel_count);
    image->green    = (unsigned char*) malloc(pixel_count);
    image->alpha    = (unsigned char*) malloc(pixel_count);

    if(!image->red ||
        !image->blue ||
        !image->green ||
        !image->alpha
    ) {
        fclose(file);
        return MDIF_ERROR_CANNOT_ALLOCATE;
    }

    if(fread(image->red,    sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fread(image->blue,  sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fread(image->green, sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fread(image->alpha, sizeof(unsigned char), pixel_count, file)
            != pixel_count
    ) {
        fclose(file);
        return MDIF_ERROR_READ;
    }

    fclose(file);

    #else

    File file = SD.open(filename, FILE_READ);
    if(!file)
        return MDIF_ERROR_INVALID_FILE_HANDLE;

    if(file.read((uint8_t*) image->signature, 2) != 2) {
        file.close();
        return MDIF_ERROR_READ;
    }

    if(strncmp(image->signature, "NT", 2) != 0) {
        file.close();
        return MDIF_ERROR_INVALID_SIGNATURE;
    }

    if(file.read((unsigned char*) &image->width, sizeof(short))
        != sizeof(short)) {
        file.close();
        return MDIF_ERROR_READ;
    }

    if(file.read((unsigned char*) &image->height, sizeof(short))
        != sizeof(short)) {
        file.close();
        return MDIF_ERROR_READ;
    }

    if(image->width < 1 || image->width > 1024) {
        file.close();
        return MDIF_ERROR_INVALID_WIDTH;
    }

    if(image->height < 1 || image->height > 1024) {
        file.close();
        return MDIF_ERROR_INVALID_HEIGHT;
    }

    size_t pixel_count = image->width * image->height;
    image->red   = (unsigned char*) malloc(pixel_count);
    image->green = (unsigned char*) malloc(pixel_count);
    image->blue  = (unsigned char*) malloc(pixel_count);
    image->alpha = (unsigned char*) malloc(pixel_count);

    if(!image->red      ||
        !image->green   ||
        !image->blue    ||
        !image->alpha) {
        file.close();
        return MDIF_ERROR_CANNOT_ALLOCATE;
    }

    if(file.read(image->red, pixel_count)    != pixel_count ||
        file.read(image->green, pixel_count) != pixel_count ||
        file.read(image->blue, pixel_count)  != pixel_count ||
        file.read(image->alpha, pixel_count) != pixel_count) {
        file.close();
        return MDIF_ERROR_READ;
    }

    file.close();

    #endif

    return MDIF_ERROR_NONE;
}

mdif_error_t mdif_write(const char* filename, mdif_t* image) {
    #ifndef ARDUINO

    if(image->width < 1 || image->width > 1024)
        return MDIF_ERROR_INVALID_WIDTH;

    if(image->height < 1 || image->height > 1024)
        return MDIF_ERROR_INVALID_HEIGHT;

    FILE *file = fopen(filename, "wb");
    if(!file)
        return MDIF_ERROR_INVALID_FILE_HANDLE;

    if(fwrite(image->signature, sizeof(char), 2, file) != 2) {
        fclose(file);
        return MDIF_ERROR_WRITE;
    }

    if(fwrite(&image->width, sizeof(short), 1, file) != 1) {
        fclose(file);
        return MDIF_ERROR_WRITE;
    }

    if(fwrite(&image->height, sizeof(short), 1, file) != 1) {
        fclose(file);
        return MDIF_ERROR_WRITE;
    }

    size_t pixel_count = image->width * image->height;
    if(fwrite(image->red,    sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fwrite(image->blue,  sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fwrite(image->green, sizeof(unsigned char), pixel_count, file)
            != pixel_count ||
        fwrite(image->alpha, sizeof(unsigned char), pixel_count, file)
            != pixel_count) {
        fclose(file);
        return MDIF_ERROR_WRITE;
    }

    fclose(file);

    #else

    if(image->width < 1 || image->width > 1024)
        return MDIF_ERROR_INVALID_WIDTH;

    if(image->height < 1 || image->height > 1024)
        return MDIF_ERROR_INVALID_HEIGHT;

    File file = SD.open(filename, FILE_WRITE);
    if(!file)
        return MDIF_ERROR_INVALID_FILE_HANDLE;

    if(file.write(image->signature, 2) != 2) {
        file.close();
        return MDIF_ERROR_WRITE;
    }

    if(file.write((char*)&image->width, sizeof(short)) != sizeof(short)) {
        file.close();
        return MDIF_ERROR_WRITE;
    }

    if(file.write((char*)&image->height, sizeof(short)) != sizeof(short)) {
        file.close();
        return MDIF_ERROR_WRITE;
    }

    size_t pixel_count = image->width * image->height;
    if(file.write(image->red, pixel_count)    != pixel_count ||
        file.write(image->green, pixel_count) != pixel_count ||
        file.write(image->blue, pixel_count)  != pixel_count ||
        file.write(image->alpha, pixel_count) != pixel_count) {
        file.close();
        return MDIF_ERROR_WRITE;
    }

    file.close();

    #endif

    return MDIF_ERROR_NONE;
}

mdif_error_t mdif_grayscale(mdif_t* image, float* grayscale) {
    if(!image)
        return MDIF_ERROR_IMAGE;

    if(!grayscale)
        return MDIF_ERROR_GRAYSCALE;

    int pixel_count = image->width * image->height;
    for(int i = 0; i < pixel_count; i++) {
        float brightness =
            0.299 * (unsigned char) image->red[i] +
            0.587 * (unsigned char) image->green[i] +
            0.114 * (unsigned char) image->blue[i];

        grayscale[i] = brightness / 255.0;
    }

    return MDIF_ERROR_NONE;
}

const char* mdif_error_message(mdif_error_t error_num) {
    switch(error_num) {
        case MDIF_ERROR_IO:
            return "I/O error";

        case MDIF_ERROR_NONE:
            return "None";

        case MDIF_ERROR_INVALID_SIGNATURE:
            return "Invalid MDIF signature";

        case MDIF_ERROR_CANNOT_ALLOCATE:
            return "Canot allocate memory for channels";

        case MDIF_ERROR_INVALID_WIDTH:
            return "Invalid image width (> 1024)";

        case MDIF_ERROR_INVALID_HEIGHT:
            return "Invalid image height (> 1024)";

        case MDIF_ERROR_READ:
            return "Error reading MDIF file";

        case MDIF_ERROR_WRITE:
            return "Error writing MDIF file";

        case MDIF_ERROR_INVALID_FILE_HANDLE:
            return "Invalid file handle";

        case MDIF_ERROR_IMAGE:
            return "Image error";

        case MDIF_ERROR_GRAYSCALE:
            return "Invalid grayscale pointer";
    }

    return "Unknown error";
}