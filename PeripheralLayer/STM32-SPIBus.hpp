/**
  ******************************************************************************
  * @file    SPIBusController.hpp
  *
  * @author  D. Baines
  *
  * @brief   File contains defines, type definitions, and function prototypes
  *          for controlling an SPI comms bus on the STM32F4
  *
  * @version v1.0
  ******************************************************************************
  * @attention
  *
  * Copyright (c) D. Baines
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion --------------------------------------------*/
#ifndef __SPIBusController_H
#define __SPIBusController_H

/**************************************************************************************
 * INCLUDES
 *************************************************************************************/

#include "gpio.h"
#include "../Utilities/queue.hpp"


/**************************************************************************************
 * TYPEDEFS
 *************************************************************************************/

typedef enum: uint8_t
{
  SPI_BUS_1 = 0,
  NUMBER_OF_SPI_BUS
} SPIBusID_t;


/**************************************************************************************
 * PROTOTYPES/CLASS DEFINITIONS
 *************************************************************************************/


class SPI
{

  public:

  /* Public Constants ---------------------------------------------------------------*/

  typedef struct
  {
	  SPI*          SPIObject;
	  SPIBusID_t    SPIBusID;
	  GPIO_TypeDef* csPort;
	  uint16_t      csPin;
	  uint8_t*      txBuffer;
	  uint8_t*      rxBuffer;
	  uint8_t       length;

  } SPIJob_t;


  /* Public Prototypes --------------------------------------------------------------*/

  SPI(void);

  virtual ~SPI() {};

  SPI* getObjectContext(void);

  status_t transmitReceiveAsync(SPIJob_t SPIJob);


  private:

  /* Private Prototypes -------------------------------------------------------------*/

  virtual void transmitReceiveComplete(void) = 0;

  virtual void transferError(void) = 0;


  /* Friend Class Declarations ------------------------------------------------------*/

  friend class SPIBus;

};


class SPIBus
{

  public:

  /* Public Functions --------------------------------------------------------------*/

  SPIBus(SPI_HandleTypeDef* spiHandle);

  void jobComplete(status_t transferStatus);


  private:

  /* Private Variables --------------------------------------------------------------*/

  SPI_HandleTypeDef*   _spiHandle = NULL;
  QUEUE<SPI::SPIJob_t> _jobQueue;


  /* Private Functions --------------------------------------------------------------*/

  status_t addJobToQueue(SPI::SPIJob_t SPIJob);

  void transmitReceiveFirstInQueue(void);

  void abortJob(void);


  /* Friend Class Declarations ------------------------------------------------------*/

  friend class SPI;

};



#endif /* __SPIBusController_H */

/**
  * @}End of File
  */


