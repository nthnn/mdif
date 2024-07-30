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

#include <math.h>
#include <mdif.h>
#include <stdio.h>
#include <stdlib.h>

void draw_circle(mdif_t* image, int centerX, int centerY, int radius) {
    int width = image->width;
    int height = image->height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = x - centerX;
            int dy = y - centerY;

            if (dx * dx + dy * dy <= radius * radius) {
                image->red[y * width + x] = 255;
                image->green[y * width + x] = 255;
                image->blue[y * width + x] = 255;
                image->alpha[y * width + x] = 255;
            }
        }
    }
}

int main() {
    mdif_t image, aliased;
    mdif_init(&image, 256, 256);

    draw_circle(&image, 128, 128, 50);
    mdif_antialias(&image, &aliased);

    if(mdif_write("circle_image.mdif", &image) != MDIF_ERROR_NONE) {
        fprintf(stderr, "Failed to write MDIF image to file.\r\n");
        mdif_free(&image);
        return EXIT_FAILURE;
    }

    mdif_free(&image);
    mdif_free(&aliased);

    printf("MDIF image with circle has been created successfully.\n");
    return 0;
}
