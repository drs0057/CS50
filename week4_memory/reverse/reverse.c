#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse INPUTFILE.wav OUTPUTFILE.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Error; could not open input file.\n");
        return 1;
    }

    // Read header into an array
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);
    long EOHeader = ftell(input); //should be 44 (size of header in bytes)

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) == 1)
    {
        printf("Error; unsupported file format.\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Error; could not open output file.\n");
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    long buffer;
    //determine how large the input file is
    fseek(input, 0, SEEK_END);
    long EOFile = ftell(input);
    long total_bytes = EOFile - EOHeader; //tells how many total bytes there are besides the header

    for (int i = 0; i < (total_bytes / block_size); i++)
    {
        fseek(input, -sizeof(block_size) * (i + 1), SEEK_END);
        fread(&buffer, sizeof(block_size), 1, input);
        fwrite(&buffer, sizeof(block_size), 1, output);
    }

    //Close files
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    char *wave = "WAVE";

    //iterate through the "format" member of the struct to determine if it says "WAVE"
    for (int i = 0; i < sizeof(header.format); i++)
    {
        if (header.format[i] != wave[i])
        {
            return 1;
        }
    }

    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}