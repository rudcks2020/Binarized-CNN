#ifdef MEMORY_H_
#else
#define MEMORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Operation.h"
#include "Network_config.h"


#define MAX2to1(a,b) ((a) > (b) ? (a) : (b))

void mem_cal(layer_config *lc);

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



#endif