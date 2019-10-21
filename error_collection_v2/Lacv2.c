#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include<time.h>

#include "random.h"
#include "Lacv2.h"  
 
// generate sparse secret of a 1 and b -1
void generate_sparse_secret(char *s, int a, int b, int l)
{
    int idx; 
    for (int i = 0; i < a; i++)
    {
        do
        { 
            idx = (r() % l);
        } while (s[idx] != 0);
        s[idx] = 1;
    }
    for (int i = 0; i < b; i++)
    {
        do
        {
            idx = (r() % l);
        } while (s[idx] != 0);
        s[idx] = -1;
    }
}

// Assume that the odd part has more positives than negatives.
void generate_sparse_secrect_bias(char *s, int diff)
{
    generate_sparse_secret(s, h / 2, h / 2, N);

    int diffTemp = SumOdd(s, N);
    int idxOdd, idxEven;
    int temp;

    while (diffTemp < diff)
    {
        do
        {
            idxOdd = 2 * (((int)(rc4()) * 256 + (int)(rc4())) % h) + 1;
            idxEven = 2 * (((int)(rc4()) * 256 + (int)(rc4())) % h);
        } while (s[idxOdd] >= s[idxEven]);
        temp = s[idxOdd];
        s[idxOdd] = s[idxEven];
        s[idxEven] = temp;
        diffTemp += (s[idxOdd] - s[idxEven]);
    }
    printf("%d, %d\n", SumOdd(s, N), SumEven(s, N));
}

void printVector(int *s, int l)
{
    for (int i = 0; i < l; i++)
    {
        printf("%d, ", s[i]);
    }
} 
    
char uniform15()
{
    return (r() % 15) - 7;
}  
 
