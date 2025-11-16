// L1
#include <stdio.h>
#include <time.h>
#include "L1.h"

void calorie_calculate(int carb_g, int prot_g, int fat_g);
void first_boot;

//입력 프로필 베이스
typedef struct
{
	int carb_g;
	int prot_g;
	int fat_g;

	int carb_c;
	int prot_c;
	int fat_c;
} macro_base;





//첫 구동
int first_boot{
	float tmp_carb = 0;
	float tmp_prot = 0;
	float tmp_fat = 0;

	printf("please choose method to set your goal macronutrients & calories \n")
	printf("1. Enter Calorie & Auto calculate Macros \n")
	printf("2. Enter Macros & Auto calculate Calorie \n")
	scanf_s("%d", &trackmethod);
	
	// 칼로리 입력 - 매크로 자동 계산
	if (trackmethod == 1) {
		printf("Enter your goal calorie : ");
		scanf_s("%f", &goalcalorie);

		tmp_carb = goalcalorie * (4 / 10);
		tmp_prot = goalcalorie * (4 / 10);
		tmp_fat = goalcalorie * (2 / 10);		// 자동 프로필 탄단지 비율 4:4:2 
	}
	// 매크로 입력 - 칼로리 자동 계산
	if (trackmethod == 2) {
		printf("\nEnter your goal amount of carbohydrate : ");
		scanf_s("%f", &tmp_carb);
		printf("\nEnter your goal amount of protein : ");
		scanf_s("%f", &tmp_protein);
		printf("\nEnter your goal amount of fat : ");
		scanf_s("%f", &tmp_fat);
	}



}