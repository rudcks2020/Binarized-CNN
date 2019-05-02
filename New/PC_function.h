#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TEST_SETS 1
#define EPSILON 0.001

#define VEC3(array,size,d,y,x) ((array)[d + (x)*(size) + (y)*(size)*(size)])
#define VEC4(array,size,input_depth,output_depth,ol,d,y,x) ((array)[ol + (d*output_depth) + (x*input_depth*output_depth) + (y*size*output_depth*input_depth)])

#define ACCESS1D(buff, i)		 			(*((buff) + (i)))
#define ACCESS2D_WH_order(buff, W, y, x) 	(*((buff) + (W)*(y) + (x)))
#define ACCESS3D(buff, H, W, z, y, x) 		(*((buff) + (W)*(H)*(z) + (W)*(y) + (x)))
#define ACCESS3D_CWH_order(buff, C, W, z, y, x) 	(*((buff) + (C)*((W)*(y)+(x)) + (z)))

#define LEAKY(a, b) (a) > (b) ? (0.3*b) : (b)

float find_max_val(const float *inArr, int size) {
	double max_val = inArr[0];
	for (int i = 0; i < size; i++) {
		if (max_val < inArr[i]) max_val = inArr[i];
	}
	return max_val;
}
int find_max_ind(const float *inArr, int size) {
	float max_val = inArr[0];
	int max_ind = 0;
	for (int i = 0; i < size; i++) {
		if (max_val < inArr[i]) {
			max_val = inArr[i];
			max_ind = i;
		}
	}
	return max_ind;
}
int correct(const float *inArr) {
	int result = 0;
	for (int i = 0; i < 49; i++) {
		if (inArr[i] >= 0.5) result = 1;
	}
	
	return result;
}

float average(const float *inArr, int size) {
	float avg_val = 0;
	float a = size;
	for (int i = 0; i < size; i++) {
		avg_val += inArr[i];
	}
	return avg_val / a;
}

void convolution(const float *inArr, float *outArr, const float *weight, int input_size, int input_depth, int ksize, int output_depth, int stride) {
	int out_height = (input_size / stride) + (input_size%stride);
	printf("\nout_height = %d", out_height);
	int pad_size = MAX2(((out_height - 1) * stride + ksize - input_size), 0);
	printf("\npad_size = %d", pad_size);
	int pad_top = pad_size / 2;
	if (ksize == 4) pad_top = 0;
	printf("\npad_top = %d", pad_top);
	for (int ol = 0; ol < output_depth; ol++) {
		for (int y = 0; y < out_height; y++) {
			for (int x = 0; x < out_height; x++) {
				outArr[ol + output_depth*x + output_depth*out_height*y] = 0;
				for (int filter_y = 0; filter_y < ksize; filter_y++) {
					for (int filter_x = 0; filter_x < ksize; filter_x++) {
						for (int d = 0; d < input_depth; d++) {
							if ((x*stride) + filter_x - pad_top < 0 || (y*stride) + filter_y - pad_top < 0 || (y*stride) + filter_y - pad_top >= input_size || (x*stride) + filter_x - pad_top >= input_size){
								outArr[ol + output_depth*x + output_depth*out_height*y] += (-1) * weight[ol*input_depth*ksize*ksize + d + input_depth*filter_x + input_depth*ksize*filter_y];
							}
							else {
								outArr[ol + output_depth*x + output_depth*out_height*y] += inArr[d + input_depth*(x-pad_top+filter_x) + input_depth*input_size*(y-pad_top+filter_y)] * weight[ol*input_depth*ksize*ksize + d + input_depth*filter_x + input_depth*ksize*filter_y];
								/*if (ol == 0 && d ==0) {
								printf("(%d, %d)<=[%d, %d, %d] ", x, y, x + filter_x, y + filter_y, d);
								}*/
							}
						}
					}
				}
				// printf("\n width = %d height = %d channel = %d", x,y,ol);
				// printf("\noutput = %f", outArr[ol + output_depth*x + output_depth*out_height*y]);
			}
		}
	}
}

void convolution_first(const float *inArr, float *outArr, const float *weight, int input_size, int input_depth, int ksize, int output_depth, int stride) {
	int out_height = (input_size / stride) + (input_size%stride);
	printf("\nout_height = %d", out_height);
	int pad_size = MAX2(((out_height - 1) * stride + ksize - input_size), 0);
	printf("\npad_size = %d", pad_size);
	int pad_top = pad_size / 2;
	//printf("\npad_top = %d", pad_top);
	for (int i=0; i<10; i++) printf("\nweight = %f", weight[i]);
	for (int i=0; i<10; i++) printf("\ninArr = %f", inArr[i]);

	for (int ol = 0; ol < output_depth; ol++) {
		for (int y = 0; y < out_height; y++) {
			for (int x = 0; x < out_height; x++) {
				outArr[ol + output_depth*x + output_depth*out_height*y] = 0;
				for (int filter_y = 0; filter_y < ksize; filter_y++) {
					for (int filter_x = 0; filter_x < ksize; filter_x++) {
						for (int d = 0; d < input_depth; d++) {
							if ((x*stride) + filter_x - pad_top < 0 || (y*stride) + filter_y - pad_top < 0 || (y*stride) + filter_y - pad_top >= input_size || (x*stride) + filter_x - pad_top >= input_size);
							else {
								outArr[ol + output_depth*x + output_depth*out_height*y] += inArr[d + input_depth*(x-pad_top+filter_x) + input_depth*input_size*(y-pad_top+filter_y)] * weight[ol*input_depth*ksize*ksize + d + input_depth*filter_x + input_depth*ksize*filter_y];
								// if (ol == 0 && y ==0 && x ==0) printf("\noutput = %f", outArr[ol + output_depth*x + output_depth*out_height*y]);
								// if (ol == 0 && y ==0 && x ==0) printf("\ninArr = %f", inArr[d + input_depth*(x-pad_top+filter_x) + input_depth*input_size*(y-pad_top+filter_y)]);
								// if (ol == 0 && y ==0 && x ==0) printf("\nweight = %f", weight[ol*input_depth*ksize*ksize + d + input_depth*filter_x + input_depth*ksize*filter_y]);
								// if (ol == 0 && y ==0 && x ==0) {
								// 	for printf("weight %f")
								// }
							}
						}
					}
				}
				// printf("\n width = %d height = %d channel = %d", x,y,ol);
				// printf("\noutput = %f", outArr[ol + output_depth*x + output_depth*out_height*y]);
			}
		}
	}
}

