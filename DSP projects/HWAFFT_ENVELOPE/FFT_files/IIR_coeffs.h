#ifndef __IIR_COEFFS_H__
#define __IIR_COEFFS_H__

#define NSEC 13
const int IIR_LP_1000_Hz_NL = NSEC;
const int16 IIR_LP_1000_Hz_NUM {
    12328, -24346, 12328 ,-32145, 16071,
    11539, -22736, 11539 ,-31412, 15371
    9975, -19522, 9975 ,-30346, 14390
    7132, -13672, 7132  ,-28635, 12843
    2901, -4962, 2901  ,-26198, 10654
    2020, 2020, 0, -12345, 0
}

#endif
