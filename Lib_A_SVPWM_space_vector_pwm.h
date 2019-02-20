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

#define SVPWM_60_DEG_IN_RAD			((SVPWM_FLOAT_POINT_TYPE)(1.0471975511965977461542144610932))
#define SVPWM_90_DEG_IN_RAD			((SVPWM_FLOAT_POINT_TYPE)(1.5707963267948966192313216916398))
#define SVPWM_120_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(2.0943951023931954923084289221863))
#define SVPWM_150_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(2.6179938779914943653855361527329))
#define SVPWM_180_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(3.1415926535897932384626433832795))
#define SVPWM_210_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(3.6651914291880921115397506138261))
#define	SVPWM_240_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(4.1887902047863909846168578443727))
#define	SVPWM_300_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(5.2359877559829887307710723054658))
#define	SVPWM_360_DEG_IN_RAD		((SVPWM_FLOAT_POINT_TYPE)(6.2831853071795864769252867665590))
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
