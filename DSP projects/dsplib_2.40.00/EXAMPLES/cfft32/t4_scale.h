/* Test for cfft: rand */
#define NX 64
#define FNAME "t4"
#define MAXERROR 16

#pragma DATA_SECTION (x,".input")

LDATA x[2*NX] ={
915664605,	643819794,	/* 0 */ 
584459261,	735266472,	/* 1 */ 
-125733192,	-69857593,	/* 2 */ 
219209568,	177238926,	/* 3 */ 
950833175,	-1224704053,	/* 4 */ 
-1424638478,	-159153948,	/* 5 */ 
332453427,	697692854,	/* 6 */ 
-1453446161,	1269589771,	/* 7 */ 
-273492445,	-733289899,	/* 8 */ 
-630207392,	-792539252,	/* 9 */ 
1209883887,	1181561286,	/* 10 */ 
-1567397565,	-864992977,	/* 11 */ 
865964977,	985287810,	/* 12 */ 
1521681783,	1319863612,	/* 13 */ 
1583854240,	-866466852,	/* 14 */ 
933546351,	-842492483,	/* 15 */ 
-198244017,	-1455108345,	/* 16 */ 
-5457548,	-1362582923,	/* 17 */ 
-924416409,	455088769,	/* 18 */ 
463739933,	-998996136,	/* 19 */ 
-581610974,	1111321177,	/* 20 */ 
1486951649,	-1053892716,	/* 21 */ 
732431002,	-1063935357,	/* 22 */ 
-284550578,	1597469529,	/* 23 */ 
790390351,	-194584562,	/* 24 */ 
-749950594,	-516934775,	/* 25 */ 
-194153269,	-600414512,	/* 26 */ 
1400602536,	-436041134,	/* 27 */ 
592495395,	-345029577,	/* 28 */ 
-928952150,	295792146,	/* 29 */ 
1096353826,	-1228907010,	/* 30 */ 
416207467,	-1492680367,	/* 31 */ 
-1183577206,	-133803580,	/* 32 */ 
-946491623,	1195341316,	/* 33 */ 
346446712,	1403370293,	/* 34 */ 
419772959,	-761255741,	/* 35 */ 
-418594400,	-1097845050,	/* 36 */ 
242863407,	1204997665,	/* 37 */ 
-156985772,	-847121266,	/* 38 */ 
-1474044037,	471299016,	/* 39 */ 
-1528048251,	1508891833,	/* 40 */ 
-605366495,	533026480,	/* 41 */ 
-1574336018,	1197001418,	/* 42 */ 
-374995777,	-1583822255,	/* 43 */ 
591796171,	-1173115371,	/* 44 */ 
-1315856151,	1030158538,	/* 45 */ 
-1501698647,	-225690121,	/* 46 */ 
363240936,	1261445982,	/* 47 */ 
350781917,	759178907,	/* 48 */ 
-1564972675,	605394416,	/* 49 */ 
-1563049374,	-497337013,	/* 50 */ 
-1001620307,	-1079312556,	/* 51 */ 
280904060,	-1112995013,	/* 52 */ 
-1429814235,	-998253657,	/* 53 */ 
-427995050,	-250622001,	/* 54 */ 
424825300,	1150446153,	/* 55 */ 
703353284,	-31510198,	/* 56 */ 
622671040,	1021041414,	/* 57 */ 
-1344177805,	-126784485,	/* 58 */ 
-147515523,	-137822577,	/* 59 */ 
-187999942,	-159364208,	/* 60 */ 
-474266772,	-283691392,	/* 61 */ 
-1119478717,	1297926989,	/* 62 */ 
567650284,	-1597859191,	/* 63 */ 
};

#pragma DATA_SECTION (rtest,".test")

LDATA rtest[2*NX] ={
-135501657,	-51426525,	 /* 0 */ 
-64669405,	-202626274,	 /* 1 */ 
137255198,	125124276,	 /* 2 */ 
-34319993,	75135285,	 /* 3 */ 
34157606,	-32159394,	 /* 4 */ 
-46477448,	-59251295,	 /* 5 */ 
109233782,	-194364721,	 /* 6 */ 
201927139,	-91864291,	 /* 7 */ 
-46540589,	221614030,	 /* 8 */ 
-77940030,	1436203,	 /* 9 */ 
29835250,	592415,	 /* 10 */ 
-11584521,	75132876,	 /* 11 */ 
179625267,	-43038729,	 /* 12 */ 
-39291273,	-1908089,	 /* 13 */ 
153769609,	13928576,	 /* 14 */ 
96960323,	-274615382,	 /* 15 */ 
186606832,	22097134,	 /* 16 */ 
-14257892,	80586653,	 /* 17 */ 
-108946676,	-7271524,	 /* 18 */ 
153652962,	33280456,	 /* 19 */ 
113249414,	191355682,	 /* 20 */ 
100024364,	-4788887,	 /* 21 */ 
9314999,	-110917125,	 /* 22 */ 
117972903,	18528268,	 /* 23 */ 
-196517408,	-92677203,	 /* 24 */ 
20373553,	-7916819,	 /* 25 */ 
229752120,	183424499,	 /* 26 */ 
74009264,	-15237935,	 /* 27 */ 
-35596109,	53458915,	 /* 28 */ 
-48895040,	225372445,	 /* 29 */ 
-13869668,	89890933,	 /* 30 */ 
-14759072,	76353728,	 /* 31 */ 
74252142,	-17240505,	 /* 32 */ 
112905209,	-6174291,	 /* 33 */ 
17191332,	-98197235,	 /* 34 */ 
-204873621,	-107721644,	 /* 35 */ 
-114185797,	60664573,	 /* 36 */ 
15036228,	25421468,	 /* 37 */ 
-169234005,	163308104,	 /* 38 */ 
-100426643,	57395817,	 /* 39 */ 
137565297,	31151845,	 /* 40 */ 
-235396693,	-125317329,	 /* 41 */ 
-76135658,	84931106,	 /* 42 */ 
40993590,	95855736,	 /* 43 */ 
-122967917,	-33945046,	 /* 44 */ 
207008313,	164586690,	 /* 45 */ 
-92487021,	-209670814,	 /* 46 */ 
63254923,	-69515822,	 /* 47 */ 
-20943775,	-119233252,	 /* 48 */ 
210918425,	44324930,	 /* 49 */ 
-107487612,	61752253,	 /* 50 */ 
22820804,	-86814180,	 /* 51 */ 
-123194927,	-163904342,	 /* 52 */ 
-92394246,	129283063,	 /* 53 */ 
-117040899,	44590066,	 /* 54 */ 
392418892,	-21634114,	 /* 55 */ 
-51817317,	51163716,	 /* 56 */ 
-75715410,	79168044,	 /* 57 */ 
-48808184,	212297954,	 /* 58 */ 
182700980,	24131769,	 /* 59 */ 
92965253,	-124359213,	 /* 60 */ 
9978,	-10534193,	 /* 61 */ 
-57455200,	-57932359,	 /* 62 */ 
97634327,	268738785,	 /* 63 */ 
};

LDATA error;

