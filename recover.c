#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("invalid");
        return 1;
    }
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    FILE *img = NULL;
    BYTE buffer[512];
    int jpgcount = 0;
    char *filename = malloc(sizeof(char)*8);

    while (fread(buffer, sizeof(BYTE), 512, raw_file) == 512)
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg",jpgcount);
            img = fopen(filename,"w");

            if (img == NULL)
            {
                free(filename);
                fclose(raw_file);
                printf("Error creating file\n");
                return 1;
            }

            jpgcount++;
        }

        if (img != NULL)
        {
            fwrite(buffer,sizeof(BYTE),512,img);
        }
    }
    if (img != NULL)//valgrind check
    {
        fclose(img);
    }
    fclose(raw_file);
    free(filename);

    return 0;
}
