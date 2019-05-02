/*
 * bnn_utils.h
 *
 *  Created on: 2019. 1. 30.
 *      Author: Minsang
 */

#ifndef BNN_UTILS_H_
#define BNN_UTILS_H_

#include <stdio.h>

#define WEIGHT_FILE_DIR     	"../param/weight.dat"
#define THRESHOLD_FILE_DIR     	"../param/threshold.dat"
#define THRESHOLD_FILE_DIR1    	"../param/threshold1.dat"
#define THRESHOLD_FILE_DIR2    	"../param/threshold2.dat"
#define FMAP_FILE_DIR     		"../param/fmap.dat"

#define INPUT_IMAGE_WIDTH	32
#define INPUT_IMAGE_HEIGHT	32
#define INPUT_IMAGE_CHANNEL	3

#define LAYER_NUM 		9
#define CONV_NUM 		6
#define FC_NUM 			3

#define BIBOCONV		1
#define FIBOCONV		2
#define BIFOCONV		3
#define FIFOCONV		4

#define FIRST_LAST_LAYER_BINARIZED 0

#define MAX2(a,b) ((a) > (b) ? (a) : (b))

#define ACCESS1D(buff, i)		 			(*((buff) + (i)))
#define ACCESS2D_WH_order(buff, W, y, x) 	(*((buff) + (W)*(y) + (x)))
#define ACCESS3D(buff, H, W, z, y, x) 		(*((buff) + (W)*(H)*(z) + (W)*(y) + (x)))
#define ACCESS3D_CWH_order(buff, C, W, z, y, x) 	(*((buff) + (C)*((W)*(y)+(x)) + (z)))
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

typedef struct layer_property{
	u32 layer_type;	// CONV or FC
	u32 layer_number; 	// 0,1,2...
	u32 input_fmap_width;		// ifmap width
	u32 input_fmap_height;		// ifmap height
	u32 input_fmap_channel;		// ifmap channel
	u32 input_packed_channel;		// ifmap block
	u32 filter_size;		// filter width
	u32 output_fmap_width;		// ifmap width
	u32 output_fmap_height;		// ifmap height
	u32 output_fmap_channel;		// ifmap channel
	u32 output_packed_channel;		// ofmap block
	u32 quantize_bit;	// Quantize bit
	u32 conv_stride;	// Stride
	u32 conv_pad;
	u32 pool_stride;
	u32 pool_pad;
	u32 pool_size;
}layer_property;
static layer_property lp[LAYER_NUM];

void *filter_window;
void *receptive_field_window;

u32 xor_bitcount32(packed_t operand1, packed_t operand2);
// for K-1 th layer //
void BN_ReLU(float *output, float *input, thresh_t *scale, thresh_t *bias, layer_property lp);
// for K th layer (last layer) //
void softmax(float *output, float *input, int size);



#endif /* BNN_UTILS_H_ */
