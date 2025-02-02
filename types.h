#ifndef TYPES_H
#define TYPES_H

// Fixed-point numbers
typedef short			sfx16;
typedef unsigned long	ufx32;
#define PI_16	(157 << 6) / 50
#define PI_32	(157 << 12) / 50

// Vectors
typedef struct{
	int x,y;
}Vec2i;

typedef struct{
	int x,y,w,h;
}Vec4i;

typedef struct{	// Ideal for normalized vectors.
	sfx16 x,y;	// 64 >> 6 = 1
				// 32 >> 6 = 0.5
}Vec2sfx16;

typedef struct{	// Ideal for positions.
	ufx32 x,y;	// 4096 >> 12 = 1
				// 2048 >> 12 = 0.5
}Vec2ufx32;

#endif