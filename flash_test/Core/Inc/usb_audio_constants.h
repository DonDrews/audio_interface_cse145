/**
  ******************************************************************************
  * @file    usb_audio_constants.h
  * @author  MCD Application Team 
  * @brief   USB audio application configuration.
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
#ifndef __USB_AUDIO_CONSTANTS_H
#define __USB_AUDIO_CONSTANTS_H

#ifdef __cplusplus
 extern "C" {
#endif
/* includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* list of frequencies*/
#define USB_AUDIO_CONFIG_FREQ_192_K   192000 /* to use only with class audio 2.0 */
#define USB_AUDIO_CONFIG_FREQ_96_K   96000
#define USB_AUDIO_CONFIG_FREQ_48_K   48000 
#define USB_AUDIO_CONFIG_FREQ_44_1_K 44100
#define USB_AUDIO_CONFIG_FREQ_32_K   32000
#define USB_AUDIO_CONFIG_FREQ_16_K   16000
#define USB_AUDIO_CONFIG_FREQ_8_K    8000 
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USB_AUDIO_CONSTANTS_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
