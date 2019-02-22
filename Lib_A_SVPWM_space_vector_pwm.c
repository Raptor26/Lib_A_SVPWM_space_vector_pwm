/**
 * @file   	Lib_A_SVPWM_space_vector_pwm.c
 * @author 	Isaev Mickle
 * @version	V_1.0.0
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
__SVPWM_STATIC_INLINE void
SVPWM_CalcBaseVect(
	__SVPWM_FPT__ (*SinFunc) (__SVPWM_FPT__),
	__SVPWM_FPT__ *baseVect_1,
	__SVPWM_FPT__ *baseVect_2,
	__SVPWM_FPT__ electAngle,
	__SVPWM_FPT__ vectAmplit);

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector1(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector2(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector3(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector4(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);

__SVPWM_STATIC_INLINE void
SVPWM_CalcRelativEnTime_Sector5(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector6(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/

/*-------------------------------------------------------------------------*//**
 * @author    Mickle Isaev
 * @date      21-фев-2019
 *
 * @brief   Функция выполняет расчет коэффициентов заполнения ШИМ для каждой
 * 			из 3-х фаз электродвигателя
 * 			
 * @details В функцию необходимо записать желаемый электрический угол в
 *  		который необходимо выставить вектор тока и желаемую амплитуду
 *  		этого вектора. Функция рассчитает амплитуду двух базовых векторов,
 *  		затем рассчитает коэффициенты заполнения ШИМ для каждой из 3-х
 *  		фаз электродвигателя
 *
 * @param[in]	(*SinFunc):	Указатель на функцию нахождения синуса угла в радианах
 * @param[in] 	desiredElectAngle:	Электрический угол, в который необходимо установить
 * 									вектор тока (в диапазоне от "0" до "2*PI")
 * @param[in] 	desiredVectAmplit: 	Амплитуда вектора тока (в диапазоне от "0" до "1")
 * @param[out] 	*pPWMDutyCycles_s:	Указатель на структуру, в которую будут
 * 									записан коэффициент заполнения ШИМ для
 * 									3-х фаз электродвигателя
 *
 * @return	None
 */
void
SVPWM_GetPhasesEnTime(
	__SVPWM_FPT__ (*SinFunc) (__SVPWM_FPT__),
	__SVPWM_FPT__ desiredElectAngle,
	__SVPWM_FPT__ desiredVectAmplit,
	svpwm_duty_cycle_s *pPWMDutyCycles_s)
{
	__SVPWM_FPT__ baseVect_1, baseVect_2;
	pPWMDutyCycles_s->fnc_status_e = SVPWM_OK;

	/* Если заданный электрический угол находится в одном из 1-3 сектор
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

			return;
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

			return;
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

			return;
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

			return;
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

			return;
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

			return;
		}
	}

	/* Если заданный угол находится вне допустимого диапазона */
	pPWMDutyCycles_s->fnc_status_e = SVPWM_ANGLE_NOT_IN_RANGE;
	pPWMDutyCycles_s->phase_A = (__SVPWM_FPT__) 0.0;
	pPWMDutyCycles_s->phase_B = (__SVPWM_FPT__) 0.0;
	pPWMDutyCycles_s->phase_C = (__SVPWM_FPT__) 0.0;
}

__SVPWM_STATIC_INLINE  void
SVPWM_CalcBaseVect(
	__SVPWM_FPT__ (*SinFunc) (__SVPWM_FPT__),
	__SVPWM_FPT__ *baseVect_1,
	__SVPWM_FPT__ *baseVect_2,
	__SVPWM_FPT__ electAngle,
	__SVPWM_FPT__ vectAmplit)
{
	*baseVect_1 =
		vectAmplit * SinFunc(SVPWM_60_DEG_IN_RAD - electAngle);
	*baseVect_2 =
		vectAmplit * SinFunc(electAngle);
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector1(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
	*pPhase_B_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_2;
	*pPhase_C_DutyCycle =
		baseVect_0 * (__SVPWM_FPT__) 0.5;
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector2(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1;
	*pPhase_B_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
	*pPhase_C_DutyCycle =
		baseVect_0 * (__SVPWM_FPT__) 0.5;
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector3(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		baseVect_0 * (__SVPWM_FPT__) 0.5;
	*pPhase_B_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
	*pPhase_C_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_2;
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector4(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		baseVect_0 * (__SVPWM_FPT__) 0.5;
	*pPhase_B_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1;
	*pPhase_C_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcRelativEnTime_Sector5(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_2;
	*pPhase_B_DutyCycle =
		baseVect_0 * (__SVPWM_FPT__) 0.5;
	*pPhase_C_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
}

__SVPWM_STATIC_INLINE void
SVPWM_CalcPWMDutyCycle_Sector6(
	__SVPWM_FPT__ baseVect_1,
	__SVPWM_FPT__ baseVect_2,
	__SVPWM_FPT__ *pPhase_A_DutyCycle,
	__SVPWM_FPT__ *pPhase_B_DutyCycle,
	__SVPWM_FPT__ *pPhase_C_DutyCycle)
{
	__SVPWM_FPT__ baseVect_0 =
		SVPWM_MAX_VECT_VALUE - baseVect_1 - baseVect_2;

	*pPhase_A_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1 + baseVect_2;
	*pPhase_B_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5);
	*pPhase_C_DutyCycle =
		(baseVect_0 * (__SVPWM_FPT__) 0.5) + baseVect_1;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
