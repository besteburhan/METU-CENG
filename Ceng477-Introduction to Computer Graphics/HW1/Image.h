#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstdio>
#include <cstdlib>

#include "defs.h"

//
// Access the color using either individual component names
// or the channel array.
//
typedef union Color
{
    struct
    {
        unsigned char red;
        unsigned char grn;
        unsigned char blu;
    };

    unsigned char channel[3];
} Color;

/* This class is provided to you for defining an image as a variable, manipulate it easily, and save it as a ppm file. */
class Image
{
public:
    Color** data;                   // Image data
	int width;						// Image width
	int height;						// Image height

	Image(int width, int height);	// Constructor
	void setPixelValue(int col, int row, const Color& color); // Sets the value of the pixel at the given column and row
	void saveImage(const char *imageName) const;	          // Takes the image name as a file and saves it as a ppm file. 
};

#endif
