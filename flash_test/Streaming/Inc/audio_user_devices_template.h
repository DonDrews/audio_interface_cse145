/**
  ******************************************************************************
  * @file    audio_user_devices_template.h
  * @author  MCD Application Team 
  * @brief   Abstraction of boared specific devices template .
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
#ifndef __AUDIO_USER_DEVICES_H
#define __AUDIO_USER_DEVICES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* add hardware (mic/speaker) bsp  include files */
   
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#if  USE_USB_AUDIO_RECORDING
typedef struct
{
  /* define specifc mic node fields */
  uint8_t dummy;
}AUDIO_MicrophoneSpecificParams_t;
#endif /* USE_USB_AUDIO_RECORDING */
#if USE_USB_AUDIO_PLAYBACK
typedef struct
{
  /* define specifc speaker node fields */
  uint8_t dummy;
} AUDIO_SpeakerSpecificParms_t;
#endif /* USE_USB_AUDIO_PLAYBACK */

/* Exported macros -----------------------------------------------------------*/
#if USE_USB_AUDIO_PLAYBACK
#define AUDIO_SPEAKER_USER_Init TEMPLATE_AUDIO_SpeakerInit
#endif /* USE_USB_AUDIO_PLAYBACK */
#if  USE_USB_AUDIO_RECORDING
#define AUDIO_USER_MicInit TEMPLATE_AUDIO_MicInit
#endif /* USE_USB_AUDIO_RECORDING */

/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_USER_DEVICES_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