char LAC_noise() 
{
    int temp;
    // srand(time(NULL));
    temp = rc4() % 4;
    if (temp == 0)
    {
        return 1;
    }
    else if (temp == 1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void generate_bias_nosie(char *s, int l, int k)
{
    int idx;
    int len_temp = l - 2 * k;
    for (int i = len_temp; i < l; i++)
    {
        if (i % 2 == 0)
        {
            s[i] = 1;
        }
        else
        {
            s[i] = -1;
        }
    }

    for (int i = 0; i < HALF_h - k; i++)
    {
        do
        {
            idx = (r() % len_temp);
        } while (s[idx] != 0);
        s[idx] = 1;
    }
    for (int i = 0; i < HALF_h - k; i++)
    {
        do
        {
            idx = (r() % len_temp);
        } while (s[idx] != 0);
        s[idx] = -1;
    }
}

void generate_bias_nosie_typeII(char *s, int l, int l1_half, int l0_half)
{
    int idx;
    int len_temp = l - 2 * l1_half;
    int l1 = 2*l1_half;

    for (int i = 0; i < l0_half; i++)
    {
        do
        { 
            idx = 2*(r() % l1_half) + len_temp;
        } while (s[idx] != 0);
        s[idx] = 1;
    }
    for (int i = 0; i < l0_half; i++)
    {
        do
        {
            idx = 2*(r() % l1_half)+1 + len_temp;
        } while (s[idx] != 0);
        s[idx] = -1;
    }

    for (int i = 0; i < HALF_h - l0_half; i++)
    {
        do  
        {
            idx = (r() % len_temp);
        } while (s[idx] != 0);
        s[idx] = 1;
    }
    for (int i = 0; i < HALF_h - l0_half; i++)
    {
        do
        {
            idx = (r() % len_temp);
        } while (s[idx] != 0);
        s[idx] = -1;
    }
} 

void generate_vec_LAC_noise(char *s, int l)
{
    for (int i = 0; i < l; i++)
    {
        s[i] = LAC_noise();
    }
}

void generate_vec_rounding_noise(char *s, int l)
{
    for (int i = 0; i < l; i++)
    {
        s[i] = uniform15();
    }
}

int SumEven(char *s, int l)
{
    int sum = 0;
    for (int i = 0; i < l / 2; i++)
    {
        sum += s[2 * i];
    }
    return sum;
}

int SumOdd(char *s, int l)
{
    int sum = 0;
    for (int i = 0; i < l / 2; i++)
    {
        sum += s[2 * i + 1];
    }
    return sum;
}

int decode(char *s, char *e, record *b, int *err, int *errPosi, int *errPosi_consitioned)
{
    int errorPosition = 0;
    int sum;
    int err_note[ETA_half];

    for (int  i = 0; i < ETA_half; i++)
    {
        err_note[i] = 0;
    }  
     
    for (int i = 0; i < ETA_half; i++)
    {

        sum = b->Round_noise[i] + b->Round_noise[i + ETA_half];
        sum += b->e2[i] + b->e2[i + ETA_half];
        for (int j = 0; j < i + 1; j++)
        {
            sum += e[j] * b->r[i - j];
            sum -= s[j] * b->e1[i - j];
        }
        for (int j = i + 1; j < N; j++)
        {
            sum -= e[j] * b->r[N  + i - j];
            sum += s[j] * b->e1[N  + i - j];
        }

        for (int j = 0; j < i + ETA_half + 1; j++)
        {
            sum += e[j] * b->r[i+ETA_half - j];
            sum -= s[j] * b->e1[i +ETA_half - j];
        }
        for (int j = i +ETA_half+ 1; j < N; j++)
        {
            sum -= e[j] * b->r[N  + i +ETA_half - j];
            sum += s[j] * b->e1[N + i +ETA_half - j];
        } 

        if (sum > Thres)  
        {
            errorPosition += 1;
            errPosi[i] += 1;
            err_note[i] = 1;
        }
        if (sum < -Thres) 
        {
            errorPosition += 1;
            errPosi[i] += 1;
            err_note[i] = 1;
        }
    }

    err[0] += errorPosition;

    for (int i = 1; i < 5; i++)
    {
        if (errorPosition > i)
        {
            err[i]++;
        }
    }
 
    if (errorPosition >= ThresNum)
    {
        for (int i = 0; i < ETA_half; i++)
        {
            errPosi_consitioned[i] += err_note[i];
        }
        
        return 1;   // a decryption error is found.
    }
    return 0;
}

int Lacv2(int counter)
{
    time_t begin = time(NULL);    // randomness initialization;
    char s[N] = {0, 0, -1, -1, 0, 0, 0, -1, 1, 1, -1, 1, 0, 1, -1, 0, 0, 0, -1, 1, 0, 1, -1, 0, 0, 0, -1, 1, 0, 0, -1, 1, 1, 0, 0, 0, -1, 0, -1, 0, 1, 1, -1, 1, 1, 0, -1, 1, 0, -1, -1, 1, 0, 0, 1, -1, 1, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 1, 0, 1, -1, 0, 0, -1, 0, 1, 0, -1, 0, 0, 1, 1, -1, 0, -1, -1, 0, 0, -1, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 1, 0, -1, 1, 1, -1, 1, 0, 1, 0, 0, 0, 0, 1, -1, -1, 1, -1, 0, 0, 0, -1, 1, 0, 1, 1, 0, -1, 0, -1, 1, 0, 0, 0, 1, -1, 0, 0, 0, -1, 1, -1, 1, 1, 0, 1, -1, -1, 0, -1, 0, 0, 1, 0, 1, 0, -1, 1, 1, 0, 0, 0, -1, 0, -1, -1, 1, 0, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 1, 0, 0, -1, 0, -1, 1, 1, 0, 0, 1, -1, 1, -1, 1, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, 1, 0, 0, 1, -1, 0, 0, 0, -1, 1, -1, 0, 0, 1, 0, 0, -1, 0, -1, 0, -1, -1, 0, 1, 1, 1, -1, 0, -1, 0, -1, 0, 0, 0, 0, 1, -1, -1, 0, 0, 0, 0, -1, 0, -1, 1, 0, 1, -1, 1, 0, 0, -1, 0, -1, 0, 0, 0, -1, -1, 0, 0, -1, 1, 1, 0, 1, 0, -1, 1, 1, 0, 1, 1, -1, -1, 0, 0, 0, 1, -1, -1, -1, 1, 0, 1, -1, -1, 1, 1, -1, 0, -1, 1, 1, -1, 0, 1, 0, 0, -1, 1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, 1, -1, 0, 0, 0, -1, -1, -1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, 0, -1, 0, 1, -1, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, 0, 1, 1, 0, -1, 1, 0, -1, 1, 0, 0, 1, 0, -1, 0, 0, 1, 1, -1, 1, 0, 0, -1, 0, -1, 0, -1, 1, -1, -1, 0, 1, 0, 0, 0, 1, 0, 0, -1, 0, 1, 1, 0, 0, 0, -1, -1, 1, 0, 0, 0, 0, -1, 1, 1, 1, -1, 0, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 1, 1, 0, -1, 0, 0, -1, -1, -1, -1, 1, 1, 0, 0, 0, -1, 0, 1, 0, 1, 0, -1, 1, -1, 0, 0, 0, 1, 0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 1, 0, 0, 0, 0, 0, 0, -1, 1, -1, 0, 0, 1, -1, 1, -1, -1, 0, 1, 0, 1, 0, 0, 1, 0, -1, 0, -1, -1, 1, 1, 0, 0, -1, 1, 0, 0, 1, 0, -1, 1, -1, 1, -1, 0, -1, 1, 1, 0, 0, 0, -1, -1, 0, 1, -1, 0, 0, 0, 0, 0, 0, 1, -1, 0, 0, 1, 1, 0, 0, 1, -1, 1, -1, 1, 0, 0, -1, 1, -1, 1, 0, 1, -1, 1, 1, -1, -1, 0, 0, 0, 1, 1, -1, 0, 0, 0, 0, 1, -1, 1, 0, 1, -1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, -1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, -1, 0, 0, 0, 0, 1, 0, -1, 1, 0, 0, 1, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, 0, -1, 0, 1, 0, -1, -1, 0, 0, 0, -1, 1, 0, 0, -1, 0, -1, 0, -1, 1, 1, 0, 0, 1, 0, 1, -1, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, -1, 1, -1, -1, 0, 0, 0, -1, 0, 0, 0, -1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 1, 0, -1, 0, 1, 1, 1, 0, 0, 1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, -1, 1, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, -1, 1, 0, 1, 0, 1, 0, 1, 0, -1, -1, 1, -1, -1, -1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, -1, -1, 1, 1, -1, -1, 0, 0, 0, 1, 0, 1, -1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, -1, 0, 0, 1, -1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 1, -1, 0, -1, 1, -1, 1, 0, 1, -1, 0, 0, 0, 0, 1, -1, 0, -1, 0, -1, -1, 0, 0, 1, 0, -1, 1, -1, -1, 0, -1, 1, 1, 0, 0, -1, 0, -1, 0, 0, 0, -1, 1, 0, 0, 1, 0, -1, 0, 0, 1, 1, 1, 0, -1, 0, 1, 0, 1, -1, 0, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, -1, 0, 1, -1, -1, -1, 0, 1, 0, 0, 1, 0, -1, 0, 0, 0, -1, -1, 1, 1, 1, 1, -1, 0, 1, 0, 0, 1, -1, 1, 1, -1, 1, 0, -1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, -1, -1, 0, 0, 1, -1, 0, 0, 0, 1, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 1, 0, 0, -1, 0, 0, 0, 1, 0, 0, -1, -1, 1, 0, 1, 0, 1, -1, -1, 0, 1, 1, 1, 0, -1, -1, 0};
    char e[N] = {1, 0, -1, 0, 0, 0, 0, 0, -1, 1, -1, -1, 0, 0, 1, -1, 1, 0, 0, -1, 1, 1, 0, -1, 0, 0, 1, 0, 0, 0, -1, 0, -1, -1, 1, -1, 1, 1, 1, -1, 0, 0, 0, -1, 1, 0, 1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 0, -1, 1, 1, 0, 1, 0, 0, 0, -1, 0, -1, 1, -1, 1, 0, -1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, -1, 0, 0, -1, 0, 1, 0, 0, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, 0, 0, 1, 1, 0, -1, -1, 0, 0, -1, -1, 0, 0, 1, 0, -1, 0, 1, 0, 0, 1, -1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, -1, 1, -1, 0, -1, 1, 0, 1, -1, -1, 0, 0, 1, -1, -1, 1, 0, 1, 1, -1, 0, 0, 1, 0, 0, 0, 0, 1, -1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, -1, -1, 1, 0, 1, 0, 0, 0, -1, 0, 1, 0, 0, 0, -1, 0, 0, 1, -1, 0, -1, 1, 1, 0, 0, -1, 1, -1, 0, 1, 0, 1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, -1, 0, 1, 0, 0, 0, 0, -1, 1, 0, 0, -1, 0, -1, 0, 0, 1, -1, -1, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, -1, 1, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, -1, 1, 0, 0, -1, 0, 0, -1, -1, -1, 1, 0, 0, 0, 1, 1, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, -1, 1, 0, 1, -1, -1, 1, 0, -1, 0, -1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, -1, -1, 1, -1, 1, 0, 1, 1, 1, 0, 1, 1, 0, -1, 1, -1, 0, 1, 0, 1, -1, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 1, -1, 1, 1, 0, 0, 1, 1, 1, -1, 1, 0, 0, -1, 0, 0, 0, 0, 1, 1, -1, 0, 0, 1, -1, 1, 0, 0, 0, -1, 1, 1, 1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, -1, -1, -1, 0, 0, 0, 1, -1, 1, 0, 0, 0, 1, 0, -1, 1, 0, 0, 1, -1, -1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, -1, 1, 0, -1, 0, 0, 0, 1, 1, 0, 1, -1, 1, 0, -1, 1, 0, -1, -1, -1, 0, 1, 0, 0, 0, 0, -1, 0, 0, 1, 1, -1, -1, -1, -1, 1, 0, 0, 1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0, -1, -1, 1, -1, 0, -1, -1, 1, 0, -1, 0, -1, 0, 1, 1, -1, 0, 0, 1, -1, 0, 0, 1, 1, 0, 1, 0, 0, 0, -1, -1, 1, 0, 1, 0, 1, 0, 1, -1, 0, -1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, -1, 0, -1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 1, -1, 1, 0, -1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 1, 0, 0, -1, 0, 0, -1, -1, 1, 0, -1, -1, 0, 0, -1, 1, 1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 1, 1, 0, 0, -1, 1, -1, 0, 1, 0, 0, 1, 1, 0, 1, -1, 1, -1, -1, -1, 1, 0, 0, 0, -1, 1, 0, -1, 0, 0, 0, 0, 1, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, 1, 0, 0, 1, -1, 0, 0, 0, 1, 1, 0, -1, 0, -1, 1, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, -1, 1, -1, 1, 0, -1, -1, 0, 0, 0, -1, 0, 0, 1, 0, 1, -1, 0, 0, 1, 0, 0, -1, 1, 1, -1, 0, 0, -1, -1, 0, 0, 1, 1, 1, 0, 1, 0, 0, -1, 0, 1, 0, 1, 0, 0, -1, -1, 0, 1, 1, 0, 0, 0, 1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 1, 1, -1, -1, 1, 1, 0, -1, 0, -1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, -1, -1, -1, 0, -1, -1, -1, 0, 1, -1, -1, 0, 0, -1, -1, 0, 0, 1, 0, -1, -1, -1, 0, 0, -1, 0, 1, 0, 1, -1, 0, -1, 1, 1, -1, -1, 0, 0, 1, 1, -1, 0, 0, 0, 1, -1, 0, -1, -1, 0, -1, 0, 1, 0, 1, 1, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 1, 0, 1, -1, -1, -1, -1, -1, 1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 1, -1, 0, 0, 0, 1, 1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, -1, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, -1, -1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, -1, 1, 0, 0, 1, -1, 0, -1, -1, 0, -1, -1, 0, 1, -1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, 1, 1, -1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, -1, 0, 0, 1, 1, 0};
    long errorNum = 0;
    double errorRate = 0.0;
    int err[6];
    int noise_type = NOISE_type;
    int errPosi[ETA_half];
    int errPosi_conditioned[ETA_half];
    double errPositionRate;
    for (int i = 0; i < 6; i++)
    {
        err[i] = 0; 
    }

    for (int i = 0; i < ETA_half; i++)
    {
        errPosi[i] = 0;
        errPosi_conditioned[i] = 0;
    }
 
	char noise_name[40];
	char result_name[40];
	sprintf(noise_name, "noise%03d.txt", counter);
	sprintf(result_name, "result%03d.txt", counter);

    FILE *file_noise = fopen(noise_name, "w");
    FILE *file_result = fopen(result_name, "w");

    // generate secret;
    srand(time(NULL));
    rc4_setup();

    printf("%d\n", TESTNUM);
    printf("start\n"); 
    for (int i = 0; i < TESTNUM; i++)
    {
        record *noi = (record *)calloc(1, sizeof(record));
        if (noise_type == 1)
        {
            generate_bias_nosie(noi->e1, N, DIFF_noise);
        }
        else  
        {
            generate_bias_nosie_typeII(noi->e1, N, LENGTH_noise_typeII, DIFF_noise_typeII);
        }  
        generate_sparse_secret(noi->r, h / 2, h / 2, N);
        generate_vec_LAC_noise(noi->e2, N);
        generate_vec_rounding_noise(noi->Round_noise, ETA);

        if (decode(s, e, noi, err, errPosi, errPosi_conditioned) == 1)
        {
            errorNum += 1;
            record tmp;
            memcpy(tmp.e1, noi->e1, SIZE_NOISE);
            memcpy(tmp.r, noi->r, SIZE_NOISE);
            memcpy(tmp.e2, noi->e2, SIZE_NOISE);
            memcpy(tmp.Round_noise, noi->Round_noise, SIZE_ETA);
            fwrite (&tmp, sizeof(record), 1, file_noise);
        }
        free(noi);
        if (((i + 1) % 500000) == 0)
        {
            printf("Have done (%d ) times\n", (i+1));
            printf("The number of error is %ld\n", errorNum);
            printVector(errPosi, ETA_half);
            printf("\n");
            errPositionRate = (double)err[0] / (double)(i + 1) / (double)ETA_half;
            printf("After %d iterations, the error rate of one position is %.10lf\n", i + 1, errPositionRate);
            printVector(err, 4);
            printf("\n");
            printVector(errPosi_conditioned, ETA_half);

        }
    }

    errPositionRate = (double)err[0] / (double)TESTNUM / (double)ETA_half;
    printf("The error rate of one position is %lf\n", errPositionRate);
    errorRate = (double)errorNum / (double)TESTNUM;
    if (errorNum == 0)
    {
        printf("No errors.\n");
    }
    else
    {
        printf("The number of error is %ld\n", errorNum);
        printf("The error rate is %lf\n", errorRate);
    }

    fprintf(file_result, "Have done %d tests\n", TESTNUM);
    fprintf(file_result, "number of errors: %ld \n", errorNum);
    fprintf(file_result, "error rate: %f \n", errorRate);
    for (int temp_i = 0; temp_i < ETA_half; temp_i++)
    {
        fprintf(file_result,"%d,", errPosi_conditioned[temp_i]);
    }
    time_t end = time(NULL);
    printf("Running time is %d seconds\n", (end-begin));
    printf("Have done (%d ) times\n", (TESTNUM));
    printVector(errPosi, ETA_half);
    printf("\n");
    printVector(err, 5); 
    printf("\n");
    printVector(errPosi_conditioned, ETA_half);
    fclose (file_noise);
    fclose (file_result);
    return 0;
}
