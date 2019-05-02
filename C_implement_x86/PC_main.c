#include "PC_function.h"

void teardown(int exit_status) {
	exit(exit_status);
}
void cleanup() {
	// Called from aocl_utils::check_error, so there's an error.
	teardown(-1);
}
void load_fire_data(float *test_sets) {
	FILE *file;
	file = fopen("/userhome/urp/yolo/fire3.bin", "rb");
	if (file == NULL) printf("load error: test_sets\n");
	fread(test_sets, sizeof(float), (112 * 112 * 3) * NUM_TEST_SETS, file);
	fclose(file);

}
void load_weight(float *weight_1, float *weight_2, float *weight_3, float *weight_4, float *weight_5, float *weight_6) {
	FILE *file;
	// conv1
	file = fopen("/userhome/urp/yolo_lighter/w_1.bin", "rb");
	if (file == NULL) printf("load error: weight_1\n");
	fread(weight_1, sizeof(float), 108, file);

	// block1_1
	file = fopen("/userhome/urp/yolo_lighter/w_2.bin", "rb");
	if (file == NULL) printf("load error: weight_2\n");
	fread(weight_2, sizeof(float), 288, file);

	file = fopen("/userhome/urp/yolo_lighter/w_3.bin", "rb");
	if (file == NULL) printf("load error: weight_3\n");
	fread(weight_3, sizeof(float), 1152, file);
	//block1_2
	file = fopen("/userhome/urp/yolo_lighter/w_4.bin", "rb");
	if (file == NULL) printf("load error: weight_4\n");
	fread(weight_4, sizeof(float), 4608, file);

	file = fopen("/userhome/urp/yolo_lighter/w_5.bin", "rb");
	if (file == NULL) printf("load error: weight_5\n");
	fread(weight_5, sizeof(float), 1024, file);
	fclose(file);
	//block1_3
	file = fopen("/userhome/urp/yolo_lighter/w_6.bin", "rb");
	if (file == NULL) printf("load error: weight_4\n");
	fread(weight_6, sizeof(float), 32, file);
	fclose(file);

}
void load_beta(float *beta_1, float *beta_2, float *beta_3, float *beta_4, float *beta_5) {
	FILE *file;
	//conv1
	file = fopen("/userhome/urp/yolo_lighter/b_1.bin", "rb");
	if (file == NULL) printf("load error: bias_1\n");
	fread(beta_1, sizeof(float), 4, file);
	//block1_1
	file = fopen("/userhome/urp/yolo_lighter/b_2.bin", "rb");
	if (file == NULL) printf("load error: bias_2\n");
	fread(beta_2, sizeof(float), 8, file);

	file = fopen("/userhome/urp/yolo_lighter/b_3.bin", "rb");
	if (file == NULL) printf("load error: bias_3\n");
	fread(beta_3, sizeof(float), 16, file);
	//block1_2
	file = fopen("/userhome/urp/yolo_lighter/b_4.bin", "rb");
	if (file == NULL) printf("load error: bias_4\n");
	fread(beta_4, sizeof(float), 32, file);

	file = fopen("/userhome/urp/yolo_lighter/b_5.bin", "rb");
	if (file == NULL) printf("load error: bias_5\n");
	fread(beta_5, sizeof(float), 32, file);
	fclose(file);
}
void load_gamma(float *gamma_1, float *gamma_2, float *gamma_3, float *gamma_4, float *gamma_5) {
	FILE *file;
	//conv1
	file = fopen("/userhome/urp/yolo_lighter/g_1.bin", "rb");
	if (file == NULL) printf("load error: bias_1\n");
	fread(gamma_1, sizeof(float), 4, file);
	//block1_1
	file = fopen("/userhome/urp/yolo_lighter/g_2.bin", "rb");
	if (file == NULL) printf("load error: bias_2\n");
	fread(gamma_2, sizeof(float), 8, file);

	file = fopen("/userhome/urp/yolo_lighter/g_3.bin", "rb");
	if (file == NULL) printf("load error: bias_3\n");
	fread(gamma_3, sizeof(float), 16, file);
	//block1_2
	file = fopen("/userhome/urp/yolo_lighter/g_4.bin", "rb");
	if (file == NULL) printf("load error: bias_4\n");
	fread(gamma_4, sizeof(float), 32, file);

	file = fopen("/userhome/urp/yolo_lighter/g_5.bin", "rb");
	if (file == NULL) printf("load error: bias_5\n");
	fread(gamma_5, sizeof(float), 32, file);
	fclose(file);
}
void load_mov_mean(float *mov_mean_1, float *mov_mean_2, float *mov_mean_3, float *mov_mean_4, float *mov_mean_5) {
	FILE *file;
	//conv1
	file = fopen("/userhome/urp/yolo_lighter/mm_1.bin", "rb");
	if (file == NULL) printf("load error: bias_1\n");
	fread(mov_mean_1, sizeof(float), 4, file);
	//block1_1
	file = fopen("/userhome/urp/yolo_lighter/mm_2.bin", "rb");
	if (file == NULL) printf("load error: bias_2\n");
	fread(mov_mean_2, sizeof(float), 8, file);

	file = fopen("/userhome/urp/yolo_lighter/mm_3.bin", "rb");
	if (file == NULL) printf("load error: bias_3\n");
	fread(mov_mean_3, sizeof(float), 16, file);
	//block1_2
	file = fopen("/userhome/urp/yolo_lighter/mm_4.bin", "rb");
	if (file == NULL) printf("load error: bias_4\n");
	fread(mov_mean_4, sizeof(float), 32, file);

	file = fopen("/userhome/urp/yolo_lighter/mm_5.bin", "rb");
	if (file == NULL) printf("load error: bias_5\n");
	fread(mov_mean_5, sizeof(float), 32, file);
	fclose(file);
}
void load_mov_var(float *mov_var_1, float *mov_var_2, float *mov_var_3, float *mov_var_4, float *mov_var_5) {
	FILE *file;
	//conv1
	file = fopen("/userhome/urp/yolo_lighter/mv_1.bin", "rb");
	if (file == NULL) printf("load error: bias_1\n");
	fread(mov_var_1, sizeof(float), 4, file);
	//block1_1
	file = fopen("/userhome/urp/yolo_lighter/mv_2.bin", "rb");
	if (file == NULL) printf("load error: bias_2\n");
	fread(mov_var_2, sizeof(float), 8, file);

	file = fopen("/userhome/urp/yolo_lighter/mv_3.bin", "rb");
	if (file == NULL) printf("load error: bias_3\n");
	fread(mov_var_3, sizeof(float), 16, file);
	//block1_2
	file = fopen("/userhome/urp/yolo_lighter/mv_4.bin", "rb");
	if (file == NULL) printf("load error: bias_4\n");
	fread(mov_var_4, sizeof(float), 32, file);

	file = fopen("/userhome/urp/yolo_lighter/mv_5.bin", "rb");
	if (file == NULL) printf("load error: bias_5\n");
	fread(mov_var_5, sizeof(float), 32, file);
	fclose(file);
}
int main() {
	// fire data
	float *test_sets = (float *)malloc(112 * 112 * 3 * NUM_TEST_SETS * sizeof(float));
	int crt[100] = { 0 };
	int false_1[600] = { 0 };
	int false_2[200] = { 0 };

	// weight
	float *weight_1 = (float *)malloc(108 * sizeof(float));
	float *weight_2 = (float *)malloc(288 * sizeof(float));
	float *weight_3 = (float *)malloc(1152 * sizeof(float));
	float *weight_4 = (float *)malloc(4608 * sizeof(float));
	float *weight_5 = (float *)malloc(1024 * sizeof(float));
	float *weight_6 = (float *)malloc(32 * sizeof(float));
	

	// beta
	float *beta_1 = (float *)malloc(4 * sizeof(float));
	float *beta_2 = (float *)malloc(8 * sizeof(float));
	float *beta_3 = (float *)malloc(16 * sizeof(float));
	float *beta_4 = (float *)malloc(32 * sizeof(float));
	float *beta_5 = (float *)malloc(32 * sizeof(float));
	

	// gamma
	float *gamma_1 = (float *)malloc(4 * sizeof(float));
	float *gamma_2 = (float *)malloc(8 * sizeof(float));
	float *gamma_3 = (float *)malloc(16 * sizeof(float));
	float *gamma_4 = (float *)malloc(32 * sizeof(float));
	float *gamma_5 = (float *)malloc(32 * sizeof(float));
	

	// mov_mean
	float *mov_mean_1 = (float *)malloc(4 * sizeof(float));
	float *mov_mean_2 = (float *)malloc(8 * sizeof(float));
	float *mov_mean_3 = (float *)malloc(16 * sizeof(float));
	float *mov_mean_4 = (float *)malloc(32 * sizeof(float));
	float *mov_mean_5 = (float *)malloc(32 * sizeof(float));
	

	// mov_var
	float *mov_var_1 = (float *)malloc(4 * sizeof(float));
	float *mov_var_2 = (float *)malloc(8 * sizeof(float));
	float *mov_var_3 = (float *)malloc(16 * sizeof(float));
	float *mov_var_4 = (float *)malloc(32 * sizeof(float));
	float *mov_var_5 = (float *)malloc(32 * sizeof(float));
	

	// variables
	int testing_results[NUM_TEST_SETS] = { 0 };

	load_fire_data(test_sets);
	load_beta(beta_1, beta_2, beta_3, beta_4, beta_5);
	load_gamma(gamma_1, gamma_2, gamma_3, gamma_4, gamma_5);
	load_mov_mean(mov_mean_1, mov_mean_2, mov_mean_3, mov_mean_4, mov_mean_5);
	load_mov_var(mov_var_1, mov_var_2, mov_var_3, mov_var_4, mov_var_5);
	load_weight(weight_1, weight_2, weight_3, weight_4, weight_5, weight_6);
	float start_time = clock();
	float *buffer_1 = (float *)malloc(112 * 112 * 16 * sizeof(float));
	float *buffer_2 = (float *)malloc(112 * 112 * 16 * sizeof(float));
	float p_time_1 = 0;
	float f_time_1 = 0;
	float p_time_2 = 0;
	float f_time_2 = 0;
	float p_time_3 = 0;
	float f_time_3 = 0;
	float p_time_4 = 0;
	float f_time_4 = 0;
	float p_time_5 = 0;
	float f_time_5 = 0;
	float p_time_6 = 0;
	float f_time_6 = 0;
	float runningtime_1 = 0;
	float runningtime_2 = 0;
	float runningtime_3 = 0;
	float runningtime_4 = 0;
	float runningtime_5 = 0;
	float runningtime_6 = 0;

	for (int j = 0; j < NUM_TEST_SETS; j++) {
		for (int d = 0; d < 3; d++) {
			for (int i = 0; i < 112; i++) {
				for (int k = 0; k < 112; k++) {
					buffer_1[(d * 112 * 112) + (i * 112) + k] = test_sets[d + (k * 3) + (i * 3 * 112) + (j * 3 * 112 * 112)] / 255;
				}
			}
		}
		// conv1
		p_time_1 = clock();
		convolution(buffer_1, buffer_2, weight_1, 112, 3, 3, 4, 1);
		leaky(buffer_2, buffer_2, 112 * 112 * 4);
		bn(buffer_2, buffer_2, mov_mean_1, mov_var_1, beta_1, gamma_1, 112, 4);
		max_pooling(buffer_2, buffer_1, 112, 4, 2);
		f_time_1 = clock();
		runningtime_1 = runningtime_1 + (f_time_1 - p_time_1);

		// conv2
		p_time_2 = clock();
		convolution(buffer_1, buffer_2, weight_2, 56, 4, 3, 8, 1);
		leaky(buffer_2, buffer_2, 56 * 56 * 8);
		bn(buffer_2, buffer_2, mov_mean_2, mov_var_2, beta_2, gamma_2, 56, 8);
		max_pooling(buffer_2, buffer_1, 56, 8, 2);
		f_time_2 = clock();
		runningtime_2 = runningtime_2 + (f_time_2 - p_time_2);

		// conv3
		p_time_3 = clock();
		convolution(buffer_1, buffer_2, weight_3, 28, 8, 3, 16, 1);
		leaky(buffer_2, buffer_2, 28 * 28 * 16);
		bn(buffer_2, buffer_2, mov_mean_3, mov_var_3, beta_3, gamma_3, 28, 16);
		max_pooling(buffer_2, buffer_1, 28, 16, 2);
		f_time_3 = clock();
		runningtime_3 = runningtime_3 + (f_time_3 - p_time_3);

		// conv4
		p_time_4 = clock();
		convolution(buffer_1, buffer_2, weight_4, 14, 16, 3, 32, 1);
		leaky(buffer_2, buffer_2, 14 * 14 * 32);
		bn(buffer_2, buffer_2, mov_mean_4, mov_var_4, beta_4, gamma_4, 14, 32);
		max_pooling(buffer_2, buffer_1, 14, 32, 2);
		f_time_4 = clock();
		runningtime_4 = runningtime_4 + (f_time_4 - p_time_4);

		// conv5
		p_time_5 = clock();
		convolution(buffer_1, buffer_2, weight_5, 7, 32, 1, 32, 1);
		leaky(buffer_2, buffer_2, 7 * 7 * 32);
		bn(buffer_2, buffer_2, mov_mean_5, mov_var_5, beta_5, gamma_5, 7, 32);
		f_time_5 = clock();
		runningtime_5 = runningtime_5 + (f_time_5 - p_time_5);

		// FC1
		p_time_6 = clock();
		convolution(buffer_2, buffer_1, weight_6, 7, 32, 1, 1, 1);
		clip(buffer_1, 49);
		f_time_6 = clock();
		runningtime_6 = runningtime_6 + (f_time_6 - p_time_6);
		for (int i = 0; i < 49; i++) {
			if (buffer_1[i] > 0.3) printf("1    ");
			else printf("0    ");
		}
		/*for (int i = 0; i < 49; i++) {
			printf("%.3f ", buffer_1[i]);
		}
		printf("\n");*/
		/*if (j < 100) crt[j] = correct(buffer_2);
		else if (j < 700) false_1[j - 100] = correct(buffer_2);
		else false_2[j - 700] = correct(buffer_2);*/
																			/*printf("\n");
																			for (int i = 0; i < 10; i++) printf("%f ", buffer_2[i]);*/
																			//printf("// result: %d, label: %d \n", testing_results[j], labels[j]);
	}
	runningtime_1 = runningtime_1 / CLOCKS_PER_SEC;
	runningtime_2 = runningtime_2 / CLOCKS_PER_SEC;
	runningtime_3 = runningtime_3 / CLOCKS_PER_SEC;
	runningtime_4 = runningtime_4 / CLOCKS_PER_SEC;
	runningtime_5 = runningtime_5 / CLOCKS_PER_SEC;
	runningtime_6 = runningtime_6 / CLOCKS_PER_SEC;

	printf("\nRunning Time_1 : %fsec \n", runningtime_1);
	printf("\nRunning Time_2 : %fsec \n", runningtime_2);
	printf("\nRunning Time_3 : %fsec \n", runningtime_3);
	printf("\nRunning Time_4 : %fsec \n", runningtime_4);
	printf("\nRunning Time_5 : %fsec \n", runningtime_5);
	printf("\nRunning Time_6 : %fsec \n", runningtime_6);

	printf("Running Time : %lfsec\n", (clock() - start_time) / CLOCKS_PER_SEC);
	/*int sum = 0;
	for (int i = 0; i < 100; i++) sum += crt[i];
	printf("Correct Alarm: %.2f\n", (float)sum / 1);
	sum = 0;
	for (int i = 0; i < 600; i++) sum += false_1[i];
	printf("False_1 Alarm: %.2f\n", (float)sum / 6);
	sum = 0;
	for (int i = 0; i < 200; i++) sum += false_2[i];
	printf("False_2 Alarm: %.2f\n", (float)sum / 2);*/
	return 0;
}