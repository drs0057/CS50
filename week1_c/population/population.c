#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // TODO: Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Enter the start size, the minimum size is 9: ");
    }
    while (start_size < 9);

    // TODO: Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("Enter the end size, it should be greater than or equal to the starting size: ");
    }
    while (end_size < start_size);

    // TODO: Calculate number of years until we reach threshold
    int size = start_size;
    int years = 0;
    while (size < end_size)
    {
        size = size + trunc(size/3) - trunc(size/4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);

}