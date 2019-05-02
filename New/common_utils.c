/*
 * common_utils.c
 *
 *  Created on: 2019. 1. 30.
 *      Author: Minsang
 */

#include "bnn_utils.h"
#include "common_utils.h"


//compares if the float f1 is equal with f2 and returns 1 if true and 0 if false
int is_float_value_same(float f1, float f2){
  float precision = 0.001;
  if (((f1 - precision) < f2) && ((f1 + precision) > f2)) return 1;
  else return 0;
}

void print_num_of_diff_fmap(float *ofmap_ref, float *ofmap_input, layer_property lp){
	u32 num_of_diff_ofmap = 0;
	u32 channel;
	u32 height;
	u32 width;

	printf("\n--------------------------------------\n");
	printf("\n%d channel %d height %d width \n", lp.output_fmap_channel, lp.output_fmap_height, lp.output_fmap_width);
	for (channel=0; channel<lp.output_fmap_channel; channel++){
		for(height=0; height<lp.output_fmap_height; height++){
			for(width=0; width<lp.output_fmap_width; width++){
				if (!is_float_value_same(ACCESS3D_CWH_order(ofmap_ref, lp.output_fmap_channel, lp.output_fmap_width, channel, height, width), ACCESS3D_CWH_order(ofmap_input, lp.output_fmap_channel, lp.output_fmap_width, channel, height, width))) num_of_diff_ofmap ++;
			}
		}
	}
	printf("num_of_diff_ofmap = %d\n", num_of_diff_ofmap);
	printf("num_of_total_ofmap = %d\n", lp.output_fmap_channel*lp.output_fmap_height*lp.output_fmap_width);
}


void print_num_of_diff_fmap2(u32 *ofmap_ref, u32 *ofmap_input, layer_property lp){
	u32 num_of_diff_ofmap = 0;
	u32 num_of_diff_ofmap_packed;
	u32 channel;
	u32 height;
	u32 width;

	printf("\n--------------------------------------\n");
	printf("\n%d channel %d height %d width \n", lp.output_fmap_channel, lp.output_fmap_height, lp.output_fmap_width);
	for (channel=0; channel<lp.output_fmap_channel; channel++){
		for(height=0; height<lp.output_fmap_height; height++){
			for(width=0; width<lp.output_fmap_width; width++){
				num_of_diff_ofmap_packed = ACCESS3D_CWH_order(ofmap_ref, lp.output_fmap_channel, lp.output_fmap_width, channel, height, width) ^ ACCESS3D_CWH_order(ofmap_input, lp.output_fmap_channel, lp.output_fmap_width, channel, height, width);
				for (i=0; i<32; i++){
					if((num_of_diff_ofmap_packed >> i) & 1) {
						num_of_diff_ofmap ++;
						// printf("\n%d channel %d height %d width %d th\n", channel, height, width, i);
					}
				}
			}
		}
	}
	printf("num_of_diff_ofmap = %d\n", num_of_diff_ofmap);
	printf("num_of_total_ofmap = %d\n", lp.output_fmap_channel*lp.output_fmap_height*lp.output_fmap_width);
}

