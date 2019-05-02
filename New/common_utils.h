/*
 * common_utils.h
 *
 *  Created on: 2019. 1. 30.
 *      Author: Minsang
 */

#ifdef COMMON_UTILS_H_
#else
#define COMMON_UTILS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "bnn_utils.h"
int is_float_value_same(float f1, float f2);
void print_num_of_diff_fmap(float *ofmap_ref, float *ofmap_input, layer_property lp);
void print_num_of_diff_fmap2(u32 *ofmap_ref, u32 *ofmap_input, layer_property lp);
//-------------------------system functions-------------------------------
void network_config(layer_property *layer_info);
void print_network(layer_property *layer_info);
void mem_allocs();
void open_file(FILE *file, char *file_path, u32 correct_file_size);
void load_param(char *weight_file_dir, char *threshold_file_dir, char *threshold_file_dir1, char *threshold_file_dir2, char *fmap_file_dir);
void perf_end(clock_t start_time, double *total_processing_time, char *print_name);
void mem_free();
u32 i;
u32 max_block_num;
void *ifmaps_ref[LAYER_NUM];
void *weight[LAYER_NUM];

thresh_t *threshold[LAYER_NUM];
thresh_t *threshold1[LAYER_NUM];
thresh_t *threshold2[LAYER_NUM];

u32 ifmap_size[LAYER_NUM];
u32 weight_size[LAYER_NUM];
u32 threshold_size[LAYER_NUM];
u32 threshold_size2[LAYER_NUM];
u32 ofmap_size[LAYER_NUM];
u32 total_ifmap_size;
u32 total_weight_size;
u32 total_threshold_size;
u32 total_threshold_size2;
u32 max_fmap_size;
u32 max_kernel_size;
u32 final_result_size;
last_layer_BNparam_t *last_layer_BNparam;

void *fmap_buffer1;
void *fmap_buffer2;
void *filter_window;
float *final_result;
void *ifmap_window;



#endif /* COMMON_UTILS_H_ */
