/**
  ******************************************************************************
  * @file    encoder.hpp
  *
  * @author  D. Baines
  *
  * @brief   File contains defines, type declarations, and function prototypes
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

/* Define to prevent recursive inclusion --------------------------------------------*/
#ifndef __RLSOrbis_H
#define __RLSOrbis_H

/*************************************************************************************/
/* INCLUDES                                                                          */
/*************************************************************************************/

#include "gpio.h"
#include "../PeripheralLayer/STM32-SPIBus.hpp"
#include "../Utilities/CRC8.hpp"
#include "../Utilities/utilities.hpp"


/*************************************************************************************/
/* CLASS DEFINITIONS                                                                 */
/*************************************************************************************/

class Encoder:
private SPI,
private CRC8
{

  public:

  /*-- Public Variables -------------------------------------------------------------*/


  /*-- Public Prototypes ------------------------------------------------------------*/

  Encoder(GPIO_TypeDef*  chipSelectPort,
          uint16_t       chipSelectPin,
          SPIBusID_t     SPIBusID);

  virtual ~Encoder() {};

  void triggerPositionFetch(void);

  uint16_t getLastValidPosition(void);


  private:

  /*-- Private Constants ------------------------------------------------------------*/

  static const uint8_t ORBIS_CRC_POLYNOMIAL                = 0x97U;

  static const uint8_t ORBIS_POSITION_PACKET_SIZE_IN_BYTES = 3U;
  static const uint8_t ORBIS_CRC_SIZE_IN_BYTES             = 1U;

  static const uint8_t ORBIS_POSITION_DATA_RESOLUTION      = 14U;
  static const uint8_t ORBIS_STATUS_BIT_SIZE               = 2U;

  /*-- Private Typedefs -------------------------------------------------------------*/

  typedef enum: uint8_t
  {
    ORBIS_STATUS_OK                = 0b11,
    ORBIS_STATUS_ERROR             = 0b01,
    ORBIS_STATUS_WARNING           = 0b10,
    ORBIS_STATUS_ERROR_AND_WARNING = 0b00,

  } OrbisStatus_t;

  typedef union
  {
    uint16_t asUINT16;

    struct
    {
      unsigned status   : ORBIS_STATUS_BIT_SIZE;
      unsigned position : ORBIS_POSITION_DATA_RESOLUTION;

    } asData;

  } OrbisPositionPayload_t;


  typedef union
  {
    uint8_t asBytes[ORBIS_POSITION_PACKET_SIZE_IN_BYTES];

    struct
    {
      uint16_t positionPayload;
      uint8_t  OrbisCRC;

    } asData;

  } OrbisPositionReceivePacket_t;


  typedef enum: uint8_t
  {
    ORBIS_DRIVER_ERROR_CRC_FAIL     = 0,
    ORBIS_DRIVER_ERROR_SPI_BAD_JOB  = 1,
    ORBIS_DRIVER_ERROR_SPI_TRANSFER = 2,
    ORBIS_DRIVER_ERROR_STATUS       = 3,
    NUMBER_OF_ORBIS_DRIVER_ERRORS
  } OrbisDriverError_t;


  /*-- Private Variables ------------------------------------------------------------*/

  GPIO_TypeDef                *_chipSelectPort;
  uint16_t                     _chipSelectPin;
  SPIBusID_t                   _SPIBusID;

  volatile uint16_t            _lastValidPosition;
  volatile OrbisStatus_t       _orbisStatus;

  uint8_t                      _positionTxBuffer[ORBIS_POSITION_PACKET_SIZE_IN_BYTES] = {0U};
  OrbisPositionReceivePacket_t _positionRxPacket;

  uint32_t                     _errorCounts[NUMBER_OF_ORBIS_DRIVER_ERRORS] = {0U};

  /*-- Private Prototypes -----------------------------------------------------------*/

  void incrementErrorCount(OrbisDriverError_t driverError);

  status_t processReceivedPacket(OrbisPositionReceivePacket_t packetIn);

  /* Callback to derived class to signal complete position data collection */
  virtual void positionFetchComplete(status_t positionFetchStatus) = 0;

  /* Callback from SPI base class to signal complete data transaction */
  virtual void transmitReceiveComplete(void) final;

  /* Callback from SPI base class to signal data transaction error */
  virtual void transferError(void) final;

};


#endif /* __RLSOrbis_H */

/**
  * @}End of File
  */


