#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>
#include <stdio.h>
#include <string.h>

//#define __TEST__

#define	INPUT
#define OUTPUT


#define	FILTER_NUM	3		//It is also the Output image numbers
#define FILTER_SIZE	3
#define PADS_SIZE	((FILTER_SIZE-1)/2)	//the padding size on one side 

#define	IMG_NUM		3		//Input image numbers
#define IMG_SIZE	512
#define IMG_H_EXP	(IMG_SIZE+PADS_SIZE)
#define IMG_W_EXP	(IMG_SIZE+PADS_SIZE)

#define STRIDE_H	1		//filter step along the col
#define STRIDE_W	1		//filter step along the row


#define	MAXVAL	255
#define MINVAL	0


typedef	ap_uint<8>		U8;
typedef	ap_uint<16>		U16;

typedef	hls::stream<U8>		TypeU8Stream;
typedef	hls::stream<U16>	TypeU16Stream;
typedef	hls::stream<float>	TypeFloatStream;


/*
卷积核
*/
const U8 Kernel[FILTER_NUM][FILTER_SIZE][FILTER_SIZE] = {

//	{ { 1, 1, 1 },
//	{ 1, 1, 1 },
//	{ 1, 1, 1 } },
//
//	{ { 1, 2, 1 },
//	{ 2, 2, 2 },
//	{ 1, 2, 1 } },
//
//	{ { 1, 0, 1 },
//	{ 0, 2, 0 },
//	{ 1, 0, 1 } } ,
//
//	{ { 1, 1, 1 },
//	{ 1, 1, 1 },
//	{ 1, 1, 1 } },
//
//	{ { 1, 2, 1 },
//	{ 2, 2, 2 },
//	{ 1, 2, 1 } },
//
//	{ { 1, 0, 1 },
//	{ 0, 2, 0 },
//	{ 1, 0, 1 } },
//
//	{ { 1, 1, 1 },
//	{ 1, 1, 1 },
//	{ 1, 1, 1 } },
 
 	{ { 1, 2, 1 },
 	{ 2, 2, 2 },
 	{ 1, 2, 1 } },
 
 	{ { 1, 0, 1 },
 	{ 0, 2, 0 },
 	{ 1, 0, 1 } },

	{ { 1, 1, 1 },
	{ 1, 1, 1 },
	{ 1, 1, 1 } }

};




/*
函数定义
*/
void ConvU8(INPUT TypeU8Stream InImgStream[IMG_NUM], OUTPUT TypeU8Stream ResStreram[FILTER_NUM]);

void CalBaseCol(INPUT const U16 BufLine[FILTER_SIZE - 1][IMG_SIZE], INPUT int bufLineI, INPUT int j, INPUT U16 inVal,
	INPUT const U8 Kernel[FILTER_SIZE][FILTER_SIZE], INPUT int kcol, OUTPUT U16 &resVal);

U8 TransU16ToU8(INPUT U16 inVal);

