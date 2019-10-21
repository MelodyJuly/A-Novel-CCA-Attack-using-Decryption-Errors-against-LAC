#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

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




