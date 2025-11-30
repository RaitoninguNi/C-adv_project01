// MAIN
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <windows.h>
#include <time.h>

	int firsttimeboot = 1;
	int trackmethod = 0;
	int UpdateDate = 0;
	int noRS = 0;
	int Curr_Y, Curr_M, Curr_D;
	int Temp_Y, Temp_M, Temp_D;
	float Curr_cal, Curr_carb, Curr_prot, Curr_fat = 0.0;
	int minuskcal = 0;
	int day = 1;
	int DayChk = 0;
	float goalcalorie = 0.0;
	int Genboot = 0;

	typedef struct {
		float carb;
		float prot;
		float fat;
		float goalcal;
	} CalorieProfile;
	CalorieProfile GeneralProfile;

	int first_boot();
	int CurrentDay();
	void TimeDiffSync();
	int GeneralConsole();
	void InputMacro();
	void KcalMinus();
	void SaveData();
	int LoadData();
	void SaveLogs();
	void ResetLogs();
	void DayCheck();
	void ResetScreen();
	int GetEnter();
//함수 메인	
int main() {
	int LoadStat = LoadData();
	if (firsttimeboot == 1 || LoadStat == 2) {	// 성공적이지 않은 로딩 or 첫 부팅
		printf("기본 설정이 인식되지않아 기본설정 단계로 넘어갑니다...\n");
		CurrentDay();
		noRS = 1;
		first_boot();
		firsttimeboot = 0;
		SaveData();	
	}
										//콘솔 부팅 전 데이체크
	if (LoadStat == 1 || firsttimeboot == 0) {		//성공적인 로딩
		while (1) {
			GeneralConsole();	//콘솔 연결
		}
	}

	printf("\t-------- main 정상 종료 확인 --------\t");
	return 0;
}
//첫 구동
int first_boot(){
	float tmp_carb = 0;
	float tmp_prot = 0;
	float tmp_fat = 0;
	
	ResetScreen();
	printf("목표 탄단지 혹은 칼로리를 설정하기 위한 방법을 선택해주세요.\n");
		printf("1. 목표 칼로리 입력 -> 목표 탄단지 자동 배분 \n");
		printf("2. 목표 탄단지 입력 -> 목표 칼로리 자동 배분 \n");
		if (Genboot == 1) {
			printf("3. 뒤로가기 \n");
		}
			scanf_s("%d", &trackmethod);
	
	
	if (trackmethod == 1) {			//방법 1 : 칼로리 입력 - 매크로 자동 계산
		ResetScreen();
		printf("\nEnter your goal calorie : ");
		scanf_s("%f", &goalcalorie);
	
		tmp_carb = (goalcalorie * 0.4f) / 4.0f;
		tmp_prot = (goalcalorie * 0.4f) / 4.0f;
		tmp_fat = (goalcalorie * 0.2f) / 9.0f;		// 자동 프로필의 탄단지 비율 4:4:2 		
		
		ResetScreen();
		printf("\n권장되는 식단 영양 비율 : \n탄수화물: %.1fg | 단백질: %.1fg | 지방: %.1fg \n", tmp_carb, tmp_prot, tmp_fat);
	}

	if (trackmethod == 2) {			//방법 2 :  매크로 입력 - 칼로리 자동 계산
		ResetScreen();
		printf("\n탄수화물의 목표 섭취량(g)을 적어주세요 : ");
			scanf_s("%f", &tmp_carb);
		printf("\n단백질의 목표 섭취량(g)을 적어주세요 : ");
			scanf_s("%f", &tmp_prot);
		printf("\n지방의 목표 섭취량(g)을 적어주세요 : ");
			scanf_s("%f", &tmp_fat);
		ResetScreen();
		goalcalorie = (tmp_carb * 4.0) + (tmp_prot * 4.0) + (tmp_fat * 9.0);
			printf("\n계산된 목표 칼로리 : %.1f kcal \n", goalcalorie);
			
	}							
	if (trackmethod == 3) {
		Genboot = 0;
		return;
	}
	//전역변수로 옮기기
	GeneralProfile.carb = tmp_carb;
	GeneralProfile.prot = tmp_prot;
	GeneralProfile.fat = tmp_fat;
	GeneralProfile.goalcal = goalcalorie;
	GetEnter();
}
//현재 시간 계산														|	Temp는 앱의 표시 일자, Curr은 인식되는 현시점의 일자
int CurrentDay() {
	time_t now = time(NULL);
	struct tm* t = localtime(&now);
	
	Curr_Y = t->tm_year + 1900;
	Curr_M = t->tm_mon + 1;
	Curr_D = t->tm_mday;
	
	if (firsttimeboot == 1) {	//앱의 표시 일자 세팅
		Temp_Y = Curr_Y;
		Temp_M = Curr_M;
		Temp_D = Curr_D;
	}
	if (Temp_D != Curr_D || Temp_M != Curr_M || Temp_Y != Curr_Y) {	
		int TempAns = 0;
		printf("현재 일자와 앱의 일자가 다릅니다, 현재 일자로 변경하시겠습니까?\n1.현재 일자로 변경하기\n2.변경하지 않기\n");
		scanf_s("%d", &TempAns);
		
		if (TempAns == 1){
			SaveData();
			SaveLogs();
			ResetLogs();
			TimeDiffSync();
		}
		if (TempAns == 2) {
			DayChk = 1;
		}
	}
}
// 시간 싱크 맞추기
void TimeDiffSync() {
	struct tm temp_time = { 0 };
	temp_time.tm_year = Temp_Y - 1900;
	temp_time.tm_mon = Temp_M - 1;
	temp_time.tm_mday = Temp_D;

	struct tm curr_time = { 0 };
	curr_time.tm_year = Curr_Y - 1900;
	curr_time.tm_mon = Curr_M - 1;
	curr_time.tm_mday = Curr_D;

	time_t temp_sec = mktime(&temp_time);
	time_t curr_sec = mktime(&curr_time);

	double sec_diff = curr_sec - temp_sec;
	int day_diff = (int)(sec_diff / (60 * 60 * 24));

	day += day_diff;
	Temp_Y = Curr_Y;
	Temp_M = Curr_M;
	Temp_D = Curr_D;
	printf("업데이트 된 일차 : Day %d\n\n", day);
}
//메인 콘솔 [미완]
int GeneralConsole(){
	int TempConAns = 0;
	if (DayChk == 0) { CurrentDay(); }
	ResetScreen();

	printf("| Day %d | %d년 %d월 %d일 |\n",day, Temp_Y, Temp_M, Temp_D);
	if (minuskcal > 0) {
		printf("\n| 탄: %.1f / %.1f g | 단: %.1f / %.1f g | 지: %.1f / %.1f g |\n| %.1f \033[31m(-%d)\033[0m / %.1f kcal |"
			, Curr_carb, GeneralProfile.carb, Curr_prot, GeneralProfile.prot, Curr_fat, GeneralProfile.fat, Curr_cal, minuskcal, GeneralProfile.goalcal);
	}
	else {
		printf("\n| 탄: %.1f / %.1f g | 단: %.1f / %.1f g | 지: %.1f / %.1f g |\n| %.1f / %.1f kcal |"
			, Curr_carb, GeneralProfile.carb, Curr_prot, GeneralProfile.prot, Curr_fat, GeneralProfile.fat, Curr_cal, GeneralProfile.goalcal);
	}
	if (Curr_cal > GeneralProfile.goalcal) {
		printf("\033[31m !!목표 칼로리 초과!! \033[0m|");
	}
	printf("\n");
	printf("\n기능을 선택해주세요.\n");
	printf("1. 탄단지 입력 \n");
	printf("2. 소모 칼로리 입력 \n");
	printf("3. 목표 칼로리 재설정 \n");
	printf("0. 저장 및 종료 \t(저장되지 않은 정보는 사라집니다) \n");
	scanf_s("%d", &TempConAns);
	switch (TempConAns) {
	case 0:
		SaveData();
		exit(0);
	case 1:
		InputMacro();
		break;
	case 2:
		KcalMinus();
		break;
	case 3:
		Genboot = 1;
		first_boot();
		break;
	}
}
//식품(탄단지) 입력
void InputMacro(){
	ResetScreen();
	float TempAns3 = 0.0;
	
	printf("(뒤로 가려면 -1을, 특정값을 줄이려면 음수를 입력하세요.)\n");
	printf("음식의 탄수화물 함량을 입력해주세요(g) : ");
	scanf_s("%f", &TempAns3);
	if (TempAns3 == -1) {
		return;
	}
	Curr_carb += TempAns3;
	
	printf("\n음식의 단백질 함량을 입력해주세요(g) : ");
	scanf_s("%f", &TempAns3);
	Curr_prot += TempAns3;
	
	printf("\n음식의 지방 함량을 입력해주세요(g) : ");
	scanf_s("%f", &TempAns3);
	Curr_fat += TempAns3;

	Curr_cal = (Curr_carb * 4.0) + (Curr_prot * 4.0) + (Curr_fat * 9.0);
} 
//칼로리 -표시 추가
void KcalMinus() {
	ResetScreen();
	int tempn = 0;
	printf("소모한 칼로리를 입력해 주세요 : ");
	scanf_s("%d",&tempn);
	minuskcal += tempn;
}
//데이체크 트리거
void DayCheck() {
	if (DayChk == 0) {
		CurrentDay();
		DayChk = 1;
	}
}
//데이터(셋팅) 세이브
void SaveData() {
	SaveLogs();
	FILE* fp = fopen("usersettings.txt", "w");
	fprintf(fp, "%d\n", firsttimeboot);
	fprintf(fp, "%d %d %d %d\n",Temp_Y, Temp_M, Temp_D ,day);
	fprintf(fp, "%f %f %f %f\n",GeneralProfile.carb , GeneralProfile.prot, GeneralProfile.fat, GeneralProfile.goalcal);
	fprintf(fp, "%f %f %f %f\n",Curr_carb , Curr_prot, Curr_fat, Curr_cal);
	fprintf(fp, "%d\n", minuskcal);
	fclose(fp);
}
//셋팅 데이터 불러오기
int LoadData() {
	FILE* fp = fopen("usersettings.txt", "r");
	if (fp == NULL) {
		return 2;
	}

	fscanf(fp, "%d\n", &firsttimeboot);
	fscanf(fp, "%d %d %d %d\n", &Temp_Y, &Temp_M, &Temp_D, &day);
	fscanf(fp, "%f %f %f %f\n", &GeneralProfile.carb, &GeneralProfile.prot, &GeneralProfile.fat, &GeneralProfile.goalcal);
	fscanf(fp, "%f %f %f %f\n", &Curr_carb, &Curr_prot, &Curr_fat, &Curr_cal);
	fscanf(fp, "%d\n", &minuskcal);
	fclose(fp);
	return 1;
}
//기록(매크로) 세이브
void SaveLogs(){
	_mkdir("MacroLogs");
	char filepath[100];
	sprintf(filepath, "MacroLogs/Day%d_%d%02d%02d.txt",day,Temp_Y,Temp_M,Temp_D);

	FILE* fp = fopen(filepath, "w");
	fprintf(fp, "%d년 %d월 %d일 | Day %d\n", Temp_Y, Temp_M, Temp_D, day);
	fprintf(fp, "섭취/목표 칼로리 : %.1f - %d / %.1f kcal\n", Curr_cal, minuskcal, GeneralProfile.goalcal);
	fprintf(fp, "섭취/목표 탄수화물량 : %.1f / %.1f g\n", Curr_carb, GeneralProfile.carb);
	fprintf(fp, "섭취/목표 단백질량 : %.1f / %.1f g\n", Curr_prot, GeneralProfile.prot);
	fprintf(fp, "섭취/목표 지방량 : %.1f / %.1f g\n", Curr_fat, GeneralProfile.fat);
	fprintf(fp, "\n\n");
	fclose(fp);
}
//매크로 초기화
void ResetLogs() {
	Curr_cal = 0.0;
	Curr_carb = 0.0;
	Curr_prot = 0.0;
	Curr_fat = 0.0;
	minuskcal = 0;
}
//콘솔 비우기
void ResetScreen() {
	if (noRS == 1) {
		noRS = 0;
		return;
	}
	system("cls");
}
//Enter받기
int GetEnter() {
	printf("\n계속하려면 Enter키를 눌러주세요.\n");
	while (getchar() != '\n');
	getchar();
	return;
}