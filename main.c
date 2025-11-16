// MAIN
#include<stdio.h>
#include <time.h>
#include "L1.h"
int firsttimeboot = 0;
int trackmethod = 0;
float goalcalorie = 0.0;





//	기능
//Day N 세팅 [오토리셋, 원하는 날짜로 바꾸기]
//칼로리 입력 | w1.수동  w2.식품
//입력된 칼로리 수정 | 0~9 per page
//총 섭취 칼로리 표시 (입력된 칼로리 기반
	
//	통계
//평균 데이터 | 주,달,년
//데이터 파일로 추출


int main() {
//	첫 구동용 기능:
//최대 탄, 단, 지 - 목표 칼로리 도출 or 탄단지비율 추천 (dev 추천 프로필
//키, 몸무게 입력 - bmi 도출
	if (firsttimeboot == 0) 
	{
	first_boot();
	firsttimeboot = 1;
	}
	



	return 0;
}




//첫 구동
float first_boot(){
	float tmp_carb = 0;
	float tmp_prot = 0;
	float tmp_fat = 0;

	printf("please choose method to set your goal macronutrients & calories \n");
		printf("1. Enter Calorie & Auto calculate Macros \n");
	printf("2. Enter Macros & Auto calculate Calorie \n");
	scanf_s("%d", &trackmethod);
	
	// 칼로리 입력 - 매크로 자동 계산 [미구현]
	if (trackmethod == 1) {
		printf("Enter your goal calorie : ");
		scanf_s("%f", &goalcalorie);
	
		tmp_carb = goalcalorie * 0.4f * 0.4f;
		tmp_prot = goalcalorie * 0.4f * 0.4f;
		tmp_fat = goalcalorie * 0.2f * 0.8f;			// 자동 프로필 탄단지 비율 4:4:2 
		
		//temp!
		printf("c: %f / p: %f / f: %f", tmp_carb, tmp_prot, tmp_fat);
	}
	// 매크로 입력 - 칼로리 자동 계산
	if (trackmethod == 2) {
		printf("\nEnter your goal amount of carbohydrate : ");
		scanf_s("%f", &tmp_carb);

		printf("\nEnter your goal amount of protein : ");
		scanf_s("%f", &tmp_prot);
		
		printf("\nEnter your goal amount of fat : ");
		scanf_s("%f", &tmp_fat);
		
		goalcalorie = (tmp_carb * 4.0) + (tmp_prot * 4.0) + (tmp_fat * 9.0);
		//temp!
		printf("\ncalculated goal calorie %.1f: \n", goalcalorie);
	}


	

	return 0;
}