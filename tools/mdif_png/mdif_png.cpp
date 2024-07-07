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
#include <png.h>

#include "mdif.h"

int png_to_mdif(const char* png_filename, const char* mdif_filename) {
    FILE *png_file = fopen(png_filename, "rb");
    if(!png_file) {
        fprintf(stderr, "Error opening PNG file %s\n", png_filename);
        return 1;
    }

    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL
    );
    if(!png_ptr) {
        fclose(png_file);
        fprintf(stderr, "Error initializing PNG read structure\n");
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        fprintf(stderr, "Error initializing PNG info structure\n");
        return 1;
    }

    if(setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        fprintf(stderr, "Error during PNG file read\n");
        return 1;
    }

    png_init_io(png_ptr, png_file);
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    mdif_t mdif_image;
    mdif_init(&mdif_image, width, height);

    if(!mdif_image.red ||
        !mdif_image.green ||
        !mdif_image.blue ||
        !mdif_image.alpha
    ) {
        fprintf(stderr, "Error allocating memory for MDIF image\n");
        mdif_free(&mdif_image);

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);

        return 1;
    }

    png_bytep row_pointers[height];
    for(int y = 0; y < height; y++)
        row_pointers[y] = (png_byte *) malloc(
            png_get_rowbytes(png_ptr, info_ptr)
        );
    png_read_image(png_ptr, row_pointers);

    int index = 0;
    for(int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];

        for(int x = 0; x < width; x++) {
            mdif_image.red[index]   = row[x * 4];
            mdif_image.green[index] = row[x * 4 + 1];
            mdif_image.blue[index]  = row[x * 4 + 2];
            mdif_image.alpha[index] = row[x * 4 + 3];

            index++;
        }
    }

    for(int y = 0; y < height; y++)
        free(row_pointers[y]);

    mdif_error_t result = mdif_write(mdif_filename, &mdif_image);
    if(result != MDIF_ERROR_NONE) {
        fprintf(stderr, "Error: %s\r\n", mdif_error_message(result));
        return 1;
    }

    mdif_free(&mdif_image);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(png_file);

    if(!result) {
        fprintf(
            stderr,
            "Error writing MDIF file %s\n",
            mdif_filename
        );
        return 1;
    }

    return 0;
}

int mdif_to_png(const char* mdif_filename, const char* png_filename) {
    mdif_t mdif_image;

    mdif_error_t result = mdif_read(mdif_filename, &mdif_image);
    if(result != MDIF_ERROR_NONE) {
        fprintf(stderr, "Error: %s\r\n", mdif_error_message(result));
        return 1;
    }

    FILE *png_file = fopen(png_filename, "wb");
    if(!png_file) {
        fprintf(
            stderr,
            "Error opening PNG file %s\n",
            png_filename
        );

        mdif_free(&mdif_image);
        return 1;
    }

    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL
    );
    if(!png_ptr) {
        fclose(png_file);
        mdif_free(&mdif_image);

        fprintf(
            stderr,
            "Error initializing PNG write structure\n"
        );
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        fprintf(
            stderr,
            "Error initializing PNG info structure\n"
        );

        png_destroy_write_struct(&png_ptr, NULL);
        fclose(png_file);

        mdif_free(&mdif_image);
        return 1;
    }

    if(setjmp(png_jmpbuf(png_ptr))) {
        fprintf(
            stderr,
            "Error during PNG file write.\n"
        );

        png_destroy_write_struct(&png_ptr, &info_ptr);
        mdif_free(&mdif_image);

        fclose(png_file);
        return 1;
    }

    png_init_io(png_ptr, png_file);
    png_set_IHDR(
        png_ptr, info_ptr,
        mdif_image.width,
        mdif_image.height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_ptr, info_ptr);

    png_bytep row_pointers[mdif_image.height];
    for(int y = 0; y < mdif_image.height; y++) {
        row_pointers[y] = (png_byte *)
            malloc(4 * mdif_image.width * sizeof(png_byte));

        for(int x = 0; x < mdif_image.width; x++) {
            int index = y * mdif_image.width + x;

            row_pointers[y][x * 4]      = mdif_image.red[index];
            row_pointers[y][x * 4 + 1]  = mdif_image.green[index];
            row_pointers[y][x * 4 + 2]  = mdif_image.blue[index];
            row_pointers[y][x * 4 + 3]  = mdif_image.alpha[index];
        }
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    for(int y = 0; y < mdif_image.height; y++)
        free(row_pointers[y]);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(png_file);

    mdif_free(&mdif_image);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        return 1;
    }

    const char* infile = argv[1];
    const char* outfile = argv[2];

    int direction;
    if(strcmp(infile + strlen(infile) - 4, ".png") == 0 &&
        strcmp(outfile + strlen(outfile) - 5, ".mdif") == 0)
        direction = 0;
    else if(strcmp(infile + strlen(infile) - 5, ".mdif") == 0 &&
        strcmp(outfile + strlen(outfile) - 4, ".png") == 0)
        direction = 1;
    else {
        fprintf(stderr, "Invalid input and/or output file.\n");
        return 1;
    }

    int result;
    if(direction == 0)
        result = png_to_mdif(infile, outfile);
    else if(direction == 1)
        result = mdif_to_png(infile, outfile);
    else {
        fprintf(stderr, "Invalid input files specified.\n");
        return 1;
    }

    if(result != 0) {
        fprintf(stderr, "Conversion failed.\n");
        return 1;
    }

    printf("Conversion successful.\n");
    return 0;
}
