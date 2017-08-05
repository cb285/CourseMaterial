/* File: main.c
 * processes a Mad­Lib in a text file into another
 * text file which contains the resulting story.
 *
 * --------------------------------------------------------------
 * Class: CS 210                        Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 6 Date assigned:  3/26/15
 * Programmer: Clayton Brutus           Date completed: 4/6/15
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

FILE *open_file();
int is_vowel(const char ch);
int is_token(const char str[]);
void replace(const char word[], char replacement[]);
void token_to_english(const char token[], char english[]);
void process_file(FILE *input_file, FILE *output_file);


int main()
{
    FILE *input_file;
    FILE *output_file;

    /* prompt user for input and output filenames */
    input_file = open_file("Enter a mad-lib file name", "r");
    output_file = open_file("Enter a file name for the resulting story", "w");

    /* read input file using function process_file */
    process_file(input_file, output_file);

    /* close files */
    fclose(input_file);
    fclose(output_file);

    printf("\nProcessing finished. Enjoy your story.\n");

    return 0;
}

/*  Function: get_file
    ask user for file name and opens file.
 */
FILE *open_file(const char prompt[], const char mode[])
{
    FILE *file;
    char filename[FILENAME_MAX];

    /* 1. Ask user for the filename */
    do
    {
        /* 2. Open the file */
        printf("%s: ", prompt);
        scanf("%s", filename);
        while(getchar() != '\n');

        file = fopen(filename, mode);

        /* 3. Check if successful */
        if (file == NULL)
            printf("Unable to open file: %s. Try again!\n", filename);

    }
    while(file == NULL);

    /* return file pointer */
    return file;
}

/*  Function: is_vowel
    test if a character is a vowel.
 */
int is_vowel(const char ch)     /* Rec'd: character to be tested */
{
    /* Returned: true or false       */
    if(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
        return 1;
    else
        return 0;
}

/*  Function: is_token
    tests if a word is a madlib token that needs to be replaced.
 */
int is_token(const char str[])  /* Rec'd: word to be tested */
{
    /* Returned: true or false  */
    if(str[0] == '<')
        return 1;
    else
        return 0;
}

/*  Function: replace
 *  asks user for a replacement word if needed.
 *  otherwise, sends back same word.
 */
void replace(const char word[],     /* Rec'd: word from input file */
             char replacement[])    /* P'back: word to be put in output file */
{
    char english_token[MAX_STRING_LENGTH];

    /* if word is a token, then ask user for replacement */
    if(is_token(word))
    {
        token_to_english(word, english_token);
        if(is_vowel(english_token[0]))
        {

            printf("Please type an %s: ", english_token);
            fgets(replacement, MAX_STRING_LENGTH, stdin);
            replacement[strlen(replacement)-1] = '\0';
        }
        else
        {
            printf("Please type a %s: ", english_token);
            fgets(replacement, MAX_STRING_LENGTH, stdin);
            replacement[strlen(replacement)-1] = '\0';
        }
    }

    /* otherwise, the same word is set to be used in the output file */
    else
        strcpy(replacement, word);
}

/*  Function: token to english
    converts madlib token into plain english
 */
void token_to_english(const char token[], char english[])
{
    int i = 1;

    while(token[i] != '>')          /* copy the characters from token to english word */
    {
        if(token[i] == '_')
            english[i-1] = ' ';     /* replace '_'s with spaces */
        else
            english[i-1] = token[i];
        i++;
    }
    english[i-1] = '\0';    /* put the null character at end of word */
}

/*  Function: process_file
    read from the madlib file, and write results to the output file.
 */
void process_file(FILE *input_file,     /* Rec'd: input file pointer */
                  FILE *output_file)    /* Rec'd: output file pointer */
{
    char word[MAX_STRING_LENGTH];
    char replacement[MAX_STRING_LENGTH];

    fscanf(input_file, "%s", word);

    replace(word, replacement);
    fprintf(output_file, "%s", replacement);

    while(fscanf(input_file, "%s", word) != EOF)        /* read from madlib file until the end of the file */
    {
        replace(word, replacement);
        fprintf(output_file, " %s", replacement);       /* write result to output file */
    }

    fprintf(output_file, "\n");
}
