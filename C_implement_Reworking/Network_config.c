#include "Operation.h"
#include "Network_config.h"
#include "Memory.h"

/*=========================================================================*/
/* ===========================Layer Configuration==========================*/
/*=========================================================================*/

void layer_configuration(layer_config *lc){
	lc[0].input_fmap_width 	 = INPUT_IMAGE_WIDTH;
	lc[0].input_fmap_height  = INPUT_IMAGE_HEIGHT;
	lc[0].input_fmap_channel = INPUT_IMAGE_CHANNEL;

	lc[0].filter_size = 3;	lc[0].output_fmap_chanel = 128;
	lc[1].filter_size = 3;	lc[1].output_fmap_chanel = 128;
	lc[2].filter_size = 3;	lc[2].output_fmap_chanel = 128;
	lc[3].filter_size = 3;	lc[3].output_fmap_chanel = 128;
	lc[4].filter_size = 3;	lc[4].output_fmap_chanel = 128;
	lc[5].filter_size = 3;	lc[5].output_fmap_chanel = 128;
	lc[6].filter_size = 3;	lc[6].output_fmap_chanel = 128;
	lc[7].filter_size = 3;	lc[7].output_fmap_chanel = 128;
	lc[8].filter_size = 3;	lc[8].output_fmap_chanel = 128;

}