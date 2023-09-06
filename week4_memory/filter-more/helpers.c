#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double avg = 0; //used to determine the average of the RGB values for each pixel

    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width; p++) //iterates through each pixel on the row
        {
            //average all the colors
            avg = (image[r][p].rgbtBlue + image[r][p].rgbtGreen + image[r][p].rgbtRed) / 3.0;

            //round the avg to nearest integer
            avg = round(avg);

            //assign the new average to each RGB value
            image[r][p].rgbtBlue = avg;
            image[r][p].rgbtGreen = avg;
            image[r][p].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary values
    int tempb = 0;
    int tempg = 0;
    int tempr = 0;
    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width / 2; p++) //iterates through each pixel on the row
        {
            //define temporary variables before swapping pixel values
            tempb = image[r][p].rgbtBlue;
            tempg = image[r][p].rgbtGreen;
            tempr = image[r][p].rgbtRed;

            //now that those values have been saved, rewrite the pixel to its corresponding swapee
            image[r][p].rgbtBlue = image[r][width - p - 1].rgbtBlue;
            image[r][p].rgbtGreen = image[r][width - p - 1].rgbtGreen;
            image[r][p].rgbtRed = image[r][width - p - 1].rgbtRed;

            //make sure the colors in the corrresponding pixel get their appropriate colors
            image[r][width - p - 1].rgbtBlue = tempb;
            image[r][width - p - 1].rgbtGreen = tempg;
            image[r][width - p - 1].rgbtRed = tempr;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a new  array that can be used to calculate blur values without changing image[]
    RGBTRIPLE blur[height][width];
    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width; p++) //iterates through each pixel on the row
        {
            blur[r][p].rgbtBlue = image[r][p].rgbtBlue;
            blur[r][p].rgbtGreen = image[r][p].rgbtGreen;
            blur[r][p].rgbtRed = image[r][p].rgbtRed;
        }
    }

    //now calculate pixel values in blur[] and assign them to image[]
    int sumb = 0;
    int sumg = 0;
    int sumr = 0;
    int avgb = 0;
    int avgg = 0;
    int avgr = 0;
    double count = 0; //counts how many pixels were used in the average
    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width; p++) //iterates through each pixel on the row
        {
            for (int y = r - 1; y <= r + 1; y++) //steps through the rows that form the 3x3
            {
                for (int x = p - 1; x <= p + 1; x++) //steps through the pixels in each 3x3 row
                {
                    if (x >= 0 &&  x < width && y >= 0 && y < height) //only pixels within the image will be added
                    {
                        sumb += blur[y][x].rgbtBlue;
                        sumg += blur[y][x].rgbtGreen;
                        sumr += blur[y][x].rgbtRed;
                        count++;
                    }
                }
            }

            //now find the average of the 3x3 sum
            avgb = round(sumb / count);
            avgg = round(sumg / count);
            avgr = round(sumr / count);

            //now assign these colors to the corresponding pixel in image[]
            image[r][p].rgbtBlue = avgb;
            image[r][p].rgbtGreen = avgg;
            image[r][p].rgbtRed = avgr;

            //reset all values used for calculations
            sumb = 0;
            sumg = 0;
            sumr = 0;
            count = 0;

        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //create a new  array that can be used to calculate edge values without changing image[]
    RGBTRIPLE edge[height][width];
    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width; p++) //iterates through each pixel on the row
        {
            edge[r][p].rgbtBlue = image[r][p].rgbtBlue;
            edge[r][p].rgbtGreen = image[r][p].rgbtGreen;
            edge[r][p].rgbtRed = image[r][p].rgbtRed;
        }
    }

    //now calculate pixel values in edge[] and assign them to image[]
    //initialize values for Gx
    int sumbGy = 0;
    int sumgGy = 0;
    int sumrGy = 0;

    //initialize values for Gy
    int sumbGx = 0;
    int sumgGx = 0;
    int sumrGx = 0;

    //will be used to round and cap the total gradient
    int b = 0;
    int g = 0;
    int d = 0;

    //create tables Gx and Gy
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int r = 0; r < height; r++) //iterates through each row
    {
        for (int p = 0; p < width; p++) //iterates through each pixel on the row
        {
            //we have arrived at a certain pixel. Time to stay here and calculate values
            for (int y = r - 1; y <= r + 1; y++) //steps through the rows that form the 3x3
            {
                for (int x = p - 1; x <= p + 1; x++) //steps through the pixels in each 3x3 row
                {
                    if (x >= 0 && x < width && y >= 0 && y < height) //the surrounding pixels will not add anything to the sum, a la "black"
                    {
                        //first, Gx
                        sumbGx += Gx[y - r + 1][x - p + 1] * edge[y][x].rgbtBlue;
                        sumgGx += Gx[y - r + 1][x - p + 1] * edge[y][x].rgbtGreen;
                        sumrGx += Gx[y - r + 1][x - p + 1] * edge[y][x].rgbtRed;

                        //then, Gy
                        sumbGy += Gy[y - r + 1][x - p + 1] * edge[y][x].rgbtBlue;
                        sumgGy += Gy[y - r + 1][x - p + 1] * edge[y][x].rgbtGreen;
                        sumrGy += Gy[y - r + 1][x - p + 1] * edge[y][x].rgbtRed;
                    }

                }
            }

            //round the gradients and cap at 255
            b = round(sqrt((sumbGx * sumbGx) + (sumbGy * sumbGy)));
            g = round(sqrt((sumgGx * sumgGx) + (sumgGy * sumgGy)));
            d = round(sqrt((sumrGx * sumrGx) + (sumrGy * sumrGy)));

            if (b > 255)
            {
                b = 255;
            }
            else if (g > 255)
            {
                g = 255;
            }
            else if (d > 255)
            {
                d = 255;
            }

            //now assign these colors to the corresponding pixel in image[]
            image[r][p].rgbtBlue = b;
            image[r][p].rgbtGreen = g;
            image[r][p].rgbtRed = d;

            //reset all values used for calculations
            sumbGy = 0;
            sumgGy = 0;
            sumrGy = 0;

            sumbGx = 0;
            sumgGx = 0;
            sumrGx = 0;

        }
    }

    return;

}
