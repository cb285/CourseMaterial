/* File: main.c
 *  Reads a text data file of a competition ID and degrees of difficulty and judges' scores for each dive for each diver.
 *  It computes the round score for each diver in the competition round and prints the diver competition IDs and the
 *  round scores in descending order by round score.
 *
 * --------------------------------------------------------------
 * Class: CS 210                        Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 5 Date assigned:  3/3/15
 * Programmer: Clayton Brutus           Date completed: 3/20/15
 */

#include <stdio.h>  /* printf, scanf, fprintf, fscanf */
#include <stdlib.h>

#define MAX_JUDGES 20   /* Constant: maximum number of judges */

FILE *get_file();
void compute_score(FILE *input_file, int ids[], double scores[], int *num_divers);
double read_dive_data(FILE *input_file);
void sort_arrays(int ids[], double scores[], int num_divers);
int find_index_of_largest(const double an_array[], int lower, int num_elements);

/*  Function: main
    prints greeting, prints the ids and round scores.
 */
int main()
{
    FILE *input_file;
    int ids[50];
    double scores[50];
    int num_divers;
    int i;

    /* print greeting */
    printf("Welcome to the diving competition scoring program.\n\n");

    /* open file */
    input_file = get_file();

    /* compute round scores */
    compute_score(input_file, ids, scores, &num_divers);

    /* sort arrays */
    sort_arrays(ids, scores, num_divers);

    /* print results */
    printf("\nID  Score\n");
    printf("--- ------\n");

    for(i=0; i<num_divers-1; i++)
        printf("%d %.2lf\n", ids[i], scores[i]);

    /* close file */
    fclose(input_file);

    return 0;
}

/*  Function: get_file
    ask user for file name and opens file.
 */
FILE *get_file()
{
    FILE *file;
    char filename[FILENAME_MAX];

    /* 1. Ask user for the filename */
    do
    {
        /* 2. Open the file */
        printf("Enter the diving data file name: ");
        scanf("%s", filename);

        file = fopen(filename, "r");

        /* 3. Check if successful */
        if (file == NULL)
            printf("Unable to open file. Try again.\n");

    }
    while(file == NULL);

    /* return file pointer */
    return file;
}

/*  Function: compute_score
    computes the round scores of divers.
 */
void compute_score(FILE *input_file,    /* Rec'd: file pointer  */
                   int ids[],           /* P'back: diver id's   */
                   double scores[],     /* P'back: round scores */
                   int *num_divers)     /* P'back: total number of divers */
{
    int i;
    int diver;
    double score;

    /* read from file until the end of data */
    for(diver = 0; fscanf(input_file, "%d", &ids[diver]) != EOF; diver++)
    {
        score = 0;
        for(i=0; i<6; i++)
            score += read_dive_data(input_file);

        scores[diver] = score;
    }

    /* pass back total number of divers */
    *num_divers = diver + 1;
}

/*  Function: read_dive_data
    reads one dive's data at a time and computes round score
 */
double read_dive_data(FILE *input_file)     /* Rec'd: file pointer */
{
    double difficulty;
    double value;
    double judge_scores[MAX_JUDGES];
    int i;

    /* read dive difficulty */
    fscanf(input_file, "%lf", &difficulty);

    /* read judge's scores */
    fscanf(input_file, "%lf", &value);

    judge_scores[0] = value;

    for(i=1; fscanf(input_file, "%lf", &value) != EOF && (value!= -1); i++)
        judge_scores[i] = value;

    /* compute round score */
    double high, low;
    double sum = 0;
    high = judge_scores[0];
    low = judge_scores[0];

    sum += judge_scores[0];

    for(i=1; i<7; i++)
    {
        sum += judge_scores[i];
        if(judge_scores[i] > high)
            high = judge_scores[i];

        else if(judge_scores[i] < low)
            low = judge_scores[i];
    }

    sum -= (high + low);

    /* return round score */
    return (sum * .6 * difficulty);
}

/*  Function: sort_arrays
    sorts scores and ids into descending order
 */
void sort_arrays(int ids[],         /* Rec'd/P'back: array of id's */
                 double scores[],   /* Rec'd/P'back: array of round scores */
                 int num_divers)    /* Rec'd: total number of divers */
{
    int pass, largest_index, tempi;
    double tempd;

    /* sort using select sort algorithm */
    for(pass=0; pass<num_divers-1; pass++)
    {
        /* find index of largest element */
        largest_index = find_index_of_largest(scores, pass, num_divers);

        /* place largest element at index pass */
        if(pass != largest_index)
        {
            tempd = scores[pass];
            scores[pass] = scores[largest_index];
            scores[largest_index] = tempd;

            tempi = ids[pass];
            ids[pass] = ids[largest_index];
            ids[largest_index] = tempi;
        }
    }
}

/*  Function: find_index_of_largest
    finds where largest element is located in a range of the scores array.
 */
int find_index_of_largest(const double scores[],  /* Rec'd: array of scores */
                          int lower,              /* Rec'd: lower bound of range */
                          int num_divers)       /* Rec'd: number of divers used as upper bound of range */
{
    int index;
    int largest_index;

    largest_index = lower;

    for(index=lower+1; index<num_divers; index++)
        if(scores[index] > scores[largest_index])
            largest_index = index;

    /* return index of the largest element */
    return largest_index;
}
