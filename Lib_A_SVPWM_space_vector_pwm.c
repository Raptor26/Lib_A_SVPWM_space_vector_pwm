/**
 * @file   	Lib_A_SVPWM_space_vector_pwm.c
 * @author 	Isaev Mickle
 * @version	beta
 * @date 	07.05.2018
 * @brief	Библиотека реализует векторный ШИМ
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "Lib_A_SVPWM_space_vector_pwm.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
SVPWM_CalcBaseVect(
	SVPWM_FLOAT_POINT_TYPE (*SinFunc) (SVPWM_FLOAT_POINT_TYPE),
	SVPWM_FLOAT_POINT_TYPE *baseVect_1,
	SVPWM_FLOAT_POINT_TYPE *baseVect_2,
	SVPWM_FLOAT_POINT_TYPE electAngle,
	SVPWM_FLOAT_POINT_TYPE vectAmplit);

static void
SVPWM_CalcPWMDutyCycle_Sector1(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);

static void
SVPWM_CalcPWMDutyCycle_Sector2(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);

static void
SVPWM_CalcPWMDutyCycle_Sector3(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);

static void
SVPWM_CalcPWMDutyCycle_Sector4(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);

static void
SVPWM_CalcRelativEnTime_Sector5(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);

static void
SVPWM_CalcPWMDutyCycle_Sector6(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/

/**
 * @brief 	Функция выполняет расчет коэффициентов заполнения ШИМ для каждой
 * 			из 3-х фаз электродвигателя
 * @details В функцию необходимо записать желаемый электрический угол в
 *  		который необходимо выставить вектор тока и желаемую амплитуду
 *  		этого вектора. Функция рассчитает амплитуду двух базовых векторов,
 *  		затем рассчитает коэффициенты заполнения ШИМ для каждой из 3-х
 *  		фаз электродвигателя
 *
 * @param[in]	SinFunc:	Указатель на функцию нахождения синуса угла в радианах
 * @param[in] 	electAngle:	Электрический угол, в который необходимо установить
 * 							вектор тока в диапазоне от "0" до "2*PI"
 * @param[in] 	vectAmplit: Амплитуда вектора тока в промежутке от "0" до "1"
 * @param[out] 	pPWMDutyCycles_s:	Указатель на структуру, в которую будут
 * 									записаны коэффициенты заполнения ШИМ для
 * 									3-х фаз электродвигателя
 * @return	None
 */
