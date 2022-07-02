#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Get number of rows from the user
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // print rows
    for (int row = 1; row <= height; row++)
    {
        // print characters for each rows
        for (int space = height - row; space > 0; space--)
        {
            printf(" ");
        }
        for (int block = 1; block <= row; block++)
        {
            printf("#");
        }
        printf("  ");
        for (int block = 1; block <= row; block++)
        {
            printf("#");
        }
        // linebreak
        printf("\n");
    }
}