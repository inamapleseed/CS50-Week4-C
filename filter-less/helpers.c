#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
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
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            if(sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if(sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if(sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, halfwidth = floor(width / 2); j < halfwidth; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy
    RGBTRIPLE copy[height][width];
    int fours = 4;

    float aveRedTL;
    float aveGreenTL;
    float aveBlueTL;

    float aveRedBL;
    float aveGreenBL;
    float aveBlueBL;

    float aveRedUR;
    float aveGreenUR;
    float aveBlueUR;

    float aveRedBR;
    float aveGreenBR;
    float aveBlueBR;

    int niners = 9;

    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            copy[a][b] = image[a][b];

            // corners ------------------------
            // corners upper left neighbours
            aveRedTL = round((copy[0][0].rgbtRed + copy[0][1].rgbtRed + copy[1][0].rgbtRed + copy[1][1].rgbtRed) / fours);
            aveGreenTL = round((copy[0][0].rgbtGreen + copy[0][1].rgbtGreen + copy[1][0].rgbtGreen + copy[1][1].rgbtGreen) / fours);
            aveBlueTL = round((copy[0][0].rgbtBlue + copy[0][1].rgbtBlue + copy[1][0].rgbtBlue + copy[1][1].rgbtBlue) / fours);

            // corners bottom left
            aveRedBL = round((copy[height - 1][0].rgbtRed + copy[height - 1][1].rgbtRed + copy[height - 2][0].rgbtRed + copy[height - 2][1].rgbtRed) / fours);
            aveGreenBL = round((copy[height - 1][0].rgbtGreen + copy[height - 1][1].rgbtGreen + copy[height - 2][0].rgbtGreen + copy[height - 2][1].rgbtGreen) / fours);
            aveBlueBL = round((copy[height - 1][0].rgbtBlue + copy[height - 1][1].rgbtBlue + copy[height - 2][0].rgbtBlue + copy[height - 2][1].rgbtBlue) / fours);

            // corners upper right
            aveRedUR = round((copy[0][width - 1].rgbtRed + copy[0][width - 2].rgbtRed + copy[1][width - 1].rgbtRed + copy[1][width - 2].rgbtRed) / fours);
            aveGreenUR = round((copy[0][width - 1].rgbtGreen + copy[0][width - 2].rgbtGreen + copy[1][width - 1].rgbtGreen + copy[1][width - 2].rgbtGreen) / fours);
            aveBlueUR = round((copy[0][width - 1].rgbtBlue + copy[0][width - 2].rgbtBlue + copy[1][width - 1].rgbtBlue + copy[1][width - 2].rgbtBlue) / fours);

            // corners bottom right
            aveRedBR = round((copy[height - 1][width - 1].rgbtRed + copy[height - 1][width - 2].rgbtRed + copy[height - 2][width - 1].rgbtRed + copy[height - 2][width - 2].rgbtRed) / fours);
            aveGreenBR = round((copy[height - 1][width - 1].rgbtGreen + copy[height - 1][width - 2].rgbtGreen + copy[height - 2][width - 1].rgbtGreen + copy[height - 2][width - 2].rgbtGreen) / fours);
            aveBlueBR = round((copy[height - 1][width - 1].rgbtBlue + copy[height - 1][width - 2].rgbtBlue + copy[height - 2][width - 1].rgbtBlue + copy[height - 2][width - 2].rgbtBlue) / fours);
            // end corners ------------------------

            // 9x9
            if (a > 0 && a < height - 1 && b > 0 && b < width - 1)
            {
                int sumRed = 0;
                int sumBlue = 0;
                int sumGreen = 0;
                int column;

                for (int row = a - 1; row <= a + 1; row++)
                {
                    for (column = b - 1; column <= b + 1; column++)
                    {
                        sumRed += copy[row][column].rgbtRed;
                        sumGreen += copy[row][column].rgbtGreen;
                        sumBlue += copy[row][column].rgbtBlue;
                        
                        image[a][b].rgbtRed = round(sumRed / niners);
                        image[a][b].rgbtGreen = round(sumGreen / niners);
                        image[a][b].rgbtBlue = round(sumBlue / niners);
                    }
                }
                // int test = sumRed;
            }
            // 9x9
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // if (i > 0 && i < height - 1 && j > 0 && j < width -1)
            // {
            //     image[i][j].rgbtRed = aveRed[i][j];
            //     image[i][j].rgbtGreen = aveGreen[i][j];
            //     image[i][j].rgbtBlue = aveBlue[i][j];
            // }
            // corners
            if (i == 0 && j == 0)
            {
                image[i][j].rgbtRed = aveRedTL;
                image[i][j].rgbtGreen = aveGreenTL;
                image[i][j].rgbtBlue = aveBlueTL;
            }
            if (j == 0 && i == (height - 1))
            {
                image[i][j].rgbtRed = aveRedBL;
                image[i][j].rgbtGreen = aveGreenBL;
                image[i][j].rgbtBlue = aveBlueBL;
            }
            if (j == (width - 1) && i == 0)
            {
                image[i][j].rgbtRed = aveRedUR;
                image[i][j].rgbtGreen = aveGreenUR;
                image[i][j].rgbtBlue = aveBlueUR;
            }
            if (j == (width - 1) && i == (height - 1))
            {
                image[i][j].rgbtRed = aveRedBR;
                image[i][j].rgbtGreen = aveGreenBR;
                image[i][j].rgbtBlue = aveBlueBR;
            }
            // end corners
        }
    }

    return;
}
