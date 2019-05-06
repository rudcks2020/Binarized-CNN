#include <stdio.h>
#include <windows.h>
#include "Operation.h"
#include "Network_config.h"
#include "Memory.h"

int main(){

	layer_configuration(lc);
	print_network(lc);
	mem_cal(lc);
	mem_alloc();
	load_param(WEIGHT_FILE_DIR, THRESHOLD_FILE_DIR, THRESHOLD_FILE_DIR1, THRESHOLD_FILE_DIR2, FMAP_FILE_DIR);
	system("pause");

}