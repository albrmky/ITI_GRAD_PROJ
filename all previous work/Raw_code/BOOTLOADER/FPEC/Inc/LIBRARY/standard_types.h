/*
 * standard_types.h
 *
 *  Created on: Jan 2, 2022
 *      Author: noureldeen albrmky
 */

#ifndef STANDARD_TYPES_H_
#define STANDARD_TYPES_H_

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;

typedef float f32;
typedef float f64;

typedef enum {
	ZERO,
	ONE,
	ERROR
} STATUS;

#define NULL (void*)(0)

#endif /* STANDARD_TYPES_H_ */
