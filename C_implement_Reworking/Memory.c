#include <stdio.h>
#include "Memory.h"

void mem_cal(layer_config *lc){
	for(int i = 0; i < LAYER_NUM; i++){
		ifmap_mem_size[i]  = lc[i].input_fmap_channel * lc[i].input_fmap_width * lc[i].input_fmap_height * 4;
		weight_mem_size[i] = lc[i].input_fmap_channel * lc[i].filter_size * lc[i].filter_size * lc[i].output_fmap_channel * 4;
		ofmap_mem_size[i]  = lc[i].output_fmap_channel * lc[i].output_fmap_width * lc[i].output_fmap_height * 4;

		//Problem
		threshold_mem_size2[i] = (i == 0) ? 0 :
								 (i == LAYER_NUM - 1) ?  0 : (lc[i].output_fmap_channel * sizeof(thresh_t));

		total_weight_mem_size += weight_mem_size[i];
		total_threshold_mem_size += threshold_mem_size2[i];
		total_ifmap_mem_size += ifmap_mem_size[i];
		max_fmap_size = MAX2to1(max_fmap_size, MAX2to1(ifmap_mem_size[i], ofmap_mem_size[i]));
		max_weight_size = MAX2to1(max_weight_size, (lc[i].filter_size * lc[i].filter_size));

	}
	printf("Total weight size: %d \n", total_weight_mem_size);
	printf("Total threshold size: %d \n", total_threshold_mem_size);
}