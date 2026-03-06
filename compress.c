/* 06/03/2026
 *
 * File Compressor, wrote by wilrak0v
 * It uses Raylib Game library to compress the data
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

#define EXTENSION ".wilcomp"

static inline void catch_errors()
{
    perror("Error: ");
    exit(-1);
}

static inline long get_size(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    return ftell(fp);
}

void compress_file(FILE *fp_output, size_t content_size, unsigned char *content);
void decompress_file(FILE *fp_output, size_t content_size, unsigned char *content);

int main(int argc, char *argv[])
{
    char *extension = ".wildecompress";
    // Get options and see if the user compress or no
    bool decompress = false;
    if (argc < 2 && 3 > argc)
    {
        printf("Usage: %s [-d] <file_to_process>\n", argv[0]);
        return -1;
    }
    char *input_file = argv[1];
    if (argc == 3 && !strcmp("-d", argv[1]))
    {
        decompress = true;
        input_file = argv[2];
    }
    else extension = ".wilcompress";

    // Open the files
    FILE *fp_input = fopen(input_file, "rb");
    if (fp_input == NULL)
        catch_errors();

    int output_length = strlen(input_file) + strlen(extension);
    char output_fn[output_length];
    snprintf(output_fn, output_length, "%s%s", GetFileNameWithoutExt(input_file), extension);

    FILE *fp_output = fopen(output_fn, "wb");
    if (fp_output == NULL)
        catch_errors();

    // Read the input file
    size_t input_size = get_size(fp_input);
    rewind(fp_input);
    unsigned char content[input_size];
    if (fread(content, sizeof(char), input_size, fp_input) != input_size)
        catch_errors();

    if (!decompress) compress_file(fp_output, input_size, content);
    else decompress_file(fp_output, input_size, content);

    // Close the files
    if (fclose(fp_input) != 0)
        catch_errors();
    if (fclose(fp_output) != 0)
        catch_errors();
    return 0;
}

void compress_file(FILE *fp_output, size_t content_size, unsigned char *content)
{
    int final_size = 0;
    unsigned char *result = CompressData(content, content_size, &final_size);

    // Write the output file
    if (fwrite(result, sizeof(char), final_size, fp_output) != final_size)
        catch_errors();
    UnloadFileData(result);
}

void decompress_file(FILE *fp_output, size_t content_size, unsigned char *content)
{
    int final_size = 0;
    unsigned char *result = DecompressData(content, content_size, &final_size);

    // Write the output file
    if (fwrite(result, sizeof(char), final_size, fp_output) != final_size)
        catch_errors();
    UnloadFileData(result);

}
