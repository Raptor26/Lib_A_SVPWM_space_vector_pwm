/**
 * @file   	Lib_A_SVPWM_space_vector_pwm.h
 * @author 	Isaev Mickle
 * @version	beta
 * @date 	07.05.2018
 * @brief	Библиотека реализует векторный ШИМ
 */


#ifndef LIB_A_SVPWM_SPACE_VECTOR_PWM_H_
#define LIB_A_SVPWM_SPACE_VECTOR_PWM_H_


/*#### |Begin| --> Секция - "Include" ########################################*/
/*==== |Begin| --> Секция - "C libraries" ====================================*/
/*==== |End  | <-- Секция - "C libraries" ====================================*/

/*==== |Begin| --> Секция - "MK peripheral libraries" ========================*/
/*==== |End  | <-- Секция - "MK peripheral libraries" ========================*/

/*==== |Begin| --> Секция - "Extern libraries" ===============================*/
/*==== |End  | <-- Секция - "Extern libraries" ===============================*/
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Определение констант" ###########################*/
#if !defined (SVPWM_FLOAT_POINT_TYPE)
#define SVPWM_FLOAT_POINT_TYPE		float
#endif

#define SVPWM_60_DEG_IN_RAD			((SVPWM_FLOAT_POINT_TYPE)(1.04719755))
#define SVPWM_90_DEG_IN_RAD			((SVPWM_FLOAT_POINT_TYPE)(1.57079632))
#define SVPWM_120_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(2.09439510))
#define SVPWM_150_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(2.61799387))
#define SVPWM_180_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(3.14159265))
#define SVPWM_210_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(3.66519143))
#define	SVPWM_240_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(4.18879020))
#define	SVPWM_300_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(5.23598776))
#define	SVPWM_360_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(6.28318531))
#define	SVPWM_PI					SVPWM_180_DEG_IN_RAD
#define SVPWM_MAX_VECT_VALUE		((SVPWM_FLOAT_POINT_TYPE)(1.0))
#define SVPWM_ZERO_VALUE			((SVPWM_FLOAT_POINT_TYPE)(0.0))
/*#### |End  | <-- Секция - "Определение констант" ###########################*/


/*#### |Begin| --> Секция - "Определение типов" ##############################*/
typedef struct
{
	SVPWM_FLOAT_POINT_TYPE phase_A,
						   phase_B,
						   phase_C;
} svpwm_duty_cycle_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
/*#### |End  | <-- Секция - "Определение глобальных переменных" ##############*/


/*#### |Begin| --> Секция - "Прототипы глобальных функций" ###################*/
extern void
SVPWM_GetPhasesEnTime (
	SVPWM_FLOAT_POINT_TYPE (*SinFunc) (SVPWM_FLOAT_POINT_TYPE),
	SVPWM_FLOAT_POINT_TYPE electAngle,
	SVPWM_FLOAT_POINT_TYPE vectAmplit,
	svpwm_duty_cycle_s *pPWMDutyCycles_s);
/*#### |End  | <-- Секция - "Прототипы глобальных функций" ###################*/


/*#### |Begin| --> Секция - "Определение макросов" ###########################*/
/*#### |End  | <-- Секция - "Определение макросов" ###########################*/

#endif	/* LIB_A_SVPWM_SPACE_VECTOR_PWM_H_ */

/*############################################################################*/
/*################################ END OF FILE ###############################*/
/*############################################################################*/
