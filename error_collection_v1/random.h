#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define N 1024
#define Q 251
#define threshold_one_position 62
#define threshold_one_error 56
#define insert_ones 65 // the number of ones inserted.

u8 rc4_s[256];
u32 a11, b11;

void rc4_setup()
{
	int i,k;
	u8 rc4_key[16], temp;

	for(i=0; i<16; i++)rc4_key[i] = 0x00;
	for(i=0; i<16; i++)rc4_key[i] = ( ( ( rand() + rand() ) << 16 ) + ( rand() - rand() ) ) & 0xff;

	for(a11=0; a11<256; a11++)rc4_s[a11] = (u8)a11;

	b11 = 0;
    for(a11=0; a11<256; a11++)
	{
		b11 = (b11 + rc4_s[a11] + rc4_key[a11 & 0xf]) & 0xff;
  		temp = rc4_s[a11];
	    rc4_s[a11] = rc4_s[b11];
	    rc4_s[b11] = temp;

	}

	a11 = 0;
	b11 = 0;

	for(k=0; k<8192; k++)
	{
		a11 = ( a11 + 1 ) & 0xff;
		b11 = ( b11 + rc4_s[a11] ) & 0xff;
		temp = rc4_s[a11];
	    rc4_s[a11] = rc4_s[b11];
	    rc4_s[b11] = temp;
	}
}

u8 rc4( )
{
	u8 temp;

	a11 = ( a11 + 1 ) & 0xff;
	b11 = ( b11 + rc4_s[a11] ) & 0xff;
	temp = rc4_s[a11];
	rc4_s[a11] = rc4_s[b11];
	rc4_s[b11] = temp;
	return( rc4_s[( rc4_s[a11] + rc4_s[b11] ) & 0xff ] );

}

void randomIV(u8 *iv, u32 n)
{
	u32 i;
	for(i=0; i<n; i++)iv[i]= rc4() & 0x01;
}


int intcmp(const void *v1, const void *v2)
{
	return (*(double *)v1 - *(double *)v2);
}

int intcmp1(const void *v1, const void *v2)
{
	return (*(float *)v1 - *(float *)v2);
}

int intcmp2(const void *v1, const void *v2)
 {
     return (*(int *)v1 - *(int *)v2);
}


char lac_noise()
{
    int temp;
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


void generate_noise_vector(char *a)
{
    for (int i = 0; i < N; i++)
    {
        a[i] = lac_noise();
    }
}

void generate_controlled_noise_vector(char *a)
{
    for (int i = 0; i < insert_ones; i++)
    {
        a[i] = 1;
    }

    for (int i = insert_ones; i < N; i++)
    {
        a[i] = lac_noise();
    }
}

    void print(char *a)
    {
        for (int i = 0; i < N; i++)
        {
            printf("%c, ", a[i]);
        }
    }

int sum(int *a)
{
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += a[i];
    }
    return sum;
}
void initial_zero(int *a)
{
    for (int i = 0; i < N; i++)
    {
        a[i] = 0;
    }
}

int decode(char *s, char *e, char *e1, char *r, char *e2, long *error_posi)
{
    int a[N];
    int error_num;

    initial_zero(a);
    for (int i = 0; i < N; i++)
    {
        char err_one_position = e2[i];
        for (int j = 0; j < i + 1; j++)
        {
            err_one_position += (s[j] * e1[i - j] - e[j] * r[i - j]);
        }
        for (int j = i + 1; j < N; j++)
        {
            err_one_position -= (s[j] * e1[N + i - j] - e[j] * r[N + i - j]);
        }
        if (err_one_position > threshold_one_position)
        {
            a[i] = 1;
        }
        if (err_one_position < -threshold_one_position)
        {
            a[i] = 1;
        }
    }
    error_num = sum(a);

    if (error_num >= threshold_one_error)
    {
        for (int i = 0; i < N; i++)
        {
            error_posi[i] += a[i];
        }
        return 1;
    }
    else
        return 0;
}






