/**
  ******************************************************************************
  * @file    CRC8.hpp
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
#ifndef __CRC8_H
#define __CRC8_H

/*************************************************************************************/
/* INCLUDES                                                                          */
/*************************************************************************************/

#include <stdint.h>

const uint16_t DECIMAL_WIDTH_8_BIT = 256U;

/*************************************************************************************/
/* PROTOTYPES/CLASS DEFINITIONS                                                      */
/*************************************************************************************/

class CRC8
{

  public:

  /*-- Public Prototypes ------------------------------------------------------------*/

  CRC8(uint8_t generatorPolynomial);

  uint8_t calculateCRC8(uint8_t* byteBuffer, uint8_t length);

  private:

  /*-- Private Variables ------------------------------------------------------------*/

  uint8_t _CRCTable[DECIMAL_WIDTH_8_BIT] = {0U};



};


#endif /* __CRC8_H */

/**
  * @}End of File
  */


