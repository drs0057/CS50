#include <cs50.h>
#include <stdio.h>

void print_spaces(int size, int i);
void print_bricks(int size, int i);

int main(void)
{
    //Get size from user
    int size;
    do
    {
        size = get_int("Size: ");
    }
    while (size < 1 || size > 8);

    //Build the pyramid using user defined functions
    for (int i = 0; i < size; i++)
    {
        print_spaces(i, size);
        print_bricks(i, size);
        printf("  ");
        print_bricks(i, size);
        printf("\n");
    }

}

//Calculates number of spaces then prints them
void print_spaces(int i, int size)
{
    int spaces = size - (i + 1);

    for (int j = 0; j < spaces; j++)
    {
        printf(" ");
    }
}

//Calculates number of bricks then prints them
void print_bricks(int i, int size)
{
    int bricks = i + 1;
    for (int t = 0; t < bricks; t++)
    {
        printf("#");
    }
}