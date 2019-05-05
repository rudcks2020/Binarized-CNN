#ifdef NETWORK_CONFIG_H_
#else
#define NETWORK_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#include "Operation.h"


#define _CRT_SECURE_NO_WARNINGS
#define INPUT_IMAGE_WIDTH 	32
#define INPUT_IMAGE_HEIGHT 	32
#define INPUT_IMAGE_CHANNEL 3

#define LAYER_NUM 			9
#define CONV_NUM 			6
#define FC_NUM 				3

#define FIBO				1
#define BIBO				2
#define BIFO				3
#define FIFO				4

typedef unsigned char 		u8;
typedef unsigned short int 	u16;
typedef unsigned int 		u32;
typedef unsigned long long 	u64;
typedef signed char			s8;
typedef signed short int	s16;
typedef signed int 			s32;
typedef signed long long 	s64;
typedef u32 				packed_t;
typedef u16					pfmap_t;
typedef float 				thresh_t;
typedef float				last_layer_BNparam_t;
typedef float				final_result_t;

typedef struct layer_config{
	u32 layer_type;				// CONV or FC
	u32 layer_number; 			// 0,1,2...
	u32 input_fmap_width;		// ifmap width
	u32 input_fmap_height;		// ifmap height
	u32 input_fmap_channel;		// ifmap channel
	u32 filter_size;			// filter width
	u32 output_fmap_width;		// ifmap width
	u32 output_fmap_height;		// ifmap height
	u32 output_fmap_channel;	// ifmap channel
	u32 quantize_bit;			// Quantize bit
	u32 conv_stride;			// Stride
	u32 conv_pad;
	u32 pool_stride;
	u32 pool_pad;
	u32 pool_size;
}layer_config;
static layer_config lc[LAYER_NUM];

void layer_configuration(layer_config *lc);
void print_network(layer_config *lc);

#endif