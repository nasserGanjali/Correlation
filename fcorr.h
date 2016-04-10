#ifndef FCORR
#define FCORR

#include "DataType.h"

#define MAX_OBJ_SIZE  1000
#define MAX_SW_SIZE  10000


class FCorr
{
public:

    FCorr();
    ~FCorr();

    float Corr(float *Out,Uint16 *imax,Uint16 *jmax,Uint8 *searchWin,Uint8 *objWin,Uint16 Swidth,
    		Uint16 Sheight,Uint16 Spitch,Uint16 Owidth, Uint16 Oheight,Uint16 Opitch);    //convert int to uint8

private:
    float calculateCorr(Uint64 sumBB,Uint64 sumAA,Uint64 sumAB,Uint32 sumA,Uint32 sumB,
    		Uint64 sumA2, Uint64 sumB2,Uint16 n,Uint32 sqrtB); //calculate corr for 2 matrix with the same
    Uint16 findsqrt(Uint32 x);                                                                                               //calculate sqrt of x
};


#endif // FCORR