void network_config(layer_property *lp){
	lp[0].input_fmap_width = INPUT_IMAGE_WIDTH;
	lp[0].input_fmap_height = INPUT_IMAGE_HEIGHT;
	lp[0].input_fmap_channel = INPUT_IMAGE_CHANNEL;
	// conv_filter & channels
	lp[0].filter_size = 3;	lp[0].output_fmap_channel = 128;
	lp[1].filter_size = 3;	lp[1].output_fmap_channel = 128;
	lp[2].filter_size = 3;	lp[2].output_fmap_channel = 256;
	lp[3].filter_size = 3;	lp[3].output_fmap_channel = 256;
	lp[4].filter_size = 3;	lp[4].output_fmap_channel = 512;
	lp[5].filter_size = 3;	lp[5].output_fmap_channel = 512;
	lp[6].filter_size = 4;	lp[6].output_fmap_channel = 1024;
	lp[7].filter_size = 1;	lp[7].output_fmap_channel = 1024;
	lp[8].filter_size = 1;	lp[8].output_fmap_channel = 10;

	// convolution stride & padding
	lp[0].conv_stride = 1;	lp[0].conv_pad = 1;	lp[0].quantize_bit = 1;
	lp[1].conv_stride = 1;	lp[1].conv_pad = 1;	lp[1].quantize_bit = 1;
	lp[2].conv_stride = 1;	lp[2].conv_pad = 1;	lp[2].quantize_bit = 1;
	lp[3].conv_stride = 1;	lp[3].conv_pad = 1;	lp[3].quantize_bit = 1;
	lp[4].conv_stride = 1;	lp[4].conv_pad = 1;	lp[4].quantize_bit = 1;
	lp[5].conv_stride = 1;	lp[5].conv_pad = 1;	lp[5].quantize_bit = 1;
	lp[6].conv_stride = 1;	lp[6].conv_pad = 0;	lp[6].quantize_bit = 1;
	lp[7].conv_stride = 1;	lp[7].conv_pad = 0;	lp[7].quantize_bit = 1;
	lp[8].conv_stride = 1;	lp[8].conv_pad = 0;	lp[8].quantize_bit = 1;

	// maxpool stride & padding
	lp[0].pool_stride = 1;	lp[0].pool_pad = 0;	lp[0].pool_size = 1;
	lp[1].pool_stride = 2;	lp[1].pool_pad = 0;	lp[1].pool_size = 2;
	lp[2].pool_stride = 1;	lp[2].pool_pad = 0;	lp[2].pool_size = 1;
	lp[3].pool_stride = 2;	lp[3].pool_pad = 0;	lp[3].pool_size = 2;
	lp[4].pool_stride = 1;	lp[4].pool_pad = 0;	lp[4].pool_size = 1;
	lp[5].pool_stride = 2;	lp[5].pool_pad = 0;	lp[5].pool_size = 2;
	lp[6].pool_stride = 1;	lp[6].pool_pad = 0;	lp[6].pool_size = 1;
	lp[7].pool_stride = 1;	lp[7].pool_pad = 0;	lp[7].pool_size = 1;
	lp[8].pool_stride = 1;	lp[8].pool_pad = 0;	lp[8].pool_size = 1;

	// network configuration
	for (i=0; i<LAYER_NUM; i++) {
		// layer_type
		if (!FIRST_LAST_LAYER_BINARIZED){
			if (i == 0) lp[i].layer_type = FIBOCONV;
			else if (i == LAYER_NUM-2) lp[i].layer_type = BIFOCONV;
			else if (i == LAYER_NUM-1) lp[i].layer_type = FIFOCONV;
			else lp[i].layer_type = BIBOCONV;
		}
		else {
			if (i == LAYER_NUM-1) lp[i].layer_type = BIFOCONV;
			else lp[i].layer_type = BIBOCONV;
		}
		// layer_num
		lp[i].layer_number = i;
		// input_output config.
		if(i != 0){
			lp[i].input_fmap_width = lp[i-1].output_fmap_width;
			lp[i].input_fmap_height = lp[i-1].output_fmap_height;
			lp[i].input_fmap_channel = lp[i-1].output_fmap_channel;
		}
		lp[i].output_fmap_width = (((((lp[i].input_fmap_width + 2 * lp[i].conv_pad - lp[i].filter_size) / lp[i].conv_stride) + 1) + 2 * lp[i].pool_pad - lp[i].pool_size) / lp[i].pool_stride) + 1;
		lp[i].output_fmap_height = (((((lp[i].input_fmap_height + 2 * lp[i].conv_pad - lp[i].filter_size) / lp[i].conv_stride) + 1) + 2 * lp[i].pool_pad - lp[i].pool_size) / lp[i].pool_stride) + 1;
	}

	// Block num configuration
	for (i=0; i<LAYER_NUM; i++){
		if (!FIRST_LAST_LAYER_BINARIZED){
			if (i == LAYER_NUM-2) lp[i].output_packed_channel = lp[i].output_fmap_channel;
			else if (i == LAYER_NUM-1) lp[i].output_packed_channel = lp[i].output_fmap_channel;
			// if not multiple of 32; roundup
			else lp[i].output_packed_channel = (lp[i].output_fmap_channel + 31) / 32;
		}
		else{
			if (i == LAYER_NUM-1) lp[i].output_packed_channel = lp[i].output_fmap_channel;
			// if not multiple of 32; roundup
			else lp[i].output_packed_channel = (lp[i].output_fmap_channel + 31) / 32;
		}
		if (i != 0)	lp[i].input_packed_channel = lp[i-1].output_packed_channel;
		else lp[i].input_packed_channel = INPUT_IMAGE_CHANNEL;
		max_block_num = MAX2(max_block_num, lp[i].input_packed_channel);
	}

	// calcuate mem size
	for (i=0; i<LAYER_NUM; i++) {
		ifmap_size[i] = lp[i].input_fmap_channel * lp[i].input_fmap_width * lp[i].input_fmap_height * 4;
		ifmap_size[8] = 10 * 4;
		weight_size[i] = lp[i].input_fmap_channel * lp[i].filter_size * lp[i].filter_size * lp[i].output_fmap_channel * 4;

		if (!FIRST_LAST_LAYER_BINARIZED) threshold_size[i] = (i == 0) ? (lp[i].output_fmap_channel * sizeof(float)) : (lp[i].input_fmap_channel * sizeof(thresh_t));

		threshold_size2[i] = (i == 0) ? 0 : (i == LAYER_NUM-1) ? 0 : (lp[i].output_fmap_channel * sizeof(thresh_t));

		ofmap_size[i] = lp[i].output_packed_channel * lp[i].output_fmap_width * lp[i].output_fmap_height * 32 / 8;

		total_weight_size += weight_size[i];
		total_threshold_size += threshold_size[i];
		total_threshold_size2 += threshold_size2[i];
		total_ifmap_size += ifmap_size[i];
		max_fmap_size = MAX2(max_fmap_size, MAX2(ifmap_size[i], ofmap_size[i]));
		max_kernel_size = MAX2(max_kernel_size, lp[i].filter_size*lp[i].filter_size);
	}
	printf("\ntotal_weight_size = %d", total_weight_size);
	printf("\ntotal_threshold_size = %d", total_threshold_size);
	printf("\ntotal_threshold_size2 = %d\n", total_threshold_size2);
	final_result_size = lp[LAYER_NUM-1].output_fmap_channel * sizeof(final_result_t);
}