void SVPWM_GetPhasesEnTime (
	SVPWM_FLOAT_POINT_TYPE (*SinFunc) (SVPWM_FLOAT_POINT_TYPE),
	SVPWM_FLOAT_POINT_TYPE desiredElectAngle,
	SVPWM_FLOAT_POINT_TYPE desiredVectAmplit,
	svpwm_duty_cycle_s *pPWMDutyCycles_s)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_1, baseVect_2;

	/* Если желаемый электрический угол находится в одном из 1-3 сектор
	 * включительно */
	if (desiredElectAngle <= SVPWM_180_DEG_IN_RAD)
	{
		/* 3-й сектор */
		if ((desiredElectAngle < SVPWM_180_DEG_IN_RAD )
			&& (desiredElectAngle >= SVPWM_120_DEG_IN_RAD))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle - SVPWM_120_DEG_IN_RAD,
				desiredVectAmplit);

			SVPWM_CalcPWMDutyCycle_Sector3(
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
		/* 2-й сектор */
		else if ((desiredElectAngle < SVPWM_120_DEG_IN_RAD)
				 && (desiredElectAngle >= SVPWM_60_DEG_IN_RAD))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle - SVPWM_60_DEG_IN_RAD,
				desiredVectAmplit);

			SVPWM_CalcPWMDutyCycle_Sector2 (
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
		/* 1-й сектор */
		else if ((desiredElectAngle < SVPWM_60_DEG_IN_RAD)
				 && (desiredElectAngle >= SVPWM_ZERO_VALUE))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle,
				desiredVectAmplit);

			SVPWM_CalcPWMDutyCycle_Sector1 (
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
	}
	/* Иначе, находится в одном из 4-6 сектор включительно */
	else
	{
		/* 4-й сектор */
		if ((desiredElectAngle < SVPWM_240_DEG_IN_RAD)
			&& (desiredElectAngle >= SVPWM_180_DEG_IN_RAD))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle - SVPWM_180_DEG_IN_RAD,
				desiredVectAmplit);

			SVPWM_CalcPWMDutyCycle_Sector4 (
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
		/* 5-й сектор */
		else if ((desiredElectAngle < SVPWM_300_DEG_IN_RAD)
				 && (desiredElectAngle >= SVPWM_240_DEG_IN_RAD))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle - SVPWM_240_DEG_IN_RAD,
				desiredVectAmplit);

			SVPWM_CalcRelativEnTime_Sector5 (
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
		/* 6-й сектор */
		else if ((desiredElectAngle < SVPWM_360_DEG_IN_RAD)
				 && (desiredElectAngle >= SVPWM_300_DEG_IN_RAD))
		{
			SVPWM_CalcBaseVect (
				SinFunc,
				&baseVect_1,
				&baseVect_2,
				desiredElectAngle - SVPWM_300_DEG_IN_RAD,
				desiredVectAmplit);

			SVPWM_CalcPWMDutyCycle_Sector6 (
				baseVect_1,
				baseVect_2,
				&pPWMDutyCycles_s->phase_A,
				&pPWMDutyCycles_s->phase_B,
				&pPWMDutyCycles_s->phase_C);
		}
	}
}

void SVPWM_CalcBaseVect (
	SVPWM_FLOAT_POINT_TYPE (*SinFunc) (SVPWM_FLOAT_POINT_TYPE),
	SVPWM_FLOAT_POINT_TYPE *baseVect_1,
	SVPWM_FLOAT_POINT_TYPE *baseVect_2,
	SVPWM_FLOAT_POINT_TYPE electAngle,
	SVPWM_FLOAT_POINT_TYPE vectAmplit)
{
	*baseVect_1 =
		vectAmplit * SinFunc(SVPWM_60_DEG_IN_RAD - electAngle);
	*baseVect_2 =
		vectAmplit * SinFunc(electAngle);
}

void SVPWM_CalcPWMDutyCycle_Sector1(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
	*pPhase_B_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_2;
	*pPhase_C_DutyCycle =
		baseVect_0 * 0.5f;
}

void SVPWM_CalcPWMDutyCycle_Sector2(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1;
	*pPhase_B_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
	*pPhase_C_DutyCycle =
		baseVect_0 * 0.5f;
}

void SVPWM_CalcPWMDutyCycle_Sector3(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		baseVect_0 * 0.5f;
	*pPhase_B_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
	*pPhase_C_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_2;

}

void SVPWM_CalcPWMDutyCycle_Sector4(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		baseVect_0 * 0.5f;
	*pPhase_B_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1;
	*pPhase_C_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
}

void SVPWM_CalcRelativEnTime_Sector5(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_2;
	*pPhase_B_DutyCycle =
		baseVect_0 * 0.5f;
	*pPhase_C_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
}

void SVPWM_CalcPWMDutyCycle_Sector6(
	SVPWM_FLOAT_POINT_TYPE baseVect_1,
	SVPWM_FLOAT_POINT_TYPE baseVect_2,
	SVPWM_FLOAT_POINT_TYPE *pPhase_A_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_B_DutyCycle,
	SVPWM_FLOAT_POINT_TYPE *pPhase_C_DutyCycle)
{
	SVPWM_FLOAT_POINT_TYPE baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1 + baseVect_2;
	*pPhase_B_DutyCycle =
		(baseVect_0 * 0.5f);
	*pPhase_C_DutyCycle =
		(baseVect_0 * 0.5f) + baseVect_1;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
