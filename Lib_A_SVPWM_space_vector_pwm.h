/**
 * @file   	Lib_A_SVPWM_space_vector_pwm.h
 * @author 	Isaev Mickle
 * @version	V_1.0.0
 * @date 	22.02.2019
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
/* |Begin| --> Секция определения типа числа с плавающей точкой ============= */
#if !defined (__SVPWM_FPT__)
#error "Please, set __SVPWM_FPT__ float or double in macros list"
#endif

#if !defined (__SVPWM_FPT_SIZE__)
#error "Please, set __SVPWM_FPT_SIZE__ 4 (that mean float) or 8 (that mean double) in macros list"
#endif

#if 	__SVPWM_FPT_SIZE__ == 4

#elif 	__SVPWM_FPT_SIZE__ == 8

#else
#error "Your compiler uses a non-standard floating point size"
#endif
/* |End  | <-- Секция определения типа числа с плавающей точкой ============= */

#if defined (__GNUC__)
  #define __SVPWM_INLINE         inline
  #define __SVPWM_STATIC_INLINE  static inline
#else
 #define __SVPWM_INLINE
 #define __SVPWM_STATIC_INLINE	static
#endif

#define SVPWM_60_DEG_IN_RAD			((__SVPWM_FPT__)(1.0471975511965977461542144610932))
#define SVPWM_90_DEG_IN_RAD			((__SVPWM_FPT__)(1.5707963267948966192313216916398))
#define SVPWM_120_DEG_IN_RAD		((__SVPWM_FPT__)(2.0943951023931954923084289221863))
#define SVPWM_150_DEG_IN_RAD		((__SVPWM_FPT__)(2.6179938779914943653855361527329))
#define SVPWM_180_DEG_IN_RAD		((__SVPWM_FPT__)(3.1415926535897932384626433832795))
#define SVPWM_210_DEG_IN_RAD		((__SVPWM_FPT__)(3.6651914291880921115397506138261))
#define	SVPWM_240_DEG_IN_RAD		((__SVPWM_FPT__)(4.1887902047863909846168578443727))
#define	SVPWM_300_DEG_IN_RAD		((__SVPWM_FPT__)(5.2359877559829887307710723054658))
#define	SVPWM_360_DEG_IN_RAD		((__SVPWM_FPT__)(6.2831853071795864769252867665590))
#define	SVPWM_PI					SVPWM_180_DEG_IN_RAD
#define SVPWM_MAX_VECT_VALUE		((__SVPWM_FPT__)(1.0))
#define SVPWM_ZERO_VALUE			((__SVPWM_FPT__)(0.0))
/*#### |End  | <-- Секция - "Определение констант" ###########################*/


/*#### |Begin| --> Секция - "Определение типов" ##############################*/
typedef enum
{
	SVPWM_ERROR = 0,
	SVPWM_ANGLE_NOT_IN_RANGE,
	SVPWM_OK,
} svpwm_fnc_status_e;

typedef struct
{
	__SVPWM_FPT__ 	phase_A,
					phase_B,
					phase_C;

	svpwm_fnc_status_e fnc_status_e;
} svpwm_duty_cycle_s;
/*#### |End  | <-- Секция - "Определение типов" ##############################*/


/*#### |Begin| --> Секция - "Определение глобальных переменных" ##############*/
/*#### |End  | <-- Секция - "Определение глобальных переменных" ##############*/


/*#### |Begin| --> Секция - "Прототипы глобальных функций" ###################*/
extern void
SVPWM_GetPhasesEnTime(
	__SVPWM_FPT__ (*SinFunc) (__SVPWM_FPT__),
	__SVPWM_FPT__ electAngle,
	__SVPWM_FPT__ vectAmplit,
	svpwm_duty_cycle_s *pPWMDutyCycles_s);
/*#### |End  | <-- Секция - "Прототипы глобальных функций" ###################*/


/*#### |Begin| --> Секция - "Определение макросов" ###########################*/
/*#### |End  | <-- Секция - "Определение макросов" ###########################*/

#endif	/* LIB_A_SVPWM_SPACE_VECTOR_PWM_H_ */

/*############################################################################*/
/*################################ END OF FILE ###############################*/
/*############################################################################*/
