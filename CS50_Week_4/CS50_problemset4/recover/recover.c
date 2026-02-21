// Recovers JPEG images.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    #define BLOCK_SIZE 512

    // Check to ensure only one command line argument was entered.
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card.
    FILE *card = fopen(argv[1], "r");

    // If the forensic image cannot be opened, inform the user.
    if (card == NULL)
    {
        printf("Unable to open file :(\n");
        return 2;
    }

    // Create buffer to hold blocks of data from the card.
    uint8_t buffer[BLOCK_SIZE];

    // Create an array of chars for holding jpeg file names.
    char filename[8];

    // Keeps track of which jpeg image were on.
    int file_number = 0;

    // Pointer to open and close new JPEG files. Open to fix closing error within if statement tree.
    FILE *ptr = NULL;

    // While there's still data left on the card, keep reading.
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff)
        {
            if (buffer[1] == 0xd8)
            {
                if (buffer[2] == 0xff)
                {
                    if ((buffer[3] & 0xf0) == 0xe0)
                    {
                        if (ptr != NULL)
                        {
                            fclose(ptr);
                        }
                        sprintf(filename, "%03i.jpg", file_number);
                        ptr = fopen((filename), "w");
                        file_number++;
                    }
                }
            }
        }
        if (ptr == NULL)
        {
            continue;
        }
        fwrite(buffer, BLOCK_SIZE, 1, ptr);
    }
    fclose(ptr);
    fclose(card);
}
