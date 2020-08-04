#include "helpers.h"
#include <math.h>

// prototypes
RGBTRIPLE avgBlur(int row, int column, int height, int width, RGBTRIPLE img[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // range over rows
    for (int r = 0; r < height; r++)
    {
        // range over columns
        for (int c = 0; c < width; c++)
        {
            // get orig pixel
            BYTE red = image[r][c].rgbtRed;
            BYTE green = image[r][c].rgbtGreen;
            BYTE blue = image[r][c].rgbtBlue;

            // calculate average and replace original pixal
            BYTE avg = round(((float) red + green + blue) / 3);
            image[r][c].rgbtRed = avg;
            image[r][c].rgbtGreen = avg;
            image[r][c].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // range over rows
    for (int r = 0; r < height; r++)
    {
        // range over columns
        for (int c = 0; c < width; c++)
        {
            // get orig pixel
            BYTE red = image[r][c].rgbtRed;
            BYTE green = image[r][c].rgbtGreen;
            BYTE blue = image[r][c].rgbtBlue;

            // calculate sepia colors
            double sRed = round(.393 * (double) red + .769 * (double) green + .189 * (double) blue);
            double sGreen = round(.349 * (double) red + .686 * (double) green + .168 * (double) blue);
            double sBlue = round(.272 * (double) red + .534 * (double) green + .131 * (double) blue);

            // if higher then 255 set to 255
            if (sRed > 255)
            {
                sRed = 255;
            }
            if (sGreen > 255)
            {
                sGreen = 255;
            }
            if (sBlue > 255)
            {
                sBlue = 255;
            }

            // replace the original pixel
            image[r][c].rgbtRed = sRed;
            image[r][c].rgbtGreen = sGreen;
            image[r][c].rgbtBlue = sBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // range over rows
    for (int r = 0; r < height; r++)
    {
        // range over columns
        for (int c = 0; c < floorf((float) width / 2); c++)
        {
            RGBTRIPLE thisPixel = image[r][c];
            RGBTRIPLE otherPixel = image[r][width - 1 - c];

            image[r][c] = otherPixel;
            image[r][width - 1 - c] = thisPixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of the original image
    RGBTRIPLE original[height][width];
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            original[r][c] = image[r][c];
        }
    }

    // range over rows
    for (int r = 0; r < height; r++)
    {
        // range over columns
        for (int c = 0; c < width; c++)
        {
            image[r][c] = avgBlur(r, c, height, width, original);
        }
    }
    return;
}

RGBTRIPLE avgBlur(int row, int column, int height, int width, RGBTRIPLE img[height][width])
{
    // needed vars to count average
    long red = 0;
    long green = 0;
    long blue = 0;
    int cred = 0;
    int cgreen = 0;
    int cblue = 0;

    // position info
    int top = (row == 0);
    int bottom = (row == height - 1);
    int left = (column == 0);
    int right = (column == width - 1);

    // add itself
    red += img[row][column].rgbtRed;
    green += img[row][column].rgbtGreen;
    blue += img[row][column].rgbtBlue;
    cred++;
    cgreen++;
    cblue++;
    // add pixels
    if (!top)
    {
        red += img[row - 1][column].rgbtRed;
        green += img[row - 1][column].rgbtGreen;
        blue += img[row - 1][column].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!bottom)
    {
        red += img[row + 1][column].rgbtRed;
        green += img[row + 1][column].rgbtGreen;
        blue += img[row + 1][column].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!left)
    {
        red += img[row][column - 1].rgbtRed;
        green += img[row][column - 1].rgbtGreen;
        blue += img[row][column - 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!right)
    {
        red += img[row][column + 1].rgbtRed;
        green += img[row][column + 1].rgbtGreen;
        blue += img[row][column + 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!left && !top)
    {
        red += img[row - 1][column - 1].rgbtRed;
        green += img[row - 1][column - 1].rgbtGreen;
        blue += img[row - 1][column - 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!left && !bottom)
    {
        red += img[row + 1][column - 1].rgbtRed;
        green += img[row + 1][column - 1].rgbtGreen;
        blue += img[row + 1][column - 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!right && !top)
    {
        red += img[row - 1][column + 1].rgbtRed;
        green += img[row - 1][column + 1].rgbtGreen;
        blue += img[row - 1][column + 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }
    if (!right && !bottom)
    {
        red += img[row + 1][column + 1].rgbtRed;
        green += img[row + 1][column + 1].rgbtGreen;
        blue += img[row + 1][column + 1].rgbtBlue;
        cred++;
        cgreen++;
        cblue++;
    }

    BYTE avgR = round((double) red / cred);
    BYTE avgG = round((double) green / cgreen);
    BYTE avgB = round((double) blue / cblue);

    RGBTRIPLE newPix;
    newPix.rgbtRed = avgR;
    newPix.rgbtGreen = avgG;
    newPix.rgbtBlue = avgB;

    return newPix;
}