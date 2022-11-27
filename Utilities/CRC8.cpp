/**
  ******************************************************************************
  * @file    CRC8.cpp
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

#include "CRC8.hpp"
#include "../Utilities/utilities.hpp"


/*************************************************************************************/
/* PRIVATE CONSTANTS                                                                 */
/*************************************************************************************/

const uint8_t BYTE_MSB_HIGH = 0x80U;

/*************************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS                                                       */
/*************************************************************************************/

CRC8::CRC8(uint8_t generatorPolynomial)
{
  uint8_t generator = generatorPolynomial;

  /* iterate over all byte values 0 - 255 */
  for (uint16_t divident = 0U; divident < DECIMAL_WIDTH_8_BIT; divident++)
  {
    uint8_t currentByte = static_cast<uint8_t>(divident);

    /* calculate the CRC-8 value for current byte */
    for (uint8_t bit = 0U; bit < BITS_IN_A_BYTE; bit++)
    {
      if ((currentByte & BYTE_MSB_HIGH) != 0U)
      {
        currentByte <<= 1U;
        currentByte ^= generator;
      }
      else
      {
        currentByte <<= 1U;
      }
    }

    /* store CRC value in lookup table */
    _CRCTable[divident] = currentByte;
  }
}


uint8_t CRC8::calculateCRC8(uint8_t* byteBuffer, uint8_t length)
{
  uint8_t crc = 0U;

  for (uint8_t index = 0U; index < length; index++)
  {
    crc = _CRCTable[crc ^ byteBuffer[index]];
  }

  return (crc);
}


/**
  * @}End of File
  */
