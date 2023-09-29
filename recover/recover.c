#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: ./recover filename\n");
        return 1;
    }
    FILE *derfile = fopen(argv[1], "r");

    if (derfile == NULL)
    {
        printf("Can't open");
        return 1;
    }

    FILE *current;
    current = NULL;
    int i = 0;
    char *filename = NULL;
    filename = malloc(8 * sizeof(char));

    BYTE buffer[512];
    while(fread(buffer, sizeof(char), 512, derfile) == 512)
    {

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (current != NULL)
            {
                fclose(current);
                sprintf(filename, "%03i.jpg", i);
                current = fopen(filename, "w");
                i++;
            } else {
                sprintf(filename, "%03i.jpg", i);
                current = fopen(filename, "w");
                i++;
            }
        }

        if (current != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, current);
        }
    }
    fclose(current);
    fclose(derfile);
    free(filename);
    return 0;
}