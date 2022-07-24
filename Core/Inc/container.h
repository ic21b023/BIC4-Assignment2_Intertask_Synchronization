/**
  ******************************************************************************
  * @file    container.h
  * @author  reite
  * @version 1.0
  * @date Created on: 10.03.2022
  * @brief   Header file of GPIO HAL module.
  ******************************************************************************
  */

#ifndef SRC_CONTAINER_H_
#define SRC_CONTAINER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>


/** @addtogroup CONTAINER_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup CONTAINER_Exported_Types CONTAINER Exported Types
  * @{
  */

/**
  * @brief  CONTAINER Configuration Structure definition for split the critical data
  */
enum variablepair {
	T1,
	T2
};

/**
  * @brief  CONTAINER Configuration Structure definition for the critical data
  */
typedef struct  {
 int t1val;
 char t1str[10];
 int t2val;
 char t2str[10];
}critical_data_t;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup CONTAINER_Exported_Functions
  * @{
  */

/** @addtogroup CONTAINER_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/

void Container_Init();

/**
  * @}
  */

/** @addtogroup CONTAINER_Exported_Functions_Group2
  * @{
  */
/* operation functions *****************************************************/
void countUpValuePair(enum variablepair t);
int countUpValuePair_safety(enum variablepair t);
void halveValuePair(enum variablepair t);
int halveValuePair_safety(enum variablepair t);
critical_data_t getcritical_data();
critical_data_t getcritical_data_safety();

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SRC_CONTAINER_H_ */
