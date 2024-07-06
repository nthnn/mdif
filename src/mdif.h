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

void mdif_init(mdif_t* image, short width, short height);
void mdif_free(mdif_t* image);

int mdif_read(const char* filename, mdif_t* image);
int mdif_write(const char* filename, mdif_t* image);

int mdif_grayscale(mdif_t* image, float* grayscale);

#endif