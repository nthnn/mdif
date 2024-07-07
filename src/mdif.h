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

#ifndef MDIF_H
#define MDIF_H

typedef struct mdif_struct {
    char signature[2];

    short width;
    short height;

    unsigned char *red;
    unsigned char *blue;
    unsigned char *green;
    unsigned char *alpha;
} mdif_t;

typedef enum mdif_error {
    MDIF_ERROR_IO,
    MDIF_ERROR_NONE,
    MDIF_ERROR_INVALID_SIGNATURE,
    MDIF_ERROR_CANNOT_ALLOCATE,
    MDIF_ERROR_INVALID_WIDTH,
    MDIF_ERROR_INVALID_HEIGHT,
    MDIF_ERROR_READ,
    MDIF_ERROR_WRITE,
    MDIF_ERROR_INVALID_FILE_HANDLE,
    MDIF_ERROR_IMAGE,
    MDIF_ERROR_GRAYSCALE
} mdif_error_t;

void mdif_init(mdif_t* image, short width, short height);
void mdif_free(mdif_t* image);

mdif_error_t mdif_read(const char* filename, mdif_t* image);
mdif_error_t mdif_write(const char* filename, mdif_t* image);

mdif_error_t mdif_grayscale(mdif_t* image, float* grayscale);

const char* mdif_error_message(mdif_error_t error_num);

#endif