//
// Parameters for 3 arc-second standard resolution mode of operation
// Generated for Debian packaging by bdale@gag.com on 2009-09-08
//

#define HD_MODE 0

// how big an area to allow processing in 3 arc-second mode
//	2 x 2 Degrees  ---------   25 Megabytes minimum RAM
//	3 x 3 Degrees  ---------   52 Megabytes minimum RAM
//	4 x 4 Degrees  ---------   95 Megabytes minimum RAM
//	5 x 5 Degrees  ---------  145 Megabytes minimum RAM
//	6 x 6 Degrees  ---------  210 Megabytes minimum RAM
//	7 x 7 Degrees  ---------  285 Megabytes minimum RAM
//	8 x 8 Degrees  ---------  370 Megabytes minimum RAM
#define NUMDEGREES	8

#define MAXPAGES (NUMDEGREES*NUMDEGREES)

