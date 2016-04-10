#include "fcorr.h"

int A[4000];

FCorr::FCorr()
{
	Uint16 i;
	for(i=0;i<4000;i++)
	{
		A[i] = i*i;
	}
}
FCorr::~FCorr()
{
}
float FCorr::Corr(float *Out,Uint16 *imax,Uint16 *jmax,Uint8 *searchWin,Uint8 *objWin8b,Uint16 SWwidth,
		Uint16 SWheight,Uint16 SWpitch,Uint16 OBJwidth, Uint16 OBJheight,Uint16 OBJpitch)
{
	Uint16 i,j,i1,j1,n;
	Uint64 sumBB,sumAA,sumAB,sumB2,sumA2;
	Uint8 devide_counter = 0;
	Uint64 temp1,temp2;
	Uint16 objWin[MAX_OBJ_SIZE];

	Uint32 sumB,sumA;
	Uint32 sqrtB;
	float max = 0;

	Uint32 *sumMatA;
	Uint64 *sumMatAA;
	Uint16 *bb;

	sumMatA = new Uint32[MAX_SW_SIZE];
	sumMatAA = new Uint64[MAX_SW_SIZE];
	bb = new Uint16[MAX_OBJ_SIZE];

	////////////////////////////////////////////// Change ObjWin to 16 bit data
	for (j1=0;j1<OBJheight;j1++)
	{
		for (i1=0;i1<OBJheight;i1++)
		{
			objWin[i1+j1*OBJpitch] = objWin8b[i1+j1*OBJpitch];
		}
	}
	/////////////////////////////////////////////// Calculate number of data bytes
	n = OBJwidth*OBJheight;
	////////////////////////////////////////////// Create and calculate Integral matrix of A,A^2
	for (j1=0;j1<SWheight;j1++)
	{
		for (i1=0;i1<SWheight;i1++)
		{
			if (i1>=1 && j1>=1)
			{
				sumMatA[i1+j1*SWpitch] = - sumMatA[(i1-1)+(j1-1)*SWpitch]
												 + sumMatA[(i1-1)+(j1)*SWpitch]
														   + sumMatA[(i1)+(j1-1)*SWpitch]
																	 + searchWin[i1+j1*SWpitch];

				sumMatAA[i1+j1*SWpitch] = - sumMatAA[(i1-1)+(j1-1)*SWpitch]
												   + sumMatAA[(i1-1)+(j1)*SWpitch]
															  + sumMatAA[(i1)+(j1-1)*SWpitch]
																		 + (searchWin[i1+j1*SWpitch]*searchWin[i1+j1*SWpitch]);
			}
			else if (i1>=1)
			{
				sumMatA[i1+j1*SWpitch] = searchWin[i1+j1*SWpitch] + sumMatA[(i1-1)+(j1)*SWpitch];

				sumMatAA[i1+j1*SWpitch] = (searchWin[i1+j1*SWpitch]*searchWin[i1+j1*SWpitch]) + sumMatAA[(i1-1)+(j1)*SWpitch];
			}
			else if (j1>=1)
			{
				sumMatA[i1+j1*SWpitch] = searchWin[i1+j1*SWpitch] + sumMatA[i1+(j1-1)*SWpitch];

				sumMatAA[i1+j1*SWpitch] = (searchWin[i1+j1*SWpitch]*searchWin[i1+j1*SWpitch]) + sumMatAA[i1+(j1-1)*SWpitch];
			}
			else
			{
				sumMatA[i1+j1*SWpitch] = searchWin[i1+j1*SWpitch];

				sumMatAA[i1+j1*SWpitch] = (searchWin[i1+j1*SWpitch]*searchWin[i1+j1*SWpitch]);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	sumB = 0;
	for (j1=0;j1<OBJheight;j1++)
	{
		for (i1=0;i1<OBJwidth;i1++)
		{
			sumB += objWin[i1+j1*OBJpitch];
		}
	}
	for (j = 0; j < OBJheight; j++)
	{
		for (i = 0; i < OBJwidth; i++)
		{
			bb[i+j*OBJpitch] = objWin[i+j*OBJpitch] * objWin[i+j*OBJpitch];
		}
	}
	sumBB = 0;
	for (j = 0; j < OBJheight; j++)
	{
		for (i = 0; i < OBJwidth; i++)
		{
			sumBB += bb[i+j*OBJpitch];
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	sumB2 = (Uint64)sumB*(Uint64)sumB;                                                                      //////////////// Calculate (Ey)^2
	/////////////////////////////////////////////////////////////////////////////////////////


	temp1 = n*sumBB;
	temp2 = temp1 - sumB2;

	while (temp2 > 10000000)
	{
		temp2/=10000;
		devide_counter++;
	}
	sqrtB = findsqrt(temp2);
	while (devide_counter != 0)
	{
		sqrtB*=100;
		devide_counter--;
	}

	for (j=0;j<=(SWheight-OBJheight);j+=2)
	{
		for (i=0;i<=(SWwidth-OBJwidth);i+=2)
		{
			if (i>=1 && j>=1)
			{
				sumA = sumMatA[(i-1)+(j-1)*SWpitch]
							   - sumMatA[(i-1)+(j+OBJheight-1)*SWpitch]
										 - sumMatA[(i+OBJwidth-1)+(j-1)*SWpitch]
												   + sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch];

				sumAA = sumMatAA[(i-1)+(j-1)*SWpitch]
							   - sumMatAA[(i-1)+(j+OBJheight-1)*SWpitch]
										 - sumMatAA[(i+OBJwidth-1)+(j-1)*SWpitch]
												   + sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch];
			}
			else if (i>=1)
			{
				sumA = sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
							   - sumMatA[(i-1)+(j+OBJheight-1)*SWpitch];

				sumAA = sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
								 - sumMatAA[(i-1)+(j+OBJheight-1)*SWpitch];
			}
			else if (j>=1)
			{
				sumA = sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
							   - sumMatA[i+OBJwidth-1+(j-1)*SWpitch];

				sumAA = sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
								 - sumMatAA[i+OBJwidth-1+(j-1)*SWpitch];
			}
			else
			{
				sumA = sumMatA[OBJwidth-1+(OBJheight-1)*SWpitch];

				sumAA = sumMatAA[OBJwidth-1+(OBJheight-1)*SWpitch];
			}

			sumA2 = (Uint64)sumA*(Uint64)sumA;                                                          /////////////////////////// Calculate (Ex)^2
			sumAB = 0;

			for (j1 = j; j1 < (OBJheight+j); j1++)
			{
				for (i1 = i; i1 < (OBJwidth+i); i1++)
				{
					sumAB += searchWin[i1+j1*SWpitch] * objWin[i1-i+(j1-j)*OBJpitch];
				}
			}

			Out[i+j*SWpitch] = calculateCorr(sumBB,sumAA,sumAB,sumA,sumB,sumA2,sumB2,n,sqrtB);
			if (Out[i+j*SWpitch] > max)
			{
				max = Out[i+j*SWpitch];
				*imax = i;
				*jmax = j;
			}
		}
	}
/*
	for (j=(*jmax-5);j<=(*jmax+5);j+=2)
		{
			for (i=(*imax-5);i<=(*imax+5);i+=2)
			{
				if (i>=1 && j>=1)
				{
					sumA = sumMatA[(i-1)+(j-1)*SWpitch]
								   - sumMatA[(i-1)+(j+OBJheight-1)*SWpitch]
											 - sumMatA[(i+OBJwidth-1)+(j-1)*SWpitch]
													   + sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch];

					sumAA = sumMatAA[(i-1)+(j-1)*SWpitch]
								   - sumMatAA[(i-1)+(j+OBJheight-1)*SWpitch]
											 - sumMatAA[(i+OBJwidth-1)+(j-1)*SWpitch]
													   + sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch];
				}
				else if (i>=1)
				{
					sumA = sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
								   - sumMatA[(i-1)+(j+OBJheight-1)*SWpitch];

					sumAA = sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
									 - sumMatAA[(i-1)+(j+OBJheight-1)*SWpitch];
				}
				else if (j>=1)
				{
					sumA = sumMatA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
								   - sumMatA[i+OBJwidth-1+(j-1)*SWpitch];

					sumAA = sumMatAA[(i+OBJwidth-1)+(j+OBJheight-1)*SWpitch]
									 - sumMatAA[i+OBJwidth-1+(j-1)*SWpitch];
				}
				else
				{
					sumA = sumMatA[OBJwidth-1+(OBJheight-1)*SWpitch];

					sumAA = sumMatAA[OBJwidth-1+(OBJheight-1)*SWpitch];
				}

				sumA2 = (Uint64)sumA*(Uint64)sumA;                                                          /////////////////////////// Calculate (Ex)^2
				sumAB = 0;

				for (j1 = j; j1 < (OBJheight+j); j1++)
				{
					for (i1 = i; i1 < (OBJwidth+i); i1++)
					{
						sumAB += searchWin[i1+j1*SWpitch] * objWin[i1-i+(j1-j)*OBJpitch];
					}
				}
				Out[i+j*SWpitch] = calculateCorr(sumBB,sumAA,sumAB,sumA,sumB,sumA2,sumB2,n,sqrtB);
				if (Out[i+j*SWpitch] > max)
				{
					max = Out[i+j*SWpitch];
					*imax = i;
					*jmax = j;
				}
			}
		}*/
	delete []bb;
	delete []sumMatA;
	delete []sumMatAA;

	return max;
}
float FCorr::calculateCorr(Uint64 sumBB,Uint64 sumAA,Uint64 sumAB,Uint32 sumA,Uint32 sumB,Uint64 sumA2, Uint64 sumB2,Uint16 n,Uint32 sqrtB)
{
	float ans;
	Uint64 temp1,temp2;

	Uint8 devide_counter = 0;
	Uint32 sqrtA;

	temp1 = n*sumAA;
	temp2 = temp1 - sumA2;

	while (temp2 > 10000000)
	{
		temp2/=10000;
		devide_counter++;
	}
	sqrtA = findsqrt(temp2);
	while (devide_counter != 0)
	{
		sqrtA*=100;
		devide_counter--;
	}
	temp1 = n*sumAB-(Uint64)sumA*(Uint64)sumB;
	temp2 = sqrtA*sqrtB;
	ans = ((float)temp1) / ((float)temp2);
	return ans;
}
Uint16 FCorr::findsqrt(Uint32 x)
{
	Uint16 max,min,med;
	max = 4000;
	min = 0;
	med = (max+min)/2;

	while(max-min != 1)
	{
		if(x==A[med])
			return med;
		else if(x>A[med])
		{
			min = med;
			med = (max+min)/2;
		}
		else if(x<A[med])
		{
			max = med;
			med = (max+min)/2;
		}
	}
	if((x-A[min]) >= (A[max]-x))
		return max;
	else
		return min;
}
