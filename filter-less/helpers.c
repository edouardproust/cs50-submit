#include "helpers.h"

#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the average color from R, G and B
            int avgColor = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            // Set the color of each RGB to the average color
            image[i][j].rgbtBlue = avgColor;
            image[i][j].rgbtGreen = avgColor;
            image[i][j].rgbtRed = avgColor;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);

            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
        }
    }
    return;


    // Loop over all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;
            // Apply sepia
            int blue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            int green = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int red = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            // Apply filter (+ check maximum)
            image[i][j].rgbtBlue = blue > 255 ? 255 : blue;
            image[i][j].rgbtGreen = green > 255 ? 255 : green;
            image[i][j].rgbtRed = red > 255 ? 255 : red;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all the pixels
    for (int i = 0; i < height; i++)
    {
        int midPixel = width / 2;
        for (int j = 0; j < midPixel; j++)
        {
            RGBTRIPLE temp = image[i][j];
            int n = width - 1; // max index is off-one
            image[i][j] = image[i][n - j];
            image[i][n - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int BLUR_RANGE = 1; // how many pixels around the current one are used to calculate the blur?

    // Store image in a temporary one
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // Loop over all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Vars
            float red = 0;
            float green = 0;
            float blue = 0;
            int counter = 0;
            // Loop through the blur square (3 x 3)
            for (int k = - BLUR_RANGE; k <= BLUR_RANGE; k++)
            {
                for (int l = - BLUR_RANGE; l <= BLUR_RANGE; l++)
                {
                    // Check that pixel is included in the row
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        red += temp[i + k][j + l].rgbtRed;
                        green += temp[i + k][j + l].rgbtGreen;
                        blue += temp[i + k][j + l].rgbtBlue;
                        counter++;
                    }
                }
            }
            // Get the average and blur the image
            image[i][j].rgbtRed = round(red / counter);
            image[i][j].rgbtGreen = round(green / counter);
            image[i][j].rgbtBlue = round(blue / counter);
        }
    }
    return;
}
