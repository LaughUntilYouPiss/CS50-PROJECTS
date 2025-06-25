#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j].rgbtBlue == image[i][j].rgbtGreen &&
                image[i][j].rgbtGreen == image[i][j].rgbtRed)
            {
                continue;
            }

            int average = round(
                (float) (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
}

// Reflect image horizontally
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, half = width / 2; j < half; j++)
        {
            swap(&image[i][j], &image[i][width - j - 1]);
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // CREATE A COPY OF THE IMAGE
    RGBTRIPLE temp[height][width];

    // LOOP THROUGH EACH PIXEL IN THE IMAGE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;

            // LOOP THROUGH THE NEIGHBORING PIXELS
            for (int row = -1; row <= 1; row++)
            {
                int in = i + row;

                // SKIP THE OUT-OF-BOUND ROWS
                if (in < 0 || in >= height)
                {
                    continue;
                }

                for (int col = -1; col <= 1; col++)
                {
                    int jn = j + col;

                    // SKIP THE OUT-OF-BOUND COLUMNS
                    if (jn < 0 || jn >= width)
                    {
                        continue;
                    }

                    // CALCULATE THE AVERAGE OF THE SURROUNDING PIXELS
                    sumRed += image[in][jn].rgbtRed;
                    sumGreen += image[in][jn].rgbtGreen;
                    sumBlue += image[in][jn].rgbtBlue;

                    count++;
                }
            }

            // STORE THE AVERAGE RGB IN THE TEMPORARY IMAGE
            temp[i][j].rgbtRed = round(sumRed / (float) count);
            temp[i][j].rgbtGreen = round(sumGreen / (float) count);
            temp[i][j].rgbtBlue = round(sumBlue / (float) count);
        }
    }

    // COPY THE BLURRED VALUES TO THE ORIGINAL IMAGE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // CREATE A COPY OF THE IMAGE
    RGBTRIPLE temp[height][width];

    // CREATE THE Gx AND Gy MATRICES
    int Gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1},
                    { 0,  0,  0},
                    { 1,  2,  1}};

    //LOOP THROUGH EACH PIXEL IN THE IMAGE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int mulRedX = 0, mulGreenX = 0, mulBlueX = 0,
                mulRedY = 0, mulGreenY = 0, mulBlueY = 0;

            // LOOP THROUGH THE NEIGHBORING PIXELS
            for (int row = -1; row <= 1; row++)
            {
                int in = i + row;

                // SKIP PAST BORDER ROWS
                if (in < 0 || in >= height)
                {
                    continue;
                }

                for (int col = -1; col <= 1; col++)
                {
                    int jn = j + col;

                    // SKIP PAST BORDER COLUMNS
                    if (jn < 0 || jn >= width)
                    {
                        continue;
                    }

                    // MULTIPLY THE SURROUNDING PIXELS BY THE GX AND GY MATRICES
                    mulRedX += image[in][jn].rgbtRed * Gx[row + 1][col + 1];
                    mulGreenX += image[in][jn].rgbtGreen * Gx[row + 1][col + 1];
                    mulBlueX += image[in][jn].rgbtBlue * Gx[row + 1][col + 1];

                    mulRedY += image[in][jn].rgbtRed * Gy[row + 1][col + 1];
                    mulGreenY += image[in][jn].rgbtGreen * Gy[row + 1][col + 1];
                    mulBlueY += image[in][jn].rgbtBlue * Gy[row + 1][col + 1];
                }
            }

            // STORE THE AVERAGE RGB IN THE TEMPORARY IMAGE
            int redValue = round(sqrt(pow(mulRedX, 2) + pow(mulRedY, 2)));
            int greenValue = round(sqrt(pow(mulGreenX , 2) + pow(mulGreenY, 2)));
            int blueValue = round(sqrt(pow(mulBlueX, 2) + pow(mulBlueY, 2)));

            // CHECK THE RGB VALUES
            temp[i][j].rgbtRed = (redValue > 255) ? 255 : redValue;
            temp[i][j].rgbtGreen = (greenValue > 255) ? 255 : greenValue;
            temp[i][j].rgbtBlue = (blueValue > 255) ? 255 : blueValue;
        }
    }

    // COPY THE BLURRED VALUES TO THE ORIGINAL IMAGE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}
