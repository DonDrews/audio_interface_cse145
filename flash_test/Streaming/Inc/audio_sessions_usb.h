/**
  ******************************************************************************
  * @file    audio_sessions_usb.h
  * @author  MCD Application Team 
  * @brief   header file for the audio_session_usb.c file.
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
#ifndef __AUDIO_SESSIONS_USB_H
#define __AUDIO_SESSIONS_USB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_node.h"
#include "audio_usb_nodes.h"

/* Exported types ------------------------------------------------------------*/
#if USE_AUDIO_USB_INTERRUPT
/* List of command that may be applied locally */
typedef enum 
{
  USBD_AUDIO_MUTE_UNMUTE,
  USBD_AUDIO_VOLUME                                                     
}AUDIO_ControlCommand_t;
#endif /* USE_AUDIO_USB_INTERRUPT*/
/* USB session structure: may be instantiated as recording session or playback session */
typedef struct    AUDIO_USB_StreamingSession
{
  AUDIO_Session_t session; /* the session structure */
  int8_t               (*SessionDeInit)       (uint32_t /*session handle*/);
#if USE_AUDIO_USB_INTERRUPT
  int8_t               (*ExternalControl)(AUDIO_ControlCommand_t /*control*/ , uint32_t /*val*/, uint32_t/*  session_handle*/);/* function that may be called locally to execute some commands like set volume */
#endif /*USE_AUDIO_USB_INTERRUPT*/
  uint8_t              interface_num; /* USB interface number*/
  uint8_t              alternate; /* current alternate setting*/
  AUDIO_CircularBuffer_t  buffer; /* Audio circular buffer */
}
AUDIO_USBSession_t;
 
#if USE_USB_AUDIO_PLAYBACK
 int8_t  AUDIO_PlaybackSessionInit(USBD_AUDIO_AS_InterfaceTypeDef* as_desc,
                                    USBD_AUDIO_ControlTypeDef* controls_desc,
                                    uint8_t* control_count, uint32_t session_handle);
#endif /* USE_USB_AUDIO_PLAYBACK*/
#if  USE_USB_AUDIO_RECORDING
 int8_t  AUDIO_RecordingSessionInit(USBD_AUDIO_AS_InterfaceTypeDef* as_desc,
                                     USBD_AUDIO_ControlTypeDef* controls_desc,
                                     uint8_t* control_count, uint32_t session_handle);
#ifdef  USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
 int8_t  USB_AudioRecordingSynchronizationGetSamplesCountToAddInNextPckt(struct AUDIO_Session* session_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
#endif /* USE_USB_AUDIO_RECORDING*/
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_SESSIONS_USB_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
