# MDIF (Minimal Data Image Format)

MDIF (Minimal Data Image Format) is a simple and efficient image format designed for use with Diwa, a lightweight implementation of Artificial Neural Networks. MDIF is optimized for environments with limited resources, such as microcontrollers with PSRAM, but it is also suitable for desktop environments.

> [!WARNING]
> MDIF is underdevelopment and not yet ready.

MDIF is designed to have a minimalistic image data structure that balances simplicity for functionality. It provides essential image handling capabilities while being lightweight enough to be used in resource-constrained environments. The primary use case for MDIF is with Diwa, a neural network library that supports various platforms including microcontrollers, web assembly, and traditional operating systems.

## Features

- **Minimal and Efficient Data Structure**

    MDIF is designed with a focus on minimalism and efficiency. The data structure is intentionally simple, consisting of a signature for format identification, dimensions for width and height, and separate pointers for red, blue, green, and alpha color channels. This structure minimizes overhead and ensures that the image data can be processed quickly and easily, even in environments with limited resources.

- **Support for RGBA Color Model**

    MDIF supports the RGBA (Red, Green, Blue, Alpha) color model, which provides a comprehensive way to represent color images. The separate channels for red, green, blue, and alpha allow for detailed manipulation of image data, including transparency handling via the alpha channel.

- **Tools for Converting Between MDIF and Popular Image Formats (PNG, JPG)**

    MDIF includes tools for converting between MDIF and popular image formats such as PNG and JPG. These tools enable users to easily convert images to MDIF for efficient storage and processing, and then convert them back to standard formats for display or distribution.

- **Viewing Tools for Both Windows and Linux**

    MDIF comes with viewing tools that allow users to display MDIF images on both Windows and Linux platforms. These tools provide a straightforward way to visualize MDIF images, ensuring that users can quickly verify the contents of their image files.

## Data Structure

The `mdif_t` (a.k.a `mdif_struct`) is defined as follows:

```c
typedef struct mdif_struct {
    char signature[2];

    short width;
    short height;

    unsigned char *red;
    unsigned char *blue;
    unsigned char *green;
    unsigned char *alpha;
} mdif_t;
```

- **signature**: A 2-byte signature that identifies the file as an MDIF file. (Equivalent to string "NT")
- **width**: The width of the image in pixels.
- **height**: The height of the image in pixels.
- **red, blue, green, alpha**: Pointers to the image's color and alpha channel data. Each channel is stored as a separate array of bytes, allowing for efficient access and manipulation.
