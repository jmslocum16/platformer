#include "ImageManager.h"

#include <iostream>
#include <string>
using namespace std;

void loadImage(char* fname, Image& image) {
	image.data = readBMP(fname, image.width, image.height);
}

void loadAnimation(int numImages, char** fnames, Animation& animation) {
	animation.numFrames = numImages;
	animation.images = new Image[numImages];
	for (int i = 0; i < numImages; i++) {
		loadImage(fnames[i], animation.images[i]);
	}
}
