/**
  ******************************************************************************
  * @file    SPIBusController.cpp
  *
  * @author  D. Baines
  *
  * @brief   File contains constants, variables, and function definitions
  *          for controlling an SPI comms bus on the STM32F4 MCU
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

/*************************************************************************************/
/* INCLUDES                                                                          */
/*************************************************************************************/

#include "STM32-SPIBus.hpp"
#include "spi.h"

/*************************************************************************************/
/* CLASS OBJECTS                                                                     */
/*************************************************************************************/

SPIBus SPI_BUS_ARRAY[NUMBER_OF_SPI_BUS] = {
                                            [SPI_BUS_1] = SPIBus(&hspi1)
                                          };


/*************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                      */
/*************************************************************************************/

/* CLASS: SPIBus --------------------------------------------------------------------*/

void SPIBus::transmitReceiveFirstInQueue(void)
{

  QUEUE<SPI::SPIJob_t>::return_t queueReturn = _jobQueue.front();

  /* Start transmission if queue is not empty */
  if (queueReturn.status == STATUS_OK)
  {
    SPI::SPIJob_t currentJob = queueReturn.data;

    HAL_GPIO_WritePin(currentJob.csPort, currentJob.csPin, GPIO_PIN_RESET);

    if (HAL_SPI_TransmitReceive_DMA(_spiHandle, currentJob.txBuffer, currentJob.rxBuffer, currentJob.length) != HAL_OK)
    {
      jobComplete(STATUS_ERROR);
    }
  }

}

/*************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                       */
/*************************************************************************************/

/* CLASS: SPI -----------------------------------------------------------------------*/

SPI::SPI(void)
{

}


SPI* SPI::getObjectContext(void)
{
  return (this);
}


status_t SPI::transmitReceiveAsync(SPIJob_t SPIJob)
{
  return (SPI_BUS_ARRAY[SPIJob.SPIBusID].addJobToQueue(SPIJob));
}


/* CLASS: SPIBus --------------------------------------------------------------------*/

SPIBus::SPIBus(SPI_HandleTypeDef* spiHandle)
{
  _spiHandle = spiHandle;
}


status_t SPIBus::addJobToQueue(SPI::SPIJob_t SPIJob)
{
  if ((SPIJob.SPIObject == NULL) ||
      (SPIJob.rxBuffer  == NULL) ||
      (SPIJob.txBuffer  == NULL)   )
  {
    return (STATUS_ERROR);
  }

  /* Safely disable interrupts - if the SPI TXRX complete callback fired in this section, unexpected behaviour could occur */
  uint32_t primask = ENTER_CRITICAL_SECTION();

  int8_t queueCountPrePush = _jobQueue.getSize();

  /* Add to SPI bus job queue if queue is not full */
  if (_jobQueue.push(SPIJob) == STATUS_OK)
  {
    if (queueCountPrePush == 0)
    {
      transmitReceiveFirstInQueue();
    }
  }

  EXIT_CRITICAL_SECTION(primask);

  return (STATUS_OK);
}


void SPIBus::jobComplete(status_t transferStatus)
{
  QUEUE<SPI::SPIJob_t>::return_t frontReturn = _jobQueue.front();

  /* End transmission if queue is not empty */
  if (frontReturn.status == STATUS_OK)
  {
    SPI::SPIJob_t currentJob = frontReturn.data;

    HAL_GPIO_WritePin(currentJob.csPort, currentJob.csPin, GPIO_PIN_SET);

    if (transferStatus == STATUS_OK) currentJob.SPIObject->transmitReceiveComplete();
    else                             currentJob.SPIObject->transferError();

    _jobQueue.pop();

    if (_jobQueue.getSize() > 0)
    {
      transmitReceiveFirstInQueue();
    }
  }
}


/*************************************************************************************/
/* INTERRUPT HANDLERS                                                                */
/*************************************************************************************/


/**
  * @brief Tx and Rx Transfer completed callback.
  *
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)                                      // @suppress("C-Style cast instead of C++ cast")å
  {
    SPI_BUS_ARRAY[SPI_BUS_1].jobComplete(STATUS_OK);
  }
}


/**
  * @}End of File
  */


