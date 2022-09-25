#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            float avgf = (float) sum / 3;
            int avgi = round(avgf);

            image[i][j].rgbtBlue = avgi;
            image[i][j].rgbtGreen = avgi;
            image[i][j].rgbtRed = avgi;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfwidth = round((float)width / 2);

    for (int i = 0; i < height; i++) //for each row
    {
        for (int j = 0; j < halfwidth; j++) //for each pixel in the first half of the row
        {
            //swap it with the opposite pixel in column width-1-j
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copy the image to preserve original pixel patterns
    RGBTRIPLE imagecopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j] = image[i][j];
        }
    }

    //for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumblue = 0;
            int sumred = 0;
            int sumgreen = 0;
            int numpixels = 0;

            // for each pixel in its box in imagecopy, if it's a valid pixel, add its color
            // - values to the totals (sumblue, sumred, sumgreen). keep track
            // - of how many valid pixels there are.
            for (int a = i - 1; a <= i + 1; a++)
            {
                if (a >= 0 && a < height)
                {
                    for (int b = j - 1; b <= j + 1; b++)
                    {
                        if (b >= 0 && b < width)
                        {
                            sumblue += imagecopy[a][b].rgbtBlue;
                            sumgreen += imagecopy[a][b].rgbtGreen;
                            sumred += imagecopy[a][b].rgbtRed;
                            numpixels++;
                        }
                    }
                }
            }

            //get the average color values of its box in imagecopy
            float avgbluef = (float) sumblue / numpixels;
            float avggreenf = (float) sumgreen / numpixels;
            float avgredf = (float) sumred / numpixels;

            //convert floats to ints
            int avgbluei = round(avgbluef);
            int avggreeni = round(avggreenf);
            int avgredi = round(avgredf);

            //set pixel in original image to new color
            image[i][j].rgbtBlue = avgbluei;
            image[i][j].rgbtGreen = avggreeni;
            image[i][j].rgbtRed = avgredi;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //copy the image to preserve original pixel patterns
    RGBTRIPLE imagecopy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j] = image[i][j];
        }
    }

    //for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //instantiate gx and gy values for each color
            double gxred = 0;
            double gyred = 0;
            double gxgreen = 0;
            double gygreen = 0;
            double gxblue = 0;
            double gyblue = 0;

            //gx and gy grid values
            int gxlist[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int gylist[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
            int gpoint = 0;

            //for each pixel in its surrounding box in imagecopy
            for (int a = i - 1; a <= i + 1; a++)
            {
                if (a >= 0 && a < height)
                {
                    for (int b = j - 1; b <= j + 1; b++)
                    {
                        if (b >= 0 && b < width)
                        {
                            //add weighted values to totals
                            gxred += gxlist[gpoint] * imagecopy[a][b].rgbtRed;
                            gyred += gylist[gpoint] * imagecopy[a][b].rgbtRed;
                            gxgreen += gxlist[gpoint] * imagecopy[a][b].rgbtGreen;
                            gygreen += gylist[gpoint] * imagecopy[a][b].rgbtGreen;
                            gxblue += gxlist[gpoint] * imagecopy[a][b].rgbtBlue;
                            gyblue += gylist[gpoint] * imagecopy[a][b].rgbtBlue;
                            gpoint++;
                        }
                        else
                        {
                            gpoint++;
                        }
                    }
                }
                else
                {
                    gpoint += 3;
                }
            }

            //calculate sobel: 0 is red, 1 is green, 2 is blue
            double sobels[3];
            sobels[0] = sqrt((gxred * gxred) + (gyred * gyred));
            sobels[1] = sqrt((gxgreen * gxgreen) + (gygreen * gygreen));
            sobels[2] = sqrt((gxblue * gxblue) + (gyblue * gyblue));


            //round sobel and make sure is btwn 0 and 255
            int sobelsi[3];
            for (int c = 0; c < 3; c++)
            {
                sobelsi[c] = round(sobels[c]);
                if (sobelsi[c] > 255)
                {
                    sobelsi[c] = 255;
                }
                else if (sobelsi[c] < 0)
                {
                    sobelsi[c] = 0;
                }
            }

            //set the rbg val of original input to the sobel val
            image[i][j].rgbtRed = sobelsi[0];
            image[i][j].rgbtGreen = sobelsi[1];
            image[i][j].rgbtBlue = sobelsi[2];
        }
    }
    return;
}
