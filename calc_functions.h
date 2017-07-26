#ifndef CALC_FUNCTIONS_H_
#define CALC_FUNCTIONS_H_

#define BUF_LENGTH          256
#define NUMBER_OF_AC_POINTS 512
#define DIVIDER             16
#define THRESH_AC_LOW       200
#define THRESH_DC_HIGH      4090

// coeffs
#define A1  220
#define B1  (-2205)
#define C1  1460
#define A2  209
#define B2  (-2139)
#define C2  1378

signed int SpCO;
unsigned int SCALE_FACTOR_660;
unsigned int SCALE_FACTOR_800;
unsigned int SCALE_FACTOR_940;

unsigned int get_average(unsigned int* buf, unsigned int length);

void get_DCs(unsigned int* pDC_660,
             unsigned int* pDC_800,
             unsigned int* pDC_940);

void get_ACs(unsigned int* pAC_660,
             unsigned int* pAC_800,
             unsigned int* pAC_940);

unsigned int calc_SpCO(unsigned int DC_660,
                       unsigned int AC_660,
                       unsigned int DC_800,
                       unsigned int AC_800,
                       unsigned int DC_940,
                       unsigned int AC_940);

void adjust_to_ADC(unsigned int DC_660,
                   unsigned int AC_660,
                   unsigned int DC_800,
                   unsigned int AC_800,
                   unsigned int DC_940,
                   unsigned int AC_940);

#endif /* CALC_FUNCTIONS_H_ */