void print_network(layer_property *lp){
	// print network size
	printf("%4s%-3s%-8s%-8s%-8s%-8s%-8s%-8s%-8s%-8s\n", "", "",	"iW",	"iC", "fW", 	"oC",  	"oW", "Qbit", "oB", "iB");
	for (i=0; i<LAYER_NUM; i++) {
		printf("%4s%-3u%-8u%-8u%-8u%-8u%-8u%-8u%-8u%-8u\n", "CONV", i, 	lp[i].input_fmap_width, lp[i].input_fmap_channel, lp[i].filter_size, lp[i].output_fmap_channel, lp[i].output_fmap_width,lp[i].quantize_bit, lp[i].output_packed_channel, lp[i].input_packed_channel);
	}
	printf("%4s%-3s%-20s%-20s\n", "", "",	"weight_size", 	"ifmap_size");
	for (i=0; i<LAYER_NUM; i++) {
	printf("%4s%-3u%-20u%-20u\n", "CONV", i, weight_size[i],  ifmap_size[i]);
	}
	printf("\n");
}


void mem_allocs(){
	for (i=0; i<LAYER_NUM; i++){
		if((i==0) || (i==LAYER_NUM-1)){
			ifmaps_ref[i] = (float *) malloc((size_t)ifmap_size[i]*sizeof(char));
			weight[i] = (float *) malloc((size_t)weight_size[i]*sizeof(char));
		}
		else{
			ifmaps_ref[i] = (packed_t *) malloc((size_t)ifmap_size[i]*sizeof(char));
			weight[i] = (packed_t *) malloc((size_t)weight_size[i]*sizeof(char));
		}
		threshold[i] = (thresh_t *) malloc((size_t)threshold_size2[i]*sizeof(char));
		threshold1[i] = (thresh_t *) malloc((size_t)threshold_size[i]*sizeof(char));
		threshold2[i] = (thresh_t *) malloc((size_t)threshold_size[i]*sizeof(char));
		// else if (i == LAYER_NUM-1) last_layer_BNparam = (last_layer_BNparam_t *) malloc((size_t)threshold_size[i]*sizeof(char));
	}
	fmap_buffer1 = (packed_t *) malloc(max_fmap_size*sizeof(char));
	fmap_buffer2 = (packed_t *) malloc(max_fmap_size*sizeof(char));
	// memset(Bmap,1,sizeof(Bmap));
	// printf("%d",(int)sizeof(Bmap));
	receptive_field_window = malloc(max_kernel_size*sizeof(void));
	ifmap_window = malloc(max_kernel_size*sizeof(packed_t));
	filter_window = malloc(max_kernel_size*sizeof(void));
	final_result = (float *) malloc(final_result_size);
}

