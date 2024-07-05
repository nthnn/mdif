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
#include <jpeglib.h>

#include "mdif.h"

int jpg_to_mdif(const char* infile, const char* output_file) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPARRAY buffer;
    FILE* input_file;

    if((input_file = fopen(infile, "rb")) == NULL) {
        fprintf(stderr, "Can't open %s\n", infile);
        return 1;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, input_file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    buffer = (*cinfo.mem->alloc_sarray)(
        (j_common_ptr) &cinfo,
        JPOOL_IMAGE,
        cinfo.output_width * cinfo.output_components,
        1
    );

    mdif_t image;
    mdif_init(
        &image,
        cinfo.output_width,
        cinfo.output_height
    );

    while(cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for(unsigned int i = 0; i < cinfo.output_width; i++) {
            image.red[cinfo.output_scanline * cinfo.output_width + i]
                = buffer[0][i * 3];

            image.green[cinfo.output_scanline * cinfo.output_width + i]
                = buffer[0][i * 3 + 1];

            image.blue[cinfo.output_scanline * cinfo.output_width + i]
                = buffer[0][i * 3 + 2];

            image.alpha[cinfo.output_scanline * cinfo.output_width + i]
                = 255;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(input_file);

    int result = mdif_write(output_file, &image);
    mdif_free(&image);

    return !result;
}

int mdif_to_jpg(const char* input_file, const char* outfile) {
    mdif_t image;
    if(!mdif_read(input_file, &image)) {
        fprintf(stderr, "Can't read %s\n", input_file);
        return 1;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    FILE* output_file;

    if((output_file = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "Can't open %s\n", outfile);
        return 1;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, output_file);

    cinfo.image_width = image.width;
    cinfo.image_height = image.height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    unsigned char* row = (unsigned char*)malloc(image.width * 3);
    while(cinfo.next_scanline < cinfo.image_height) {
        for(int i = 0; i < image.width; i++) {
            row[i * 3]      =
                image.red[cinfo.next_scanline * image.width + i];

            row[i * 3 + 1]  =
                image.green[cinfo.next_scanline * image.width + i];

            row[i * 3 + 2]  =
                image.blue[cinfo.next_scanline * image.width + i];
        }

        row_pointer[0] = row;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(output_file);

    jpeg_destroy_compress(&cinfo);
    free(row);

    mdif_free(&image);
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return -1;
    }

    const char* infile = argv[1];
    const char* outfile = argv[2];

    int direction;
    if(strcmp(infile + strlen(infile) - 4, ".jpg") == 0 &&
        strcmp(outfile + strlen(outfile) - 5, ".mdif") == 0)
        direction = 0;
    else if(strcmp(infile + strlen(infile) - 5, ".mdif") == 0 &&
        strcmp(outfile + strlen(outfile) - 4, ".jpg") == 0)
        direction = 1;
    else {
        fprintf(stderr, "Invalid input and/or output file.\n");
        return 1;
    }

    int result;
    if(direction == 0)
        result = jpg_to_mdif(infile, outfile);
    else if(direction == 1)
        result = mdif_to_jpg(infile, outfile);
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
