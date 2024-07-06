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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int mdif_read(const char* filename, mdif_t* image) {
    FILE *file = fopen(filename, "rb");
    if(!file) return 0;

    if(fread(image->signature, sizeof(char), 2, file) != 2) {
        fclose(file);
        return 0;
    }

    if(strcmp(image->signature, "NT") != 0) {
        fclose(file);
        return 0;
    }

    if(fread(&image->width, sizeof(short), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    if(fread(&image->height, sizeof(short), 1, file) != 1) {
        fclose(file);
        return 0;
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
        return 0;
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
        return 0;
    }

    fclose(file);
    return 1;
}

int mdif_write(const char* filename, mdif_t* image) {
    FILE *file = fopen(filename, "wb");
    if(!file) return 0;

    if(fwrite(image->signature, sizeof(char), 2, file) != 2) {
        fclose(file);
        return 0;
    }

    if(fwrite(&image->width, sizeof(short), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    if(fwrite(&image->height, sizeof(short), 1, file) != 1) {
        fclose(file);
        return 0;
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
        return 0;
    }

    fclose(file);
    return 1;
}

int mdif_grayscale(mdif_t* image, float* grayscale) {
    if(!image || !grayscale)
        return 0;

    int pixel_count = image->width * image->height;
    for(int i = 0; i < pixel_count; i++) {
        float brightness =
            0.299 * (unsigned char) image->red[i] +
            0.587 * (unsigned char) image->green[i] +
            0.114 * (unsigned char) image->blue[i];

        grayscale[i] = brightness / 255.0;
    }

    return 1;
}