
/*************************************************************************************/
/**
 *
 *  @file       utilities.hpp
 *
 *  @brief      File contains defines, type declarations, and function prototypes
 *              for global system use
 *
 *  @version    v0.1
 *
 *  @author     D. Baines
 *
 *  @date       15/10/2022
 *
 *  @copyright  D. Baines
 *
 */
/*************************************************************************************/

/* Define to prevent recursive inclusion --------------------------------------------*/
#ifndef __utilities_H
#define __utilities_H


/*************************************************************************************/
/* INCLUDES                                                                          */
/*************************************************************************************/

#include <stdint.h>
#include "stm32f4xx_hal.h"

/*************************************************************************************/
/* TYPEDEFS                                                                          */
/*************************************************************************************/

typedef enum: uint8_t
{
  STATUS_OK                = 0U,
  STATUS_ERROR             = 1U,
} status_t;


/*************************************************************************************/
/* PUBLIC CONSTANTS                                                                  */
/*************************************************************************************/

const uint8_t BITS_IN_A_BYTE = 8U;

/*************************************************************************************/
/* PUBLIC FUNCTION DECLARANTIONS                                                     */
/*************************************************************************************/

uint16_t swapUINT16(uint16_t data);


/*************************************************************************************/
/* CRITICAL FUNCTION DEFINITIONS                                                     */
/*************************************************************************************/

static inline uint32_t ENTER_CRITICAL_SECTION()
{
    /* Read PRIMASK register, check interrupt status before they are disabled */
    /* Returns 0 if they are enabled, or non-zero if disabled */
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return (primask);
}

static inline void EXIT_CRITICAL_SECTION(uint32_t priority_mask)
{
    /* Re-enable interrupts only if they were enabled before entering critical section */
    __set_PRIMASK(priority_mask);
}


#endif /* __utilities_H */

/**
  * @}End of File
  */


