/*
 * bnn_utils.c
 *
 *  Created on: 2019. 1. 30.
 *      Author: Minsang
 */
#include "bnn_utils.h"
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS


u32 xor_bitcount32(packed_t operand1, packed_t operand2){
	packed_t result = operand1^operand2;
    result = ((0xAAAAAAAA & result) >> 1) + (0x55555555 & result);
    result = ((0xCCCCCCCC & result) >> 2) + (0x33333333 & result);
    result = ((0xF0F0F0F0 & result) >> 4) + (0x0F0F0F0F & result);
    result = ((0xFF00FF00 & result) >> 8) + (0x00FF00FF & result);
    result = ((0xFFFF0000 & result) >> 16) + (0x0000FFFF & result);
    return result;
}

void softmax(float *output, float *input, int size){
	double sum = 0;
	int i;
	for (i=0; i<size; i++){
		// printf("%f\n",input[i]);
		sum += exp(input[i]);
	}
	for (i=0; i<size; i++) {
		output[i] = exp(input[i]) / sum;
	}
}

void BN_ReLU(float *output, float *input, thresh_t *scale, thresh_t *bias, layer_property lp){
	int i;
	for (i=0; i<lp.output_fmap_channel; i++){
		// printf("%f\n",((float *)scale)[i]);
		// printf("%f\n",((float *)bias)[i]);
		output[i] = MAX2(input[i] * ((float *)scale)[i] + ((float *)bias)[i],0);
		// printf("%f\t%f\n",input[i], output[i]);
		// if (output[i] > 0.0001) printf("%f\t%f\n",input[i], output[i]);
	}
}


