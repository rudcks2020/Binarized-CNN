#include "bnn_utils.h"
#include "common_utils.h"
#include "PC_function.h"
#include <unistd.h>
#include <time.h>

int main(int argc, char* args[]) {
	printf("12");
	network_config(lp);
	print_network(lp);
	mem_allocs();
	load_param(WEIGHT_FILE_DIR, THRESHOLD_FILE_DIR, THRESHOLD_FILE_DIR1, THRESHOLD_FILE_DIR2, FMAP_FILE_DIR);

	//	unsigned long layer_cycles = 0;
	// clock_t start_time, end_time, total_t;
	u32 i;
	u32 layer_cnt = 0;
	int early_termination = 0;

	//	double total_processing_time = 0;
	// 1st layer input load to fmap_buffer1
	printf("memcpy complete");
	memcpy(fmap_buffer1, ifmaps_ref[layer_cnt], ifmap_size[layer_cnt]*sizeof(char));
	printf("memcpy complete");
//1st
	convolution_first(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 32, 3, 3, 128, 1);
	bn(fmap_buffer2, fmap_buffer2, threshold2[layer_cnt], threshold1[layer_cnt], 32, 128);
	relu(fmap_buffer2, fmap_buffer2, 32 * 32 * 128);
	bn(fmap_buffer2, fmap_buffer2, threshold2[layer_cnt+1], threshold1[layer_cnt+1], 32, 128);
	bin_active(fmap_buffer2, fmap_buffer1, 32 * 32 * 128);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//2nd
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 32, 128, 3, 128, 1);
	max_pooling(fmap_buffer2, fmap_buffer1, 32, 128, 2);
	bn(fmap_buffer1, fmap_buffer2, threshold2[layer_cnt+1], threshold1[layer_cnt+1], 16, 128);
	bin_active(fmap_buffer2, fmap_buffer1, 16 * 16 * 128);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//3rd
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 16, 128, 3, 256, 1);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 16, 256);
	bin_active(fmap_buffer2, fmap_buffer1, 16 * 16 * 256);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//4th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 16, 256, 3, 256, 1);
	max_pooling(fmap_buffer2, fmap_buffer2, 16, 256, 2);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 8, 256);
	bin_active(fmap_buffer2, fmap_buffer1, 8 * 8 * 256);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//5th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 8, 256, 3, 512, 1);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 8, 512);
	bin_active(fmap_buffer2, fmap_buffer1, 8 * 8 * 512);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//6th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 8, 512, 3, 512, 1);
	max_pooling(fmap_buffer2, fmap_buffer2, 8, 512, 2);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 4, 512);
	bin_active(fmap_buffer2, fmap_buffer1, 4 * 4 * 512);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//7th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 4, 512, 4, 1024, 1);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 1, 1024);
	bin_active(fmap_buffer2, fmap_buffer1, 1 * 1 * 1024);

	print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;

//8th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 1, 1024, 1, 1024, 1);
	bn(fmap_buffer2, fmap_buffer2,threshold2[layer_cnt+1], threshold1[layer_cnt+1], 1, 1024);
	relu(fmap_buffer2, fmap_buffer1, 1 * 1 * 1024);
	// print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer1, lp[layer_cnt]);
	layer_cnt++;
 	
//9th
	convolution(fmap_buffer1, fmap_buffer2, weight[layer_cnt], 1, 1024, 1, 10, 1);
	// print_num_of_diff_fmap(ifmaps_ref[layer_cnt+1], fmap_buffer2, lp[layer_cnt]);
	softmax(fmap_buffer1, fmap_buffer2, lp[layer_cnt].output_fmap_channel);
	// layer_cnt++;
	printf("\n\n\n\n");
	printf("==========================Softmax result========================\n");

	float output_value = 0;
	int output_index = 0;

	for(i=0; i<10; i++) printf("%d th output = %f\n", i, ((float *)fmap_buffer1)[i]);

	for(i=0; i<lp[LAYER_NUM-1].output_fmap_channel; i++){
		if (((float *)fmap_buffer2)[i] > output_value){
			output_value = ((float *)fmap_buffer1)[i];
			output_index = i;
		}
	}
	printf("float = %f\n", output_value);
	printf("index = %d\n", output_index);

	mem_free();

	printf("================================Done============================\n");


 	return 0;
	// final result on fmap_buffer1

}
