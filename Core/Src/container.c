/**
  ******************************************************************************
  * @file    container.c
  * @author  reite
  * @version 1.0
  * @date Created on: 10.03.2022
  * @brief   CONTAINER module driver.
  *          This file provides functions to manage the following
  *          functionalities
  *           + Initialization and de-initialization functions
  *           + operation functions
  *
  @verbatim
  ==============================================================================
                    ##### CONTAINER features #####
  ==============================================================================


    (+) provides a struct-memory which is protected with semaphores

    (+) Optionally, the struct-memory can be operated without semaphores for test purposes
    	 ->  # define NOT_CRITICALLY_SAFE in main.c

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "container.h"
#include "cmsis_os.h"


/** @addtogroup CONTAINER_Driver
  * @{
  */

/* Private variables ---------------------------------------------------------*/

static bool initialized = false;

static osSemaphoreId_t semaOne_id = NULL;
static osSemaphoreId_t semaTwo_id = NULL;

static critical_data_t critical_data ={0};


/* Exported functions --------------------------------------------------------*/
/** @defgroup CONTAINER_Exported_Functions DMA Exported Functions
  * @{
  */

/** @defgroup CONTAINER_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
 ===============================================================================
             ##### Initialization and de-initialization functions  #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the CONTAINER-Struct
  * @param  none
  */
void Container_Init(){

	semaOne_id = osSemaphoreNew(1U, 1U, NULL);
	semaTwo_id = osSemaphoreNew(1U, 1U, NULL);

	initialized=true;
}

/**
  * @}
  */

/** @defgroup CONTAINER_Exported_Functions_Group2 Input and Output operation functions
 *  @brief   Input and Output operation functions
 *
@verbatim
 ===============================================================================
                      #####  operation functions  #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  counts up the value and string in the struct-memory without semaphores
  * @param  select die T1 for the first value pair and select T2 for the secound value pair
  */
void countUpValuePair(enum variablepair t){

	/* check if the module is initialized */
	if (initialized){

		if(t==T1){
			/* count up the values of T1 of the struct-memory */
			critical_data.t1val++;
			osDelay(100);
			itoa(critical_data.t1val,critical_data.t1str,10);
		}else{
			/* count up the values of T2 of the struct-memory */
			critical_data.t2val++;
			osDelay(100);
			itoa(critical_data.t2val,critical_data.t2str,10);
		}
	}
}

/**
  * @brief  counts up the value and string in the struct-memory with semaphores
  * @param  select die T1 for the first value pair and select T2 for the secound value pair
  * @retval osSemaphoreAcquire-status | 0=OK, 1=resource T2 not available, 1=resource T2 not available
  */
int countUpValuePair_safety(enum variablepair t){

	/* check if the module is initialized */
	if (initialized){

		/* selection between T1 and T2 */
		switch(t) {

				case T1:

					/* check whether access to the critical section is possible and take a token */
					if(osOK == osSemaphoreAcquire(semaOne_id, 0U)){
						/* count up the values of T1 of the struct-memory */
						countUpValuePair(t);
						/* return the token of the semaphore */
						osSemaphoreRelease(semaOne_id);
						return 0;
					}else{
						return 1;
					}

				break;

				case T2:

					/* check whether access to the critical section is possible and take a token */
					if (osOK == osSemaphoreAcquire(semaTwo_id, 0U)){
						/* count up the values of T2 of the struct-memory */
						countUpValuePair(t);
						/* return the token of the semaphore */
						osSemaphoreRelease(semaTwo_id);
						return 0;
					}else{
						return -1;
					}

			break;
		}
	}

}

/**
  * @brief  halve the value and string in the struct-memory without semaphores
  * @param  select die T1 for the first value pair and select T2 for the secound value pair
  */
void halveValuePair(enum variablepair t){

	/* check if the module is initialized */
	if (initialized){

		/* selection between T1 and T2 */
		if(t==T1){
			/* halve the values of T1 of the struct-memory */
			critical_data.t1val = (critical_data.t1val & 1) ? critical_data.t1val : critical_data.t1val/2;
			itoa(critical_data.t1val,critical_data.t1str,10);
		}else{
			/* halve the values of T2 of the struct-memory */
			critical_data.t2val = (critical_data.t2val & 1) ? critical_data.t2val : critical_data.t2val/2;
			itoa(critical_data.t2val,critical_data.t2str,10);
		}
	}
}

/**
  * @brief  halve the value and string in the struct-memory with semaphores
  * @param  select die T1 for the first value pair and select T2 for the secound value pair
  * @retval osSemaphoreAcquire-status | 0=OK, 1=resource T2 not available, 1=resource T2 not available
  */
int halveValuePair_safety(enum variablepair t){

	/* check if the module is initialized */
	if (initialized){

		/* selection between T1 and T2 */
		switch(t) {

			case T1:

				/* check whether access to the critical section is possible and take a token */
				if(osOK == osSemaphoreAcquire(semaOne_id, 0U)){
					/* halve the values of the struct-memory */
					halveValuePair(t);
					/* return the token of the semaphore */
					osSemaphoreRelease(semaOne_id);

					return 0;
				}else{
					return 1;
				}

			break;

			case T2:

				/* check whether access to the critical section is possible */
				if (osOK == osSemaphoreAcquire(semaTwo_id, 0U)){
					/* halve the values of the struct-memory */
					halveValuePair(t);
					/* return the token of the semaphore */
					osSemaphoreRelease(semaTwo_id);
					return 0;
				}else{
					return -1;
				}

			break;
		}

	return 1;
	}
}

/**
  * @brief  returns the struct-memory without semaphores
  * @retval struct-memory
  */
critical_data_t getcritical_data(){

	/* check if the module is initialized */
	if (initialized){
		critical_data_t datastruct= critical_data;
		return datastruct;
	}
}

/**
  * @brief  returns the struct-memory with semaphores
  * @retval struct-memory
  */
critical_data_t getcritical_data_safety(){

	/* check if the module is initialized */
	if (initialized){


		critical_data_t data={NULL};

		/* check whether access to the critical section from T1 and T2 is possible and take a token */
		if(osOK == osSemaphoreAcquire(semaOne_id, 0U) ){
			while(osOK != osSemaphoreAcquire(semaTwo_id, 0U)){}
			/* get the struct-memory */
			data=getcritical_data();
			/* return the token of the semaphores */
			osSemaphoreRelease(semaOne_id);
			osSemaphoreRelease(semaTwo_id);
			/* check whether access to the critical section from T2 and T1 is possible and take a token */
		}else if(osOK == osSemaphoreAcquire(semaTwo_id, 0U) ) {
			while(osOK != osSemaphoreAcquire(semaOne_id, 0U)){}
			/* get the struct-memory */
			data=getcritical_data();
			/* return the token of the semaphores */
			osSemaphoreRelease(semaOne_id);
			osSemaphoreRelease(semaTwo_id);
		}

		return data;
	}
}

/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */
