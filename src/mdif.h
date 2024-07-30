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

/**
 * @file mdif.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Minimal Data Image Format (MDIF) Header File
 * 
 * This file contains the data structures and function prototypes for working with the MDIF format.
 * The MDIF format is a lightweight, simple image format designed for use with both desktop environments
 * and microcontroller environments such as the Raspberry Pi Pico and ESP32 family.
 * It supports basic image functionalities such as reading, writing, and converting to grayscale.
 * 
 */

#ifndef MDIF_H
#define MDIF_H

/**
 * @brief MDIF image structure.
 * 
 * This structure represents an image in the MDIF format. It contains the image signature,
 * width, height, and separate color channels for red, green, blue, and alpha.
 */
typedef struct mdif_struct {
    char signature[2];         /**< Signature to identify the file as an MDIF image. */

    short width;               /**< Width of the image. */
    short height;              /**< Height of the image. */

    unsigned char *red;        /**< Pointer to the red channel data. */
    unsigned char *blue;       /**< Pointer to the blue channel data. */
    unsigned char *green;      /**< Pointer to the green channel data. */
    unsigned char *alpha;      /**< Pointer to the alpha channel data. */
} mdif_t;

/**
 * @brief MDIF error codes.
 * 
 * This enumeration defines the possible error codes that can be returned by MDIF functions.
 */
typedef enum mdif_error {
    MDIF_ERROR_IO,                /**< General I/O error. */
    MDIF_ERROR_NONE,              /**< No error. */
    MDIF_ERROR_INVALID_SIGNATURE, /**< Invalid MDIF signature. */
    MDIF_ERROR_CANNOT_ALLOCATE,   /**< Memory allocation error. */
    MDIF_ERROR_INVALID_WIDTH,     /**< Invalid image width (greater than 1024). */
    MDIF_ERROR_INVALID_HEIGHT,    /**< Invalid image height (greater than 1024). */
    MDIF_ERROR_READ,              /**< Error reading MDIF file. */
    MDIF_ERROR_WRITE,             /**< Error writing MDIF file. */
    MDIF_ERROR_INVALID_FILE_HANDLE,/**< Invalid file handle. */
    MDIF_ERROR_IMAGE,             /**< Generic image error. */
    MDIF_ERROR_GRAYSCALE          /**< Invalid grayscale pointer. */
} mdif_error_t;

/**
 * @brief Initialize an MDIF image.
 * 
 * This function initializes an MDIF image structure with the specified width and height.
 * It allocates memory for the red, green, blue, and alpha channels.
 * 
 * @param[in,out] image Pointer to the MDIF image structure to be initialized.
 * @param[in] width Width of the image.
 * @param[in] height Height of the image.
 */
void mdif_init(mdif_t* image, short width, short height);

/**
 * @brief Free the memory allocated for an MDIF image.
 * 
 * This function frees the memory allocated for the red, green, blue, and alpha channels of an MDIF image.
 * 
 * @param[in,out] image Pointer to the MDIF image structure to be freed.
 */
void mdif_free(mdif_t* image);

/**
 * @brief Read an MDIF image from a file.
 * 
 * This function reads an MDIF image from the specified file. It reads the image signature,
 * width, height, and color channel data.
 * 
 * @param[in] filename The name of the file to read from.
 * @param[in,out] image Pointer to the MDIF image structure to store the read data.
 * 
 * @return An mdif_error_t error code indicating the success or failure of the operation.
 */
mdif_error_t mdif_read(const char* filename, mdif_t* image);

/**
 * @brief Write an MDIF image to a file.
 * 
 * This function writes an MDIF image to the specified file. It writes the image signature,
 * width, height, and color channel data.
 * 
 * @param[in] filename The name of the file to write to.
 * @param[in] image Pointer to the MDIF image structure containing the data to be written.
 * 
 * @return An mdif_error_t error code indicating the success or failure of the operation.
 */
mdif_error_t mdif_write(const char* filename, mdif_t* image);

/**
 * @brief Convert an MDIF image to grayscale.
 * 
 * This function converts an MDIF image to grayscale. It calculates the grayscale value for each pixel
 * based on the red, green, and blue channel values, and stores the result in the provided grayscale array.
 * 
 * @param[in] image Pointer to the MDIF image structure.
 * @param[out] grayscale Pointer to the array to store the grayscale values.
 * 
 * @return An mdif_error_t error code indicating the success or failure of the operation.
 */
mdif_error_t mdif_grayscale(mdif_t* image, float* grayscale);

/**
 * @brief Perform antialiasing on an MDIF image.
 * 
 * This function performs antialiasing on the given MDIF image using a simple box blur method. 
 * The result is stored in a separate MDIF image structure provided by the user. 
 * The box blur technique averages the pixel values within a 3x3 kernel to smooth out the image.
 * 
 * @param[in] image Pointer to the MDIF image structure that needs to be antialiased.
 * @param[out] aliased_image Pointer to the MDIF image structure where the antialiased image will be stored.
 * 
 * @return An mdif_error_t error code indicating the success or failure of the operation.
 */
mdif_error_t mdif_antialias(mdif_t* image, mdif_t* aliased_image);

/**
 * @brief Get a human-readable error message.
 * 
 * This function returns a human-readable error message corresponding to the specified MDIF error code.
 * 
 * @param[in] error_num The MDIF error code.
 * 
 * @return A constant string containing the error message.
 */
const char* mdif_error_message(mdif_error_t error_num);

#endif