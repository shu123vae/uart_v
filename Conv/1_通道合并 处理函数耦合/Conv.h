#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <stdio.h>
#include <string.h>

//#define __TEST__

#define	INPUT
#define OUTPUT


#define NUM_3	3
#define	NUM_4	4
#define	NUM_5	5


#define FILTER_SIZE	3
#define PADS_SIZE	((FILTER_SIZE-1)/2)	//the padding size on one side 

#define IMG_SIZE	512
#define IMG_H_EXP	(IMG_SIZE+PADS_SIZE)
#define IMG_W_EXP	(IMG_SIZE+PADS_SIZE)

#define STRIDE_H	1		//filter step along the col
#define STRIDE_W	1		//filter step along the row

#define	MAXVAL	999999
#define MINVAL	-999999


typedef	ap_int<8>		S8;
typedef	ap_int<16>		S16;

template<unsigned int Num>
struct TypeS8
{
	S8	M[Num];
};


typedef	hls::stream<S8>		TypeS8Stream;
typedef	hls::stream<S16>	TypeS16Stream;
typedef	hls::stream<float>	TypeFloatStream;

typedef	TypeS8<NUM_4>	TypeA;
typedef	hls::stream<TypeA>	StreamTypeA;

typedef	TypeS8<NUM_4>	TypeB;
typedef	hls::stream<TypeB>	StreamTypeB;

typedef	TypeS8<NUM_5>	TypeC;
typedef	hls::stream<TypeC>	StreamTypeC;


/*
º¯Êý¶¨Òå
*/
void ProceFun(INPUT StreamTypeA &InImgStream, OUTPUT StreamTypeB &ResStreram);

void ConvA2C(INPUT StreamTypeA &InStream, OUTPUT StreamTypeC &OutStream);

void ConvC2B(INPUT StreamTypeC &InStream, OUTPUT StreamTypeB &OutStream);

void CalBaseCol(INPUT const S16 BufLine[FILTER_SIZE - 1][IMG_SIZE], INPUT int bufLineI, INPUT int j, INPUT S16 inVal,
	INPUT const S8 Kernel[FILTER_SIZE][FILTER_SIZE], INPUT int kcol, OUTPUT S16 &resVal);

void TransA(INPUT S16 inVal, OUTPUT S8 &res);

void TransB(INPUT S16 inVal, OUTPUT S16 &res);

template<class Type, int M,int N>
inline void WatchArray(Type InArr[M][N])
{
	float Temp[M][N];
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			Temp[i][j] = (float)InArr[i][j];
		}
	}
}
