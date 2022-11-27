/**
  ******************************************************************************
  * @file    encoder.cpp
  *
  * @author  D. Baines
  *
  * @brief   File contains constants, variables, and function definitions
  *          for interfacing with an RLS Orbis SPI encoder.
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

#include "encoder.hpp"


/*************************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS                                                      */
/*************************************************************************************/

/**
  * @brief  Increment driver error counter
  *
  * @param  driverError: Driver error type to increment
  *
  * @retval None
  */
void Encoder::incrementErrorCount(OrbisDriverError_t driverError)
{
  _errorCounts[driverError]++;
}


/**
  * @brief  Checks the recieved packet for errors and decodes the payload into usable member structures/variables
  *
  * @param  packetIn: Union holding packet data as raw received bytes and data structures
  *
  * @retval status_t: Success status of the packet processing
  */
status_t Encoder::processReceivedPacket(OrbisPositionReceivePacket_t packetIn)
{
  OrbisPositionPayload_t positionPayload = {0};

  uint8_t crcResult = ~(CRC8::calculateCRC8(packetIn.asBytes, ORBIS_POSITION_PACKET_SIZE_IN_BYTES - ORBIS_CRC_SIZE_IN_BYTES));

  if (crcResult == packetIn.asData.OrbisCRC)
  {
    positionPayload.asUINT16 = swapUINT16(packetIn.asData.positionPayload);

    _orbisStatus = static_cast<OrbisStatus_t>(positionPayload.asData.status);

    if (_orbisStatus != ORBIS_STATUS_OK)
    {
      incrementErrorCount(ORBIS_DRIVER_ERROR_STATUS);
      return (STATUS_ERROR);
    }
    else
    {
      _lastValidPosition = positionPayload.asData.position;
      return (STATUS_OK);
    }
  }

  else
  {
    incrementErrorCount(ORBIS_DRIVER_ERROR_CRC_FAIL);
    return (STATUS_ERROR);
  }
}


/*************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                       */
/*************************************************************************************/

/**
  * @brief  Constructor for encoder object - calls constructors for SPI and CRC base classes
  *
  * @param  chipSelectPort: Encoder GPIO chip select port
  *
  * @param  chipSelectPin:  Encoder GPIO chip select pin
  *
  * @param  SPIBusID:       Encoder SPI bus ID
  *
  * @retval None
  */
Encoder::Encoder(GPIO_TypeDef* chipSelectPort, uint16_t chipSelectPin, SPIBusID_t SPIBusID):
SPI(), CRC8(ORBIS_CRC_POLYNOMIAL)
{
  _chipSelectPort  = chipSelectPort;
  _chipSelectPin   = chipSelectPin;
  _SPIBusID        = SPIBusID;
}


/**
  * @brief   Triggers the start of an SPI position fetch transaction.
  *
  * @warning Transfer will be added to a queue at lower levels so will
  *          not start instantly if the queue is not empty.
  *
  * @param   None
  *
  * @retval  None
  */
void Encoder::triggerPositionFetch(void)
{
  SPIJob_t positionFetchSPIJob = { .SPIObject = SPI::getObjectContext(),
                                   .SPIBusID  = _SPIBusID,
                                   .csPort    = _chipSelectPort,
                                   .csPin     = _chipSelectPin,
                                   .txBuffer  = _positionTxBuffer,
                                   .rxBuffer  = _positionRxPacket.asBytes,
                                   .length    = ORBIS_POSITION_PACKET_SIZE_IN_BYTES
                                 };

  if (SPI::transmitReceiveAsync(positionFetchSPIJob) != STATUS_OK)
  {
    incrementErrorCount(ORBIS_DRIVER_ERROR_SPI_BAD_JOB);
  }
}


/**
  * @brief   Returns the last valid position successfully received from the RLSOrbis
  *
  * @warning The position return may not be up-to-date if the position fetch has not
  *          recently been triggered or the SPI transactions have failed
  *
  * @param   None
  *
  * @retval  uint16_t: Last valid encoder position
  */
uint16_t Encoder::getLastValidPosition(void)
{
  return (_lastValidPosition);
}

/*************************************************************************************/
/* CALLBACK HANDLERS                                                                 */
/*************************************************************************************/

/**
  * @brief   Callback from SPI base class called when a transaction is completed successfully
  *
  * @param   None
  *
  * @retval  None
  */
void Encoder::transmitReceiveComplete(void)
{
  status_t receiveStatus = processReceivedPacket(_positionRxPacket);

  positionFetchComplete(receiveStatus);
}


/**
  * @brief   Callback from SPI base class called when a transaction has failed
  *
  * @param   None
  *
  * @retval  None
  */
void Encoder::transferError(void)
{
  incrementErrorCount(ORBIS_DRIVER_ERROR_SPI_TRANSFER);
}


/**
  * @}End of File
  */
