#include "Conv.h"


/*
主函数
*/
void ProceFun(INPUT StreamTypeA &InImgStream, OUTPUT StreamTypeB &ResStreram)
{
	static StreamTypeC TempStreamC;

	ConvA2C(InImgStream, TempStreamC);

	ConvC2B(TempStreamC, ResStreram);
}


const S8 KernelA[NUM_5][FILTER_SIZE][FILTER_SIZE] = {
	//{ { (S8)1, (S8)-1, (S8)1 },
	//{ (S8)-1, (S8)1, (S8)-1 },
	//{ (S8)1, (S8)-1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)2, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } }
};

//3,5
void ConvA2C(INPUT StreamTypeA &InStream, OUTPUT StreamTypeC &OutStream)
{
	int i, j, k, r;
	int strideI = STRIDE_H - PADS_SIZE, strideJ = STRIDE_W - PADS_SIZE;

	static int bufLineI = 0;
	static S16 BufLine[FILTER_SIZE - 1][IMG_SIZE] = { (S16)0 };
	static S16 BufBlock[NUM_5][FILTER_SIZE - 1] = { (S16)0 };

	S8	tempRes8;
	S16 inSum, tempValA, tempValB, tempRes16;

	TypeA	tempTypeIn;
	TypeC	tempTypeRes;

	for (i = 0; i < IMG_H_EXP; i++)
	{
		strideJ = STRIDE_W - PADS_SIZE;

		ConvA2C_label0:for (j = 0; j < IMG_W_EXP; j++)
		{
			if (i < IMG_SIZE && j < IMG_SIZE)
			{
				InStream >> tempTypeIn;

				inSum = (S16)0;
				ConvA2C_label1:for (k = 0; k < NUM_3; k++)
				{
					inSum += (S16)tempTypeIn.M[k];
				}
			}
			else
			{
				inSum = (S16)0;
			}

			ConvA2C_label2:for (k = 0; k < NUM_5; k++)
			{
				CalBaseCol(BufLine, bufLineI, j, inSum, KernelA[k], FILTER_SIZE - 1, tempRes16);
				tempRes16 += BufBlock[k][FILTER_SIZE - 2];

				ConvA2C_label6:for (r = FILTER_SIZE - 2; r >= 1; r--)
				{
					CalBaseCol(BufLine, bufLineI, j, inSum, KernelA[k], r, tempValA);
					BufBlock[k][r] = BufBlock[k][r - 1] + tempValA;
				}

				CalBaseCol(BufLine, bufLineI, j, inSum, KernelA[k], 0, tempValB);
				BufBlock[k][0] = tempValB;

				TransA(tempRes16, tempRes8);
				tempTypeRes.M[k] = tempRes8;

			}//for...k

			/*The result output range*/
			if (i >= PADS_SIZE && j >= PADS_SIZE)
			{
				if (strideI == 0 && strideJ == 0)
				{
					OutStream << tempTypeRes;
				}
			}

			/*Fill the current value into BufLine*/
			if (j < IMG_SIZE)
			{
				BufLine[bufLineI][j] = inSum;
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



const S8 KernelB[NUM_4][FILTER_SIZE][FILTER_SIZE] =
{
	//{ { (S8)1, (S8)-1, (S8)1 },
	//{ (S8)-1, (S8)1, (S8)-1 },
	//{ (S8)1, (S8)-1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)2, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } },

	{ { (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 },
	{ (S8)1, (S8)1, (S8)1 } }
};

//5,4
void ConvC2B(INPUT StreamTypeC &InStream, OUTPUT StreamTypeB &OutStream)
{
	int i, j, k, r;
	int strideI = STRIDE_H - PADS_SIZE, strideJ = STRIDE_W - PADS_SIZE;
	
	static int bufLineI = 0;
	static S16 BufLine[FILTER_SIZE - 1][IMG_SIZE] = { (S16)0 };
	static S16 BufBlock[NUM_4][FILTER_SIZE - 1] = { (S16)0 };

	S8	tempRes8;
	S16 inSum, tempValA, tempValB, tempRes16;

	TypeC	tempTypeIn;
	TypeB	tempTypeRes;

	for (i = 0; i < IMG_H_EXP; i++)
	{
		strideJ = STRIDE_W - PADS_SIZE;

		ConvC2B_label3:for (j = 0; j < IMG_W_EXP; j++)
		{
			if (i < IMG_SIZE && j < IMG_SIZE)
			{
				InStream >> tempTypeIn;

				inSum = (S16)0;
				ConvC2B_label4:for (k = 0; k < NUM_5; k++)
				{
					inSum += (S16)tempTypeIn.M[k];
				}
			}
			else
			{
				inSum = (S16)0;
			}

			ConvC2B_label5:for (k = 0; k < NUM_4; k++)
			{
				CalBaseCol(BufLine, bufLineI, j, inSum, KernelB[k], FILTER_SIZE - 1, tempRes16);
				tempRes16 += BufBlock[k][FILTER_SIZE - 2];

				ConvC2B_label0:for (r = FILTER_SIZE - 2; r >= 1; r--)
				{
					CalBaseCol(BufLine, bufLineI, j, inSum, KernelB[k], r, tempValA);
					BufBlock[k][r] = BufBlock[k][r - 1] + tempValA;
				}

				CalBaseCol(BufLine, bufLineI, j, inSum, KernelB[k], 0, tempValB);
				BufBlock[k][0] = tempValB;

				TransA(tempRes16, tempRes8);
				tempTypeRes.M[k] = tempRes8;

			}//for...k

			/*The result output range*/
			if (i >= PADS_SIZE && j >= PADS_SIZE)
			{
				if (strideI == 0 && strideJ == 0)
				{
					OutStream << tempTypeRes;
				}
			}

			/*Fill the current value into BufLine*/
			if (j < IMG_SIZE)
			{
				BufLine[bufLineI][j] = inSum;
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



void CalBaseCol(INPUT const S16 BufLine[FILTER_SIZE - 1][IMG_SIZE], INPUT int bufLineI, INPUT int j, INPUT S16 inVal,
	INPUT const S8 Kernel[FILTER_SIZE][FILTER_SIZE], INPUT int kcol, OUTPUT S16 &resVal)
{
	if(j < IMG_SIZE)
	{
		int k;
		S16	temVal = (S16)0;

		int m = bufLineI;
		CalBaseCol_label4:for (k = 0; k < FILTER_SIZE - 1; k++)
		{
			temVal += (S16)BufLine[m][j] * Kernel[k][kcol];

			if (m == FILTER_SIZE - 2)
			{
				m = 0;
			}
			else
			{
				m++;
			}
		}

		temVal += (S16)inVal* Kernel[FILTER_SIZE - 1][kcol];

		resVal = temVal;
	}
	else
	{
		resVal = (S16)0;
	}
}


/*
变换函数（模拟激活层）
将输入限定在最大值和最小值之间
*/
void TransA(INPUT S16 inVal,OUTPUT S8 &res)
{
	if(inVal < MINVAL)
	{
		res = (S8)MINVAL;
	}
	else if(inVal > MAXVAL)
	{
		res = (S8)MAXVAL;
	}
	else
	{
		res = (S8)inVal;
	}
}


void TransB(INPUT S16 inVal, OUTPUT S16 &res)
{
	if (inVal < MINVAL)
	{
		res = (S16)MINVAL;
	}
	else if (inVal > MAXVAL)
	{
		res = (S16)MAXVAL;
	}
	else
	{
		res = (S16)inVal;
	}
}
