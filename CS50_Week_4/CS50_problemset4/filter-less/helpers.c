#include "helpers.h"
#include <math.h>

int average;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue.
            average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Update pixel values.
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;

            average = 0;
        }
    }
    return;
}

int sepiaBlue;
int sepiaGreen;
int sepiaRed;

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute sepia values.
            sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);

            // Make sure all values are capped at 255.
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            // Update pixel with sepia values.
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Loop over all pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels.
            temp[i][j] = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp[i][j];
        }
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare and assign variables for tracking averages.
    float Redaverage = 0;
    int Greenaverage = 0;
    float Blueaverage = 0;
    int Redcounter = 0;
    float Greencounter = 0;
    int Bluecounter = 0;

    // Declare buffer to later hold the blurred image to be copied to the original image.
    RGBTRIPLE buffer[height][width];

    // Loop over all pixels.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Check if each surrounding pixel to image[i][j] is valid, if so add its RGB values to total and increase divideby by one.

            // Check if pixel above is valid.
            if (i - 1 >= 0)
            {
                Redaverage = Redaverage + image[i - 1][j].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i - 1][j].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i - 1][j].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel above and to the right is valid.
            if (i - 1 >= 0 && j + 1 < width)
            {
                Redaverage = Redaverage + image[i - 1][j + 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i - 1][j + 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i - 1][j + 1].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located adjacently right is valid.
            if (j + 1 < width)
            {
                Redaverage = Redaverage + image[i][j + 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i][j + 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i][j + 1].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located below and to the right is valid.
            if (i + 1 < height && j + 1 < width)
            {
                Redaverage = Redaverage + image[i + 1][j + 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i + 1][j + 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i + 1][j + 1].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located below is valid.
            if (i + 1 < height)
            {
                Redaverage = Redaverage + image[i + 1][j].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i + 1][j].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i + 1][j].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located below and to the left is valid.
            if (i + 1 < height && j - 1 >= 0)
            {
                Redaverage = Redaverage + image[i + 1][j - 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i + 1][j - 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i + 1][j - 1].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located to the left is valid.
            if (j - 1 >= 0)
            {
                Redaverage = Redaverage + image[i][j - 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i][j - 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i][j - 1].rgbtBlue;
                Bluecounter++;
            }

            // Check if pixel located to the left and up is valid.
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                Redaverage = Redaverage + image[i - 1][j - 1].rgbtRed;
                Redcounter++;
                Greenaverage = Greenaverage + image[i - 1][j - 1].rgbtGreen;
                Greencounter++;
                Blueaverage = Blueaverage + image[i - 1][j - 1].rgbtBlue;
                Bluecounter++;
            }

            // Add base pixel.
            Redaverage = Redaverage + image[i][j].rgbtRed;
            Redcounter++;
            Greenaverage = Greenaverage + image[i][j].rgbtGreen;
            Greencounter++;
            Blueaverage = Blueaverage + image[i][j].rgbtBlue;
            Bluecounter++;

            // Calculate the average of each pixel including image[i][j].
            Redaverage = round(Redaverage / Redcounter);
            Greenaverage = round(Greenaverage / Greencounter);
            Blueaverage = round(Blueaverage / Bluecounter);

            // Set new pixel values in buffer.
            buffer[i][j].rgbtRed = Redaverage;
            buffer[i][j].rgbtGreen = Greenaverage;
            buffer[i][j].rgbtBlue = Blueaverage;

            //Reset variables for next pixel.
            Redaverage = 0;
            Greenaverage = 0;
            Blueaverage = 0;
            Redcounter = 0;
            Greencounter = 0;
            Bluecounter = 0;
        }
    }
    // Set blurred pixels values to original image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = buffer[i][j];
        }
    }
    return;
}
