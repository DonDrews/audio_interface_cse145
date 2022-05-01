/**
  ******************************************************************************
  * @file    audio_speaker_node_template.c
  * @author  MCD Application Team 
  * @brief   Template for speaker node implementation.
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usbd_audio.h"
#include "audio_speaker_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static int8_t  TEMPLATE_AUDIO_SpeakerDeInit(uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer, uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerStop( uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerChangeFrequency( uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume ,  uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_SpeakerStartReadCount( uint32_t node_handle);
static uint16_t TEMPLATE_AUDIO_SpeakerGetLastReadCount( uint32_t node_handle);

/* Private typedef -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/* Private variables -----------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  TEMPLATE_AUDIO_SpeakerInit
  *         Initializes the audio speaker node 
  * @param  audio_description: audio parameters
  * @param  session_handle:   session handle
  * @param  node_handle:      speaker node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  TEMPLATE_AUDIO_SpeakerInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle, 
                           uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  memset(speaker, 0, sizeof(AUDIO_SpeakerNode_t));
  speaker->node.type = AUDIO_OUTPUT;
  speaker->node.state = AUDIO_NODE_INITIALIZED;
  speaker->node.session_handle = session_handle;
  speaker->node.audio_description = audio_description;
  /* set callbacks */
  speaker->SpeakerDeInit = TEMPLATE_AUDIO_SpeakerDeInit;
  speaker->SpeakerStart = TEMPLATE_AUDIO_SpeakerStart;
  speaker->SpeakerStop = TEMPLATE_AUDIO_SpeakerStop;
  speaker->SpeakerChangeFrequency = TEMPLATE_AUDIO_SpeakerChangeFrequency;
  speaker->SpeakerMute = TEMPLATE_AUDIO_SpeakerMute;
  speaker->SpeakerSetVolume = TEMPLATE_AUDIO_SpeakerSetVolume;
  speaker->SpeakerStartReadCount = TEMPLATE_AUDIO_SpeakerStartReadCount;
  speaker->SpeakerGetReadCount = TEMPLATE_AUDIO_SpeakerGetLastReadCount;
/* Add Speaker Init code Here */
  return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  TEMPLATE_AUDIO_SpeakerDeInit
  *         De-Initializes the audio speaker node 
  * @param  audio_description: audio parameters
  * @param  node_handle: speaker node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_SpeakerDeInit(uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  if(speaker->node.state != AUDIO_NODE_OFF)
  {
/* Add Speaker DeInit code Here */

    speaker->node.state = AUDIO_NODE_OFF;
  }
  return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_SpeakerStart
  *         Start the audio speaker node 
  * @param  buffer:     buffer to use while node is being started
  * @param  node_handle: speaker node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;

  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->buf = buffer;
/* Add Speaker start code Here */
  speaker->node.state = AUDIO_NODE_STARTED;
  return 0;
}

 /**
  * @brief  TEMPLATE_AUDIO_SpeakerStop
  *         Stop speaker node
  * @param  node_handle: speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  TEMPLATE_AUDIO_SpeakerStop( uint32_t node_handle)
{
/* Add Speaker stop code Here */

  return 0;
}

 /**
  * @brief  TEMPLATE_AUDIO_SpeakerChangeFrequency
  *         change frequency then stop speaker node
  * @param  node_handle: speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  TEMPLATE_AUDIO_SpeakerChangeFrequency( uint32_t node_handle)
{
/* Add Speaker Change frequency code Here */
  return 0;
}

 
 /**
  * @brief  TEMPLATE_AUDIO_SpeakerMute
  *         set Mute value to speaker
  * @param  channel_number: channel number
* @param  mute: mute value (0 : mute , 1 unmute)
  * @param  node_handle: speaker node handle must be Started
  * @retval  : 0 if no error
  */ 
static int8_t  TEMPLATE_AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
/* Add Speaker Mute code Here */

  return 0;
}

 /**
  * @brief  TEMPLATE_AUDIO_SpeakerSetVolume
  *         set Volume value to speaker
  * @param  channel_number: channel number
  * @param  volume_db_256:  volume value in db
  * @param  node_handle:    speaker node handle must be Started
  * @retval 0 if no error
  */ 
static int8_t  TEMPLATE_AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
/* Add Speaker Set Volume code Here */
  return 0;
}

 /**
  * @brief  TEMPLATE_AUDIO_SpeakerStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_SpeakerStartReadCount( uint32_t node_handle)
{
/* Add  code Here */
  return 0;
}

 
 /**
  * @brief  AUDIO_MicGetLastReadCount
  *         read the count of bytes read in last ms
  * @param  node_handle: mic node handle must be started
  * @retval  :  number of read bytes , 0 if  an error
  */    

static uint16_t  TEMPLATE_AUDIO_SpeakerGetLastReadCount( uint32_t node_handle)
{
/* Add  code Here */
  return 0;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
