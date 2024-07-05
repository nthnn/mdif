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

void create_gradient_image(mdif_t* image, int width, int height) {
    mdif_init(image, width, height);
    if(!image->red ||
        !image->blue ||
        !image->green ||
        !image->alpha
    ) {
        perror("Failed to allocate memory for image channels.");
        exit(EXIT_FAILURE);
    }

    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++) {
            size_t index = y * width + x;

            image->red[index]   = (char) ((255 * (width - 1 - x)) / (width - 1));
            image->green[index] = 0;
            image->blue[index]  = (char) ((255 * x) / (width - 1));
            image->alpha[index] = (char) 255;
        }
}

void print_grayscale_table(const float* grayscale, int len) {
    int x = 0;
    for(int i = 0; i < len; i++) {
        if(grayscale[i] >= 0.0)
            printf(" ");
        printf("%.2f\t", grayscale[i]);

        x++;
        if(x == 5) {
            printf("\r\n");
            x = 0;
        }
    }
}

int main() {
    mdif_t image;

    int width = 5, height = 5;
    create_gradient_image(&image, width, height);

    float* grayscale = (float*)malloc(width * height * sizeof(float));
    if(!grayscale) {
        perror("Failed to allocate memory for grayscale.");
        mdif_free(&image);
        return -1;
    }

    if(mdif_grayscale(&image, grayscale))
        print_grayscale_table(grayscale, width * height);
    else fprintf(stderr, "Failed to convert image to grayscale.\n");

    free(grayscale);
    mdif_free(&image);

    return 0;
}