void mem_free(){
	for (i=0; i<LAYER_NUM; i++){
		free(ifmaps_ref[i]);
		free(weight[i]);
		free(threshold[i]);
		free(threshold1[i]);
		free(threshold2[i]);
	}
	free(last_layer_BNparam);
	free(fmap_buffer1);
	free(fmap_buffer2);
	free(receptive_field_window);
	free(ifmap_window);
	free(filter_window);
	free(final_result);
}

void open_file(FILE *file, char *file_path, u32 correct_file_size){
	file = fopen(file_path, "rb");
	if (file == NULL) {
		printf("load error: %s\n", file_path);
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	u32 file_size = ftell(file);
	if (file_size != correct_file_size) {
		printf("file size error: %s\n", file_path);
		printf("file_size = %u, correct_file_size = %u \n", file_size, correct_file_size);
		exit(1);
	}
}

void load_param(char *weight_file_dir, char *threshold_file_dir, char *threshold_file_dir1, char *threshold_file_dir2, char *fmap_file_dir){
	FILE *file;

	// load weight
	file = fopen(weight_file_dir, "rb");
	open_file(file, weight_file_dir, total_weight_size);
	for (i=0; i<LAYER_NUM; i++){
		if(weight[i] == NULL) printf("memory excess: weight[%u]\n", i);
		printf("load weight%d...\n", i);
		fread(weight[i], sizeof(char), weight_size[i], file);
		// printf("%f",((float *)weight[i]));
	}
	printf("load complete: %s\n", weight_file_dir);
	fclose(file);

	// // load threshold
	file = fopen(threshold_file_dir, "rb");
	open_file(file, threshold_file_dir, total_threshold_size2);
	for (i=0; i<LAYER_NUM; i++){
		if(threshold[i] == NULL) printf("memory excess: threshold[%u]\n", i);
		printf("load threshold%d...\n", i);
		if (i < LAYER_NUM) fread(threshold[i], sizeof(char), threshold_size2[i], file);
		else if (i == LAYER_NUM) fread(last_layer_BNparam, sizeof(char), threshold_size2[i], file);
	}
	printf("load complete: %s\n", threshold_file_dir);
	fclose(file);

	// load threshold1
	file = fopen(threshold_file_dir1, "rb");
	open_file(file, threshold_file_dir1, total_threshold_size);
	for (i=0; i<LAYER_NUM; i++){
		if(threshold1[i] == NULL) printf("memory excess: threshold[%u]\n", i);
		printf("load threshold%d...\n", i);
		if (i < LAYER_NUM) fread(threshold1[i], sizeof(char), threshold_size[i], file);
		else if (i == LAYER_NUM) fread(last_layer_BNparam, sizeof(char), threshold_size[i], file);
	}
	printf("load complete: %s\n", threshold_file_dir1);
	fclose(file);

	// load threshold2
	file = fopen(threshold_file_dir2, "rb");
	open_file(file, threshold_file_dir2, total_threshold_size);
	for (i=0; i<LAYER_NUM; i++){
		if(threshold2[i] == NULL) printf("memory excess: threshold[%u]\n", i);
		printf("load threshold%d...\n", i);
		if (i < LAYER_NUM) fread(threshold2[i], sizeof(char), threshold_size[i], file);
		else if (i == LAYER_NUM) fread(last_layer_BNparam, sizeof(char), threshold_size[i], file);
	}
	printf("load complete: %s\n", threshold_file_dir2);
	fclose(file);

	// load ifmaps
	file = fopen(fmap_file_dir, "rb");
	open_file(file, fmap_file_dir, total_ifmap_size);
	for (i=0; i<LAYER_NUM; i++){
		if(ifmaps_ref[i] == NULL) printf("memory excess: ifmaps[%u]\n", i);
		printf("load ifmaps%d...\n", i);
		fread(ifmaps_ref[i], sizeof(char), ifmap_size[i], file);
		// printf("%f",((float *)ifmaps_ref[i]));
	}
	printf("load complete: %s\n", fmap_file_dir);
	fclose(file);
}
// ----------------------------------------profile_functions----------------------------------------
void perf_end(clock_t start_time, double *total_processing_time, char *print_name){
	double processing_time = ((double)((clock()-start_time))/CLOCKS_PER_SEC); // ms
	printf("%s = %.4fs\n", print_name, processing_time);
	*total_processing_time = *total_processing_time + processing_time;
}