void mul_scale(const float *inArr, float *outArr, const float *scale, int input_size, int input_depth) {
	for (int d = 0; d < input_depth; d++) {
		for (int y = 0; y < input_size; y++) {
			for (int x = 0; x < input_size; x++) {
				VEC3(outArr, input_size, d, y, x) = VEC3(inArr, input_size, d, y, x) * scale[d];
			}
		}
	}
}

void relu(const float *inArr, float *outArr, int total_size) {
	for (int i = 0; i < total_size; i++) {
		outArr[i] = MAX2(0, inArr[i]);
	}
}

void bin_active(const float *inArr, float *outArr, int total_size) {
	for (int i = 0; i < total_size; i++) {
		outArr[i] = ((float)(inArr[i] > 0.5))*2-1;
	}
}

void leaky(const float *inArr, float *outArr, int total_size) {
	for (int i = 0; i < total_size; i++) {
		outArr[i] = LEAKY(0, inArr[i]);
	}
}

void max_pooling(const float *inArr, float *outArr, int input_size, int input_depth, int stride) {
	float data[2 * 2] = { 0 };
	// for (i=0;i<4;i++) printf("\n data = %f", data[i]);
	int out_height = (input_size / stride) + (input_size%stride);
	// printf("\nout_height = %d", out_height);
	for (int ol = 0; ol < input_depth; ol++) {
		for (int y = 0; y < out_height; y++) {
			for (int x = 0; x < out_height; x++) {
				for (int filter_y = 0; filter_y < 2; filter_y++) {
					for (int filter_x = 0; filter_x < 2; filter_x++) {
						data[filter_x + (2*filter_y)] = inArr[ol + input_depth*(2*x+filter_x) + input_depth*input_size*(2*y+filter_y)];
						// if (ol == 0 && y ==0 && x ==0) printf("\ninArr = %f", inArr[ol + input_depth*(2*x+filter_x) + input_depth*out_height*(2*y+filter_y)]);
						// VEC3(inArr, input_size, ol, (y*stride) + filter_y, (x*stride) + filter_x);
					}
				}
				outArr[ol + input_depth*x + input_depth*out_height*y] = find_max_val(data, 2 * 2);
			}
		}
	}
}

void reshape(const float *inArr, float *outArr, int input_size, int input_depth) {
	for (int i = 0; i < input_size*input_size; i++) {
		for (int j = 0; j < input_depth; j++) {
			outArr[j + input_depth * i] = inArr[input_size * input_size * j + i];
		}
	}
}

void full_connection(const float *inArr, float *outArr, const float *weight, const float *bias, int input_depth, int output_depth) {
	for (int j = 0; j < 49; j++) {
		outArr[j] = 0;
		for (int k = 0; k < 128; k++) {
			outArr[j] += (weight[k] * inArr[k+(128*j)]);
		}
		outArr[j] += bias[0];
	}
}

void avg_pool(const float *inArr, float *outArr, int input_size, int input_depth) {
	float data[8 * 8];
	for (int d = 0; d < input_depth; d++) {
		for (int y = 0; y < input_size; y++) {
			for (int x = 0; x < input_size; x++)
				data[x + (y * 8)] = VEC3(inArr, input_size, d, y, x);
		}
		outArr[d] = average(data, 8 * 8);
	}
}

void bn(const float *inArr, float *outArr, const float *beta, const float *gamma, int input_size, int input_depth) {
	for (int d = 0; d < input_depth; d++) {
		// printf("\n BN input = %f", inArr[d + (input_depth*i)] );
		for (int i = 0; i < input_size*input_size; i++) {
			outArr[d + (input_depth*i)] =
				((gamma[d] * (inArr[d + (input_depth*i)]))) + beta[d];
		}
		// printf("\n channel = %d", d);
		// printf("\n BN output = %f", outArr[d + (input_depth*i)] );
	}
}

void clip(float *inArr, int size) {
	for (int i = 0; i < size; i++) {
		if ((inArr[i]/4+0.5) < 1) inArr[i] = inArr[i]/4+0.5;
		else inArr[i] = 1;
		if (inArr[i] < 0) inArr[i] = 0;
	}
}