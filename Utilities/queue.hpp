/**
  ******************************************************************************
  * @file    queue.hpp
  *
  * @author  D. Baines
  *
  * @brief
  *
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
#ifndef __queue_H
#define __queue_H

/*************************************************************************************/
/* INCLUDES                                                                          */
/*************************************************************************************/

#include <stdint.h>
#include "utilities.hpp"

/*************************************************************************************/
/* MODULE CONSTANTS                                                                  */
/*************************************************************************************/


/*************************************************************************************/
/* TEMPLATE IMPLEMENTATIONS                                                          */
/*************************************************************************************/

template<typename elementType_t>
class QUEUE
{
  public:

  /* Public Variables ---------------------------------------------------------------*/

  typedef struct
  {
    elementType_t data;
    status_t      status;
  } return_t;


  /* Public Prototypes --------------------------------------------------------------*/

  QUEUE(void)
  {
    _frontIndex   =  0U;
    _rearIndex    =  0U;
    _elementCount =  0;
  }


  int8_t getSize(void)
  {
    return (_elementCount);
  }


  bool isEmpty(void)
  {
    return (_elementCount == 0);
  }


  QUEUE::return_t front(void)
  {
    QUEUE::return_t queueReturn;

    if(_elementCount <= 0)
    {
      queueReturn.status = STATUS_ERROR;
      return (queueReturn);
    }

    else
    {
      queueReturn.data   = _elementArray[_frontIndex];
      queueReturn.status = STATUS_OK;
      return (queueReturn);
    }
  }


  status_t push(elementType_t element)
  {
    if(_elementCount >= STATIC_QUEUE_SIZE)
    {
      return (STATUS_ERROR);
    }

    else
    {
      _elementArray[_rearIndex] = element;
      _rearIndex++;
      _elementCount++;

      return (STATUS_OK);
    }
  }


  void pop(void)
  {

    if (_elementCount <= 0)
    {
      return;
    }

    for (int8_t index = 0; index < (_elementCount-1); index++)
    {
      _elementArray[index] = _elementArray[index + 1];
    }

    _rearIndex--;
    _elementCount--;

  }


  private:

  /* Private Constants ---------------------------------------------------------------*/

  static const int8_t STATIC_QUEUE_SIZE = 10;

  /* Private Variables ---------------------------------------------------------------*/

  elementType_t _elementArray[STATIC_QUEUE_SIZE];
  uint8_t       _frontIndex;
  uint8_t       _rearIndex;
  int8_t        _elementCount;

  /* Private Prototypes --------------------------------------------------------------*/

};


#endif /* __queue_H */

/**
  * @}End of File
  */


