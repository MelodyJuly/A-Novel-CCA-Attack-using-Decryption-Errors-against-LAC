#include <stdio.h>
#include <string.h>
#include <time.h>
#include "random.h"


#define N 1024
#define Q 251
#define threshold_one_position 62
#define threshold_one_error 56
#define test_number 150000000
#define insert_ones 65 // the number of ones inserted.
#define SIZE_NOISE sizeof(char[N])

struct record
{
    char e1[N];
    char r[N];
    char e2[N];
};


void test_e2(char *s, char *e)
{
    srand((unsigned)(time(NULL)));
    rc4_setup();

    long sum_positive[N]  = {0};
    long sum_negative[N] = {0};
    long error_posi[N] = {0};
    /*Get noises*/
    int error_number = 0;
    FILE *file_noise = fopen("noise.txt", "w");
    FILE *file_result = fopen("result.txt", "w");
    for (unsigned int i = 0; i < test_number; i++)
    {
        char *e1 = (char *)malloc(N * sizeof(char));
        char *r = (char *)malloc(N * sizeof(char));
        char *e2 = (char *)malloc(N * sizeof(char));
        generate_controlled_noise_vector(e1);
        generate_noise_vector(r);
        generate_noise_vector(e2);
        if (decode(s, e, e1, r, e2, error_posi) == 1)
        {
            error_number++;
            struct record tmp;
            memcpy(tmp.e1, e1, SIZE_NOISE);
            memcpy(tmp.r, r, SIZE_NOISE);
            memcpy(tmp.e2, e2, SIZE_NOISE);
            fwrite (&tmp, sizeof(struct record), 1, file_noise);
            for (int i = 0; i < N; i++)
            {
                if (e2[i] == 1)
                {
                    sum_positive[i] += 1;
                }
                if (e2[i] == -1)
                {
                    sum_negative[i] += 1;
                }
            }
        }
        free(e1);
        free(r);
        free(e2);
    }

    float error_rate = (float)(error_number) / (float)(test_number);
    fprintf(file_result, "Have done %d tests\n", test_number);
    fprintf(file_result, "number of errors: %d \n", error_number);
    fprintf(file_result, "error rate: %f \n", error_rate);
    for (int temp_i = 0; temp_i < N; temp_i++)
    {
        fprintf(file_result,"%ld", error_posi[temp_i]);
    }
    fclose (file_noise);
    fclose (file_result);

}

