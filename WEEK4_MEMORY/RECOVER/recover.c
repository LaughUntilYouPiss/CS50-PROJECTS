#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage : ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    // Create a buffer for a block of data
    uint8_t buffer[512];
    int count = 0;
    int is_jpeg = 0;
    char filename[9];
    FILE *img = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // IF START OF A JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {

            // IF ALREADY A JPEG
            if (is_jpeg)
            {
                fclose(img);
            }

            // Create JPEGs from the data
            sprintf(filename, "%03i.jpg", count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create file\n");
                return 1;
            }

            fwrite(buffer, 512, 1, img);

            is_jpeg = 1;
            count++;
        }

        else
        {
            // IF ALREADY FOUND JPEG
            if (is_jpeg)
            {
                fwrite(buffer, 512, 1, img);
            }
        }
    }

    // CLOSE FILES
    fclose(img);
    fclose(card);
}
