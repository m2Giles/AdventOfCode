#define AOC_UTILS_IMPLEMENTATION
#include "../../../lib/aoc.h"

#define NUMBERS 9
#define WORD_SIZE 5

char *word_numbers[NUMBERS][2] =
{
    {"one", "1"},
    {"two", "2"},
    {"three", "3"},
    {"four", "4"},
    {"five", "5"},
    {"six", "6"},
    {"seven", "7"},
    {"eight", "8"},
    {"nine", "9"},
};

int is_word_number(const char * buffer, int index)
{
    if (isdigit(buffer[index]))
    {
        return buffer[index] - '0';
    }
    char temp_buffer[WORD_SIZE] = {0};
    for (int i = 0; i < WORD_SIZE; ++i)
    {
        temp_buffer[i] = buffer[index + i];
        temp_buffer[i+1] = '\0';
        for (int j = 0; j < NUMBERS; ++j)
        {
            if(strncmp(temp_buffer, word_numbers[j][0], WORD_SIZE) == 0)
            {
                return word_numbers[j][1][0] - '0';
            }
        }
    }
    return 0;
}


int main(int argc, char ** argv){

    if (argc < 2) {
        fprintf(stderr, "No file provided");
        return EXIT_FAILURE;
    }

    int solution = 0;
    
    FILE * fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char buffer[BUFSIZ];
    int calibration_value[2];

    while(fgets(buffer, BUFSIZ, fp) != NULL)
    {
        int first = 1;
        int length = strlen(buffer);
        for(int i = 0; i < length; ++i)
        {
            if(isdigit(buffer[i]))
            {
                if(first){
                    calibration_value[0] = buffer[i] - '0';
                    first = 0;
                }
                calibration_value[1] = buffer[i] - '0';
            }
        }
        solution += calibration_value[0]*10 + calibration_value[1];
    } 

    printf("Problem 1 solution: %d\n", solution);

    solution = 0;
    rewind(fp);
    int count = 1;

    while(fgets(buffer, BUFSIZ, fp) != NULL)
    {
        int first = 1;
        int length = strlen(buffer);
        for(int i = 0; i < length; ++i)
        {
            int output = is_word_number(buffer, i);
            if(output){
                if(first){
                    calibration_value[0] = output;
                    first = 0;
                }
                calibration_value[1] = output;
            }
        }
        count++;
        solution += calibration_value[0] * 10 + calibration_value[1];
    }

    printf("Problem 2 solution: %d\n", solution);

    fclose(fp);
    return EXIT_SUCCESS;
}