int main()
{
    char s[] = {-1, 1, 1, 0, 0, 1, 0, -1, 1, 0, 1, 1, 1, -1, 0, 0, 1, 1, 0, -1, 1, 1, 0, 0, 1, -1, 1, 1, 0, 0, 0, 1, 0, 0, -1, 1, 1, 1, 0, 0, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 1, -1, -1, -1, 1, 1, 1, 0, 1, 0, 1, 1, 0, -1, 1, 0, 0, -1, 1, -1, 0, 0, 0, -1, -1, 0, 1, 0, 0, 1, 0, 1, -1, 0, 0, 1, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, -1, -1, 0, 0, 1, 0, 0, 1, -1, 1, 1, 1, -1, 1, 1, 0, -1, 0, 0, 1, -1, 0, 1, 0, -1, 0, 1, 1, -1, -1, 1, 1, 0, 0, 1, -1, 0, 1, 0, -1, -1, 1, -1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, -1, 0, 0, 0, 0, 1, -1, 1, 0, 1, 0, -1, 0, 1, -1, 1, -1, 0, 1, -1, 1, -1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, -1, 0, 1, 0, 1, 0, 0, -1, 1, 1, 1, -1, -1, -1, 0, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, 1, 0, 0, -1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 1, 1, 0, 1, 0, 0, 0, 1, -1, 0, 0, 0, 1, -1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, -1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, -1, 1, -1, -1, -1, 1, 0, 1, 0, 1, 0, 0, 1, -1, 0, -1, -1, 1, 1, 0, -1, 0, 1, 0, 1, -1, 1, 1, 0, 1, -1, 0, 0, 1, 0, 0, -1, 1, 0, 0, 0, -1, 1, 1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, -1, 0, 1, 1, 1, 0, 1, 0, -1, -1, 0, 1, -1, 1, 0, 0, 0, 1, -1, -1, 0, 0, -1, 1, -1, 0, -1, 0, -1, -1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, -1, 1, 1, 0, 1, 1, 0, -1, 1, 0, 1, 1, -1, -1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, -1, 1, 0, 0, 0, -1, 0, 0, 1, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 1, 1, -1, 1, -1, 1, 0, 1, 0, 0, 0, 1, 0, -1, 1, 1, 1, 0, 0, 1, 0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, -1, 0, 0, 0, 1, 0, -1, 1, 1, 1, 1, 1, 1, 0, 0, 1, -1, -1, 0, -1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, -1, 1, 0, 1, 0, 1, 0, 0, 0, -1, 0, 1, -1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, -1, -1, 1, -1, 0, 0, -1, 1, -1, 1, 0, 0, 0, 0, 0, 0, 1, -1, 1, 0, 1, 1, 1, 0, 0, 1, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, -1, 1, 0, 1, 0, 1, -1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, -1, -1, 1, 0, -1, 0, 0, 0, 0, 1, 1, 1, 0, -1, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 1, 0, 0, 0, 1, -1, 1, 1, 1, -1, 0, 1, 1, -1, 1, 1, 1, 0, 0, -1, -1, 1, 0, 0, -1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, -1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, -1, 1, -1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, -1, 1, 1, 0, -1, -1, 1, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 1, 1, -1, 0, 0, 0, -1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 0, -1, 1, 0, 1, 0, 0, 1, -1, 0, 1, 1, 1, 1, 0, 0, 0, 0, -1, -1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 0, 0, 0, -1, 0, 1, 1, -1, 1, -1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, -1, 0, 1, 0, 1, 1, 1, -1, 0, -1, 1, 1, 1, -1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, -1, 1, -1, 1, 0, -1, 0, 0, -1, 0, 0, 1, 1, -1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, -1, 0, 0, 0, 1, 0, -1, 0, 1, 0, 0, 0, 0, 1, -1, 1, 0, 1, 0, 0, 1, 1, 0, -1, 1, 0, 1, 0, 1, -1, 1, -1, 0, 0, 0, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, -1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, -1, 0, 0, 0, 1, 1, 0, 1, -1, 1, 0, -1, -1, 1, 0, 0, 0, 1, 0, -1, 1, 1, 0, 0, -1, 0, 1, -1, -1, -1, 0, 1, 0, 1, 0, 1, 0, -1, 1, 1, -1, 0, -1, -1, 1, 0, 1, 0, 0, -1, 0, 0, 0, 1, 1, 0, 1, -1, -1, 1, -1, -1, -1};
    char e[] = {0, 0, 0, -1, 1, 0, 1, -1, 1, 0, 0, 0, 0, 0, 1, 1, -1, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, -1, 0, 0, 0, 0, 1, -1, -1, -1, -1, 1, 1, 1, 0, -1, -1, 1, 0, -1, -1, 0, 1, 0, 1, 1, 0, -1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, -1, -1, 0, -1, 0, 1, -1, 0, 0, 0, 1, 1, 1, 1, 0, -1, 1, 0, 1, 1, 1, -1, 0, 0, -1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 1, -1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, -1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, -1, 0, 1, -1, 0, 0, 1, 0, -1, 0, -1, -1, 0, -1, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, -1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, 0, 0, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, -1, -1, -1, 0, 0, 1, 0, -1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, -1, 1, 1, 0, 0, 1, -1, 0, -1, 1, 0, -1, 1, 1, -1, 0, 0, 0, -1, 0, 0, 1, -1, 0, -1, -1, 1, 0, 0, 0, 0, 0, -1, 1, 0, 1, 0, 0, 1, -1, 0, 0, 1, 0, 0, 1, -1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, -1, -1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, -1, 0, 1, 0, 0, 0, 1, 1, 0, 0, -1, -1, -1, 0, 1, 1, 0, 1, 1, -1, -1, -1, 1, 0, 0, 0, -1, 0, -1, -1, -1, 1, 0, 1, 0, 0, -1, -1, 1, 0, -1, 0, 1, 0, -1, 1, 0, -1, 0, -1, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, -1, 1, -1, 1, 0, 0, -1, 0, 1, 0, -1, 1, 0, 0, 1, 0, -1, 0, 1, 0, 1, 1, -1, 1, 0, -1, 0, 1, 0, 1, -1, 1, -1, 1, 0, 0, -1, 1, 1, 0, 0, -1, 1, 1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, -1, -1, -1, 0, 0, 0, 1, 0, -1, 1, 1, 0, -1, 0, -1, -1, 0, 0, 0, 1, 0, -1, 1, 0, -1, 1, 0, 0, -1, 0, 0, 1, 0, -1, -1, -1, 1, -1, -1, 0, 1, 0, 0, 0, 0, -1, 1, 0, -1, -1, 0, 0, 0, 0, 1, 0, -1, 0, 0, 0, 0, 0, 1, -1, 0, -1, -1, 0, 0, 1, 0, 0, -1, 0, 1, -1, 0, 1, 0, -1, 0, -1, 0, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 1, -1, 0, 1, 0, 0, -1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, -1, 1, 0, -1, 1, 1, 0, -1, -1, 1, -1, 0, 0, -1, -1, -1, 0, 0, 0, -1, 0, -1, 1, 0, 1, -1, 0, 1, 1, 1, 0, 0, -1, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, 1, 0, 0, 1, -1, -1, 1, 1, 1, -1, 1, 0, 1, -1, 0, 1, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 0, 1, 0, 0, 0, 1, -1, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, 1, -1, 1, 0, -1, 1, 0, 1, 0, -1, 1, 0, -1, 0, -1, 0, 0, 0, 0, -1, 1, 0, 1, 1, 1, 0, 0, 1, 0, -1, 1, -1, 1, 1, 0, -1, 0, 0, 0, -1, 1, 0, 0, -1, 0, -1, -1, 1, 1, 0, 0, 0, 1, -1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, -1, 0, 0, 1, -1, 0, 0, 0, -1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, -1, 0, -1, 0, 0, -1, 1, -1, 1, 0, 1, -1, 0, 1, 1, 1, 1, 1, 0, 0, -1, 0, 1, 1, 1, 0, 1, 1, -1, -1, 0, -1, 1, -1, 1, 0, -1, 0, 0, -1, 0, 1, -1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, -1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 1, -1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 1, 1, 0, 1, 1, -1, -1, 1, -1, 1, 0, -1, 0, -1, 0, 1, -1, 1, 0, 1, 0, 0, 1, 0, 1, -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, 1, 0, -1, 1, 0, 0, 1, 1, 0, -1, 1, -1, 1, 0, 0, -1, -1, 0, -1, 0, 0, -1, 0, -1, 1, 0, 1, 0, 1, -1, 0, 0, -1, -1, 0, 1, 0, 0, 0, -1, -1, -1, 1, 0, 0, -1, 0, 0, 0, 1, -1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, -1, -1, 1, -1, 0, 1, 1, 1, 1, -1, 1, -1, 1, 0, -1, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 1, -1, 0, 1, 0, -1, 1, 1, 0, -1, 0, 1, 1, 0, 0, -1, -1, 0, -1, 0, 1, -1, 1, 0, 0, 0, -1, 0, -1, -1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, -1, -1, 0, -1, 0, -1, 1, 1, -1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0};
    test_e2(s, e);
    return 0;
}


/*
// small code to read and test stored collected errors
int main()
{
    FILE *fileobj = fopen("noise.txt", "r");
    struct record temp;
    while(fread(&temp, sizeof(struct record), 1, fileobj))
    {
        printf("***********************************");
        for(int i = 0; i < N; i++)
        {
            printf("%d \t", temp.e1[i]);
        }
        printf("%d \n");
        for(int i = 0; i < N; i++)
        {
            printf("%d \t", temp.r[i]);
        }
        printf("%d \n");
        for(int i = 0; i < N; i++)
        {
            printf("%d \t", temp.e2[i]);
        }
        printf("%d \n");
    }
    fclose(fileobj);
    return 0;
}
*/



