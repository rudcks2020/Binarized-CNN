#ifdef MEMORY_H_
#else
#define MEMORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Operation.h"
#include "Network_config.h"


#define WEIGHT_FILE_DIR     	"C:\\Github\\Binarized-CNN\\param\\weight.dat"
#define THRESHOLD_FILE_DIR     	"C:\\Github\\Binarized-CNN\\param\\threshold.dat"
#define THRESHOLD_FILE_DIR1    	"C:\\Github\\Binarized-CNN\\param\\threshold1.dat"
#define THRESHOLD_FILE_DIR2    	"C:\\Github\\Binarized-CNN\\param\\threshlod2.dat"
#define FMAP_FILE_DIR			"C:\\Github\\Binarized-CNN\\param\\fmap.dat"

#define MAX2to1(a,b) ((a) > (b) ? (a) : (b))

void mem_cal(layer_config *lc);
void mem_alloc();
void dat_open(FILE *file, char *file_path, u32 correct_file_size);
void load_param(char *weight_file_dir, char *threshold_file_dir, char *threshold_file_dir1, char *threshold_file_dir2, char *fmap_file_dir);

thresh_t *threshold[LAYER_NUM];
thresh_t *threshold1[LAYER_NUM];
thresh_t *threshold2[LAYER_NUM];

u32 ifmap_mem_size[LAYER_NUM];
u32 weight_mem_size[LAYER_NUM];
u32 threshold_mem_size[LAYER_NUM];
u32 threshold_mem_size2[LAYER_NUM];
u32 ofmap_mem_size[LAYER_NUM];
u32 total_ifmap_mem_size;
u32 total_weight_mem_size;
u32 total_threshold_mem_size;
u32 total_threshold_mem_size2;
u32 max_fmap_size;
u32 max_weight_size;
u32 final_result_mem_size;

void *ifmaps_vdomain[LAYER_NUM];
void *weight_vdomain[LAYER_NUM];
void *fmap_buffer1;
void *fmap_buffer2;
void *filter_window;
float *final_result;
void *ifmap_window;




#endif