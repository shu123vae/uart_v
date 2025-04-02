#include "Conv.h"


/*
多进多出卷积函数 | U8进U8出:
m张图，n个卷积核，依次使用卷积核对所有图像进行卷积，得出m个输出，再对m个输出的对应位置的数据
进行求和，作为该函数的输出。同理，n个卷积核对应n个函数的输出。

InImg：		输入图像数据流
ResStreram：输出滤波图像数据流
*/
void ConvU8(INPUT TypeU8Stream InImgStream[IMG_NUM], OUTPUT TypeU8Stream ResStreram[FILTER_NUM])
{
	int i, j, k, r;
	int bufLineI = 0;
	//int strideI = 0, strideJ = 0;
	int strideI = STRIDE_H-PADS_SIZE, strideJ = STRIDE_W-PADS_SIZE;

	static U16 BufLine[FILTER_SIZE - 1][IMG_SIZE] = { (U16)0 };
	static U16 BufBlock[FILTER_NUM][FILTER_SIZE - 1] = { (U16)0 };

	U8	tempU8[IMG_NUM], tempResU8;
	U16 inValSum, tempVal, tempResU16 ;


	for (i = 0; i < IMG_H_EXP; i++)
	{
		strideJ = STRIDE_W - PADS_SIZE;

		ConvU8_label5:for (j = 0; j < IMG_W_EXP; j++)
		{
			//取数
			if (i < IMG_SIZE && j < IMG_SIZE)
			{
				inValSum = (U16)0;
				ConvU8_label6:for (k = 0; k < IMG_NUM; k++)
				{
					InImgStream[k] >> tempU8[k];
					inValSum += (U16)tempU8[k];
				}
			}
			else
			{
				inValSum = (U16)0;
			}

			//计算
			ConvU8_label7:for (k = 0; k < FILTER_NUM; k++)
			{
				CalBaseCol(BufLine, bufLineI, j, inValSum, Kernel[k], FILTER_SIZE - 1, tempResU16);
				tempResU16 += BufBlock[k][FILTER_SIZE - 2];

				for (r = FILTER_SIZE - 2; r >= 1; r--)
				{
					CalBaseCol(BufLine, bufLineI, j, inValSum, Kernel[k], r, tempVal);
					BufBlock[k][r] = BufBlock[k][r - 1] + tempVal;
				}

				CalBaseCol(BufLine, bufLineI, j, inValSum, Kernel[k], 0, tempVal);
				BufBlock[k][0] = tempVal;


				/*The result output range*/
				if (i >= PADS_SIZE && j >= PADS_SIZE)
				{
					if (strideI == 0 && strideJ == 0)
					{
						tempResU8 = TransU16ToU8(tempResU16);

						ResStreram[k] << tempResU8;
					}
				}

			}//for...k


			/*Fill the current value into BufLine*/
			if (j < IMG_SIZE)
			{
				BufLine[bufLineI][j] = inValSum;
			}

			if (j == IMG_W_EXP - 1)
			{
				if (bufLineI == FILTER_SIZE - 2)
				{
					bufLineI = 0;
				}
				else
				{
					bufLineI++;
				}
			}

			if (strideJ == STRIDE_W - 1)
			{
				strideJ = 0;
			}
			else
			{
				strideJ++;
			}

		}//for...j

		if (strideI == STRIDE_H - 1)
		{
			strideI = 0;
		}
		else
		{
			strideI++;
		}

	}//for...i
}

/*
功能函数：
将原图某列j数据以及当前值,与卷积核指定列Kcol进行乘积求和。原图数据BufLine中从行号bufLineI开始，
卷积核Kernel中从行号0开始。

BufLine		保存图像数据
bufLineI	BufLine中的起始行，该位置也是列数据中最古老的值
j			当前列号
inVal		当前输入值
Kernel		卷积核
kcol		卷积核指定列
resVal		结果，当前列的计算结果
*/
void CalBaseCol(INPUT const U16 BufLine[FILTER_SIZE - 1][IMG_SIZE], INPUT int bufLineI, INPUT int j, INPUT U16 inVal,
	INPUT const U8 Kernel[FILTER_SIZE][FILTER_SIZE], INPUT int kcol, OUTPUT U16 &resVal)
{
	if(j < IMG_SIZE)
	{
		int k;
		U16	temVal = (U16)0;

		int m = bufLineI;
		CalBaseCol_label4:for (k = 0; k < FILTER_SIZE - 1; k++)
		{
			temVal += (U16)BufLine[m][j] * Kernel[k][kcol];

			if (m == FILTER_SIZE - 2)
			{
				m = 0;
			}
			else
			{
				m++;
			}
		}

		temVal += (U16)inVal* Kernel[FILTER_SIZE - 1][kcol];

		resVal = temVal;
	}
	else
	{
		resVal = (U16)0;
	}
}


/*
变换函数（模拟激活层）
将输入限定在最大值和最小值之间
*/
U8 TransU16ToU8(INPUT U16 inVal)
{
	U8 res;

	if(inVal < MINVAL)
	{
		res = (U8)MINVAL;
	}
	else if(inVal > MAXVAL)
	{
		res = (U8)MAXVAL;
	}
	else
	{
		res = (U8)inVal;
	}
	return res;
}
