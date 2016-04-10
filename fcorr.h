#ifndef FCORR
#define FCORR

#include <stdint.h>
#include <iostream>
using namespace std;

#define MAX_OBJ_SIZE  1000
#define MAX_SW_SIZE  10000


class FCorr
{
public:

    FCorr();
    ~FCorr();

    float Corr(float *Out,uint16_t *imax,uint16_t *jmax,uint8_t *searchWin,uint8_t *objWin,uint16_t Swidth,
            uint16_t Sheight,uint16_t Spitch,uint16_t Owidth, uint16_t Oheight,uint16_t Opitch);    //convert int to uint8_t

private:
    float calculateCorr(uint64_t sumBB,uint64_t sumAA,uint64_t sumAB,uint32_t sumA,uint32_t sumB,
            uint64_t sumA2, uint64_t sumB2,uint16_t n,uint32_t sqrtB); //calculate corr for 2 matrix with the same
    uint16_t findsqrt(uint32_t x);                                                                                               //calculate sqrt of x
};


#endif // FCORR

