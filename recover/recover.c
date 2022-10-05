#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Your program should accept exactly one command-line argument, the name of a forensic image from which to recover JPEGs.
    // If your program is not executed with exactly one command-line argument, it should remind the user of correct usage, and main should return 1.
    // If the forensic image cannot be opened for reading, your program should inform the user as much, and main should return 1.
    // The files you generate should each be named ###.jpg, where ### is a three-digit decimal number, starting with 000 for the first image and counting up.
    // Your program, if it uses malloc, must not leak any memory.

    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open card.raw
    FILE *raw_file = fopen(argv[1], "r");

    if (!raw_file)
    {
        printf("Could not open %s.\n", argv[1]);
        return 2;
    }

    // Init. vVars
    int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];
    int img_index = 0;
    FILE *img;
    char filename[8];

    // Repeat until end of input  file
    while (fread(&buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Check if is the start of a new jpg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous image
            if (img_index != 0)
            {
                fclose(img);
            }

            // Init. file
            sprintf(filename, "%03i.jpg", img_index);
            img = fopen(filename, "w");
            img_index++;
        }

        // Write buffer content into file (if img has been initialized)
        if (img)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

    // Close all images
    fclose(img);
    fclose(raw_file);

    // Success
    return 0;
}