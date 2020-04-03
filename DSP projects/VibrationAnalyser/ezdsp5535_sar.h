#ifndef EZDSP5535_SAR_H_
#define EZDSP5535_SAR_H_

#define SARCTRL      ((ioport volatile unsigned*)0x7012)
#define SARDATA      ((ioport volatile unsigned*)0x7014)
#define SARCLKCTRL   ((ioport volatile unsigned*)0x7016)
#define SARPINCTRL   ((ioport volatile unsigned*)0x7018)
#define SARGPOCTRL   ((ioport volatile unsigned*)0x701A)

#define SW1   0x2A8  // Switch 1 value
#define SW2   0x1FC  // Switch 2 value
#define SW12  0x195  // Switch 1 + 2 value
#define NoKey 0x3FE

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
void Init_SAR(void);
Uint16 Get_Sar_Key(void);

#endif /*EZDSP5535_SAR_H_*/
