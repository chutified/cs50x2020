#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// define prototypes
int recover(FILE *image);
int startJPEG(BYTE block[]);

int main(int argc, char *argv[])
{
    // validate argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    // recover and close
    if (recover(file) == 0)
    {
        return 1;
    }
    fclose(file);
    return 0;
}

// recovers the images.
int recover(FILE *file)
{
    // remember the file count
    int count = 0;

    int recovering = 0;
    BYTE block[512];
    FILE *newFile;
    while (fread(block, sizeof(block), 1, file))
    {
        if (startJPEG(block) == 1)
        {
            if (recovering == 0)
            {
                recovering = 1;
            }
            else
            {
                fclose(newFile);
            }

            // new file
            char *name = malloc(16);
            sprintf(name, "%03i.jpg", count);
            count++;
            newFile = fopen(name, "w");
            if (newFile == NULL)
            {
                return 0;
            }
            free(name);
            fwrite(block, sizeof(block), 1, newFile);
        }
        else if (recovering)
        {
            // recovering the same JPEG
            fwrite(block, sizeof(block), 1, newFile);
        }
        // garbage
    }

    fclose(newFile);
    return 1;
}

// startJPEG returns non-zero if the BYTE is the beggining of the jpeg image.
int startJPEG(BYTE block[])
{
    unsigned char ch1 = block[0];
    unsigned char ch2 = block[1];
    unsigned char ch3 = block[2];
    unsigned char ch4 = block[3];
    return ch1 == 0xff && ch2 == 0xd8 && ch3 == 0xff && (ch4 >= 0xe0 && ch4 <= 0xef);
}