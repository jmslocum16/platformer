#ifndef __image_manager_h_
#define __image_manager_h_

#include "bitmap.h"

struct Image {
	unsigned char* data;
	int width;
	int height;
};

struct Animation {
	int numFrames;
	Image* images;
};

void loadImage(char* fname, Image& image);
void loadAnimation(int numImages, char** fnames, Animation& animation);
#endif
