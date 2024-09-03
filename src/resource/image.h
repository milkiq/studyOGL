#ifndef RESOURCE_IMAGE_H
#define RESOURCE_IMAGE_H

class Image {
private:
    unsigned char *data;
    int width, height, channels;

public:
    Image(const char *filename);
    ~Image();
    
    unsigned char *get_data() const;
    int get_width() const;
    int get_height() const;
    int get_channels() const;
};

#endif
