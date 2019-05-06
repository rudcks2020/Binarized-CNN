#include <stdio.h>
#include "Memory.h"

void mem_cal(layer_config *lc){
	for (int i = 0; i < LAYER_NUM; i++){
		//Problem : Why *4? 
		ifmap_mem_size[i]  = lc[i].input_fmap_channel * lc[i].input_fmap_width * lc[i].input_fmap_height * 4;
		weight_mem_size[i] = lc[i].input_fmap_channel * lc[i].filter_size * lc[i].filter_size * lc[i].output_fmap_channel * 4;
		ofmap_mem_size[i]  = lc[i].output_fmap_channel * lc[i].output_fmap_width * lc[i].output_fmap_height * 4;

		//Problem : First Last Layer?
		threshold_mem_size[i] = lc[i].input_fmap_channel * sizeof(thresh_t);

		total_weight_mem_size += weight_mem_size[i];
		total_threshold_mem_size += threshold_mem_size[i];
		total_ifmap_mem_size += ifmap_mem_size[i];
		max_fmap_size = MAX2to1(max_fmap_size, MAX2to1(ifmap_mem_size[i], ofmap_mem_size[i]));
		max_weight_size = MAX2to1(max_weight_size, (lc[i].filter_size * lc[i].filter_size));

	}
	printf("Total weight size: %d \n", total_weight_mem_size);
	printf("Total threshold size: %d \n", total_threshold_mem_size);
}

void mem_alloc(){
	for (int i = 0; i < LAYER_NUM; i++){
		ifmaps_vdomain[i] = (float *) malloc((size_t)ifmap_mem_size[i] * sizeof(char));
		weight_vdomain[i] = (float *) malloc((size_t)weight_mem_size[i] * sizeof(char));
		threshold1[i] = (thresh_t *)malloc((size_t)threshold_mem_size[i] * sizeof(char));
		threshold2[i] = (thresh_t *)malloc((size_t)threshold_mem_size[i] * sizeof(char));
	}
	fmap_buffer1 = (float *)malloc(max_fmap_size * sizeof(char));
	fmap_buffer2 = (float *)malloc(max_fmap_size * sizeof(char));
}

void dat_open(FILE *file, char *file_path, u32 correct_file_size){
	file = fopen(file_path, "rb");
	if (file == NULL) {
		printf("No file existing\n");
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	u32 file_size = ftell(file);
	if (file_size != correct_file_size){
		printf("file size isn't matching with the calculated size\n");
		printf("loaded file size : %u, calculated file size : %u \n", file_size, correct_file_size);
		exit(1);
	}
}


void load_param(char *weight_file_dir, char *threshold_file_dir, char *threshold_file_dir1, char *threshold_file_dir2, char *fmap_file_dir){
	FILE *file;
	file = fopen(weight_file_dir, "rb");
	dat_open(file, weight_file_dir, total_weight_mem_size);

	for (int i = 0; i < LAYER_NUM; i++){
		/*if(weight_vdomain[i] = NULL){
			printf("Memory accessing.. weight_domain[%d]\n", i);
		}*/
		printf("Loading weight_data[%d]... \n", i);
		fread(weight_vdomain[i], sizeof(char), weight_mem_size[i], file);
	}
	printf("Load Complete..\n");
	fclose(file);

	file = fopen(threshold_file_dir1, "rb");
	dat_open(file, threshold_file_dir1, total_threshold_mem_size);
	for (int i = 0; i < LAYER_NUM; i++) {
		/*if(weight_vdomain[i] = NULL){
			printf("Memory accessing.. weight_domain[%d]\n", i);
		}*/
		printf("Loading threshold_data[%d]... \n", i);
		fread(threshold1[i], sizeof(char), threshold_mem_size[i], file);
	}
	printf("Load Complete..");
	fclose(file);

	file = fopen(fmap_file_dir, "rb");
	dat_open(file, fmap_file_dir, total_ifmap_mem_size);
	for (int i = 0; i < LAYER_NUM; i++) {
		/*if(weight_vdomain[i] = NULL){
			printf("Memory accessing.. weight_domain[%d]\n", i);
		}*/
		printf("Loading ifmap_data[%d]... \n", i);
		fread(ifmaps_vdomain[i], sizeof(char), ifmap_mem_size[i], file);
	}
	printf("Load Complete..");
	fclose(file);
}
