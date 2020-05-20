/* Test for cfft: rand */
#define NX 16
#define FNAME "t2"
#define MAXERROR 16

#pragma DATA_SECTION (x,".input")

DATA x[2*NX] ={
645,	624,	/* 0 */ 
-386,	597,	/* 1 */ 
153,	-129,	/* 2 */ 
-21,	564,	/* 3 */ 
560,	-634,	/* 4 */ 
375,	-211,	/* 5 */ 
-63,	448,	/* 6 */ 
-691,	-703,	/* 7 */ 
460,	-518,	/* 8 */ 
-80,	-427,	/* 9 */ 
165,	-432,	/* 10 */ 
418,	148,	/* 11 */ 
604,	-327,	/* 12 */ 
341,	-432,	/* 13 */ 
-465,	-695,	/* 14 */ 
-136,	353,	/* 15 */ 
};

#pragma DATA_SECTION (rtest,".test")

DATA rtest[2*NX] ={
1889,	-1768,	 /* 0 */ 
-1351,	2072,	 /* 1 */ 
-2773,	1766,	 /* 2 */ 
-48,	2030,	 /* 3 */ 
3315,	632,	 /* 4 */ 
1549,	160,	 /* 5 */ 
0,	426,	 /* 6 */ 
-17,	-2384,	 /* 7 */ 
2238,	-1554,	 /* 8 */ 
-298,	-515,	 /* 9 */ 
3282,	2057,	 /* 10 */ 
-1660,	2080,	 /* 11 */ 
1644,	-727,	 /* 12 */ 
2066,	2674,	 /* 13 */ 
-749,	17,	 /* 14 */ 
1232,	3015,	 /* 15 */ 
};

DATA error;

