#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "image.h"

Image::Image(const char *filename): data(nullptr), width(0), height(0), channels(0) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename, &width, &height, &channels, 0);
}

Image::~Image() {
    if (data) {
        stbi_image_free(data);
    }
}

unsigned char *Image::get_data() const {
    return data;
}

int Image::get_width() const {
    return width;
}

int Image::get_height() const {
    return height;
}

int Image::get_channels() const {
    return channels;
}
