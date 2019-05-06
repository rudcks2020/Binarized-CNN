#include <stdio.h>
#include "Network_config.h"

/*=================================================================================================================*/
/* =================================================Layer Configuration============================================*/
/*=================================================================================================================*/

void layer_configuration(layer_config *lc){
	lc[0].input_fmap_width 	 = INPUT_IMAGE_WIDTH;
	lc[0].input_fmap_height  = INPUT_IMAGE_HEIGHT;
	lc[0].input_fmap_channel = INPUT_IMAGE_CHANNEL;

	lc[0].filter_size = 3;		lc[0].output_fmap_channel = 128;
	lc[1].filter_size = 3;		lc[1].output_fmap_channel = 128;
	lc[2].filter_size = 3;		lc[2].output_fmap_channel = 256;
	lc[3].filter_size = 3;		lc[3].output_fmap_channel = 256;
	lc[4].filter_size = 3;		lc[4].output_fmap_channel = 512;
	lc[5].filter_size = 3;		lc[5].output_fmap_channel = 512;
	lc[6].filter_size = 4;		lc[6].output_fmap_channel = 1024;
	lc[7].filter_size = 1;		lc[7].output_fmap_channel = 1024;
	lc[8].filter_size = 1;		lc[8].output_fmap_channel = 10;

	lc[0].conv_stride = 1; 		lc[0].conv_pad = 1;		lc[0].quantize_bit = 1; 
	lc[1].conv_stride = 1; 		lc[1].conv_pad = 1;		lc[1].quantize_bit = 1; 
	lc[2].conv_stride = 1; 		lc[2].conv_pad = 1;		lc[2].quantize_bit = 1; 
	lc[3].conv_stride = 1; 		lc[3].conv_pad = 1;		lc[3].quantize_bit = 1; 
	lc[4].conv_stride = 1; 		lc[4].conv_pad = 1;		lc[4].quantize_bit = 1; 
	lc[5].conv_stride = 1; 		lc[5].conv_pad = 1;		lc[5].quantize_bit = 1; 
	lc[6].conv_stride = 1; 		lc[6].conv_pad = 0;		lc[6].quantize_bit = 1; 
	lc[7].conv_stride = 1; 		lc[7].conv_pad = 0;		lc[7].quantize_bit = 1; 
	lc[8].conv_stride = 1; 		lc[8].conv_pad = 0;		lc[8].quantize_bit = 1; 

	lc[0].pool_stride = 1;		lc[0].pool_pad = 0;		lc[0].pool_size = 1;
	lc[1].pool_stride = 2;		lc[1].pool_pad = 0;		lc[1].pool_size = 2;
	lc[2].pool_stride = 1;		lc[2].pool_pad = 0;		lc[2].pool_size = 1;
	lc[3].pool_stride = 2;		lc[3].pool_pad = 0;		lc[3].pool_size = 2;
	lc[4].pool_stride = 1;		lc[4].pool_pad = 0;		lc[4].pool_size = 1;
	lc[5].pool_stride = 2;		lc[5].pool_pad = 0;		lc[5].pool_size = 2;
	lc[6].pool_stride = 1;		lc[6].pool_pad = 0;		lc[6].pool_size = 1;
	lc[7].pool_stride = 1;		lc[7].pool_pad = 0;		lc[7].pool_size = 1;
	lc[8].pool_stride = 1;		lc[8].pool_pad = 0;		lc[8].pool_size = 1;

	for(int i = 0 ; i < LAYER_NUM ; i++){
		if(i == 0)
			lc[i].layer_type = FIBO;
		else if(i == LAYER_NUM-2)
			lc[i].layer_type = BIFO;
		else if(i == LAYER_NUM-1)
			lc[i].layer_type = FIFO;
		else
			lc[i].layer_type = BIBO;

		if(i != 0){
			lc[i].input_fmap_height 	= lc[i-1].output_fmap_height;
			lc[i].input_fmap_width 		= lc[i-1].output_fmap_width;
			lc[i].input_fmap_channel 	= lc[i-1].output_fmap_channel;
		}

		lc[i].output_fmap_height = ((lc[i].input_fmap_height + lc[i].conv_pad * 2) - (lc[i].filter_size - 1)) / lc[i].pool_size;
		lc[i].output_fmap_width  = ((lc[i].input_fmap_width + lc[i].conv_pad * 2) - (lc[i].filter_size - 1)) / lc[i].pool_size;
	}

}

void print_network(layer_config *lc){
	printf("===========================================================print_network_configuration========================================================\n");
	printf("calculating....\n");
	for(int i = 0 ; i < LAYER_NUM ; i++){
		printf("Layer[%d]'s Configuration\n" , i);
		printf("Input_Channel : %-8u  Input_Width : %-8u Input_Height : %-8u " , lc[i].input_fmap_channel, lc[i].input_fmap_width , lc[i].input_fmap_height);
		printf("  Output_Channel : %-8u  Output_Width : %-8u Output_Height : %-8u \n\n", lc[i].output_fmap_channel, lc[i].output_fmap_width , lc[i].output_fmap_height);
	}
}
