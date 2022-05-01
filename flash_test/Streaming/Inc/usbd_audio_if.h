/**
  ******************************************************************************
  * @file    usbd_audio_if.h
  * @author  MCD Application Team 
  * @brief   Header for usbd_audio_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019  STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_AUDIO_IF_H
#define __USBD_AUDIO_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio.h"
#include "audio_sessions_usb.h"

/* Exported constants --------------------------------------------------------*/
 extern USBD_AUDIO_InterfaceCallbacksfTypeDef audio_class_interface;
/* Exported types ------------------------------------------------------------*/
#if USE_AUDIO_USB_INTERRUPT
typedef enum 
{
  USBD_AUDIO_PLAYBACK  = 0x01,
  USBD_AUDIO_RECORD    = 0x02
}USBD_AUDIO_FunctionTypedef;
#endif /* USE_AUDIO_USB_INTERRUPT*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#if USE_AUDIO_USB_INTERRUPT
int8_t USBD_AUDIO_ExecuteControl( uint8_t func, AUDIO_ControlCommand_t control , uint32_t val , uint32_t private_data);
#endif /* USE_AUDIO_USB_INTERRUPT*/
#endif /* __USBD_AUDIO_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
