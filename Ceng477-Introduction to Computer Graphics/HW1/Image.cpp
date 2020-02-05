#include "Image.h"

Image::Image(int width, int height)
    : width(width), height(height)
{
    data = new Color* [height];

    for (int y = 0; y < height; ++y)
    {
        data[y] = new Color [width];
    }
}

//
// Set the value of the pixel at the given column and row
//
void Image::setPixelValue(int col, int row, const Color& color)
{
    data[row][col] = color;
}

/* Takes the image name as a file and saves it as a ppm file. */
void Image::saveImage(const char *imageName) const
{
	FILE *output;

	output = fopen(imageName, "w");
	fprintf(output, "P3\n");
	fprintf(output, "%d %d\n", width, height);
	fprintf(output, "255\n");

	for(int y = 0 ; y < height; y++)
	{
		for(int x = 0 ; x < width; x++)
        {
            for (int c = 0; c < 3; ++c)
            {
                fprintf(output, "%d ", data[y][x].channel[c]);
            }
        }

		fprintf(output, "\n");
	}

	fclose(output);
}
