/**
  ******************************************************************************
  * @file    audio_dummyspeaker_node.c
  * @author  MCD Application Team 
  * @brief   Dummy speaker node implementation.
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
static int8_t  AUDIO_SpeakerDeInit(uint32_t node_handle);
static int8_t  AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer, uint32_t node_handle);
static int8_t  AUDIO_SpeakerStop( uint32_t node_handle);
static int8_t  AUDIO_SpeakerChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle);
static int8_t  AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume ,  uint32_t node_handle);
static void    AUDIO_SpeakerInitInjectionsParams( AUDIO_SpeakerNode_t* speaker);
static uint16_t  AUDIO_SpeakerUpdateBuffer(void);
static int8_t  AUDIO_SpeakerStartReadCount( uint32_t node_handle);
static uint16_t AUDIO_SpeakerGetLastReadCount( uint32_t node_handle);

/* Private typedef -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
static AUDIO_SpeakerNode_t *current_speaker = 0;
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  AUDIO_SpeakerInit
  *         Initializes the audio speaker node 
  * @param  audio_description: audio parameters
  * @param  session_handle:   session handle
  * @param  node_handle:      speaker node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  AUDIO_SpeakerInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle, 
                           uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  memset(speaker, 0, sizeof(AUDIO_SpeakerNode_t));
  speaker->node.type = AUDIO_OUTPUT;
  speaker->node.state = AUDIO_NODE_INITIALIZED;
  speaker->node.session_handle = session_handle;
  speaker->node.audio_description = audio_description;
  AUDIO_SpeakerInitInjectionsParams( speaker);

  /* set callbacks */
  speaker->SpeakerDeInit = AUDIO_SpeakerDeInit;
  speaker->SpeakerStart = AUDIO_SpeakerStart;
  speaker->SpeakerStop = AUDIO_SpeakerStop;
  speaker->SpeakerChangeFrequency = AUDIO_SpeakerChangeFrequency;
  speaker->SpeakerMute = AUDIO_SpeakerMute;
  speaker->SpeakerSetVolume = AUDIO_SpeakerSetVolume;
  speaker->SpeakerStartReadCount = AUDIO_SpeakerStartReadCount;
  speaker->SpeakerGetReadCount = AUDIO_SpeakerGetLastReadCount;
  current_speaker = speaker;
  return 0;
}

/**
  * @brief  AUDIO_SpeakerUpdateBuffer
  *         read a packet from the buffer.
  * @param  None
  * @retval None
  */
static uint16_t AUDIO_SpeakerUpdateBuffer(void)
{
  uint16_t wr_distance, read_length = 0;
    
  if((current_speaker)&&(current_speaker->node.state != AUDIO_NODE_OFF))
  {


    /* if speaker was started prepare next data */
    if(current_speaker->node.state == AUDIO_NODE_STARTED)
    {
     
      /* inform session that a packet is played */
      current_speaker->node.session_handle->SessionCallback(AUDIO_PACKET_PLAYED, (AUDIO_Node_t*)current_speaker, 
                                                            current_speaker->node.session_handle);
      /* prepare next size to inject */
      read_length = current_speaker->packet_length;
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
      if(current_speaker->node.audio_description->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)
      {
        if(current_speaker->injection_44_count < 9)
        {
          current_speaker->injection_44_count++;  
        }
        else
        {
           current_speaker->injection_44_count = 0;
           read_length = current_speaker->packet_length_max_44_1;
        }
      }
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K*/
      wr_distance = AUDIO_BUFFER_FILLED_SIZE(current_speaker->buf);
      if(wr_distance < read_length)
      {
        /** inform session that an underrun is happened */
        current_speaker->node.session_handle->SessionCallback(AUDIO_UNDERRUN, (AUDIO_Node_t*)current_speaker, 
                                                  current_speaker->node.session_handle);
        read_length = 0;
      }
      else
      {     
        /* update read pointer */
        current_speaker->buf->rd_ptr += read_length;
        if(current_speaker->buf->rd_ptr >= current_speaker->buf->size)
        {
          current_speaker->buf->rd_ptr = current_speaker->buf->rd_ptr - current_speaker->buf->size;
        }
      }
    } /* current_speaker->node.state == AUDIO_NODE_STARTED */
  }
  
  return read_length;
}


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_SpeakerDeInit
  *         De-Initializes the audio speaker node 
  * @param  audio_description: audio parameters
  * @param  node_handle: speaker node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_SpeakerDeInit(uint32_t node_handle)
{
  /* @TODO implement function */
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  if(speaker->node.state != AUDIO_NODE_OFF)
  {
    speaker->node.state = AUDIO_NODE_OFF;
  }
  current_speaker = 0;
  return 0;
}

/**
  * @brief  AUDIO_SpeakerStart
  *         Start the audio speaker node 
  * @param  buffer:     buffer to use while node is being started
  * @param  node_handle: speaker node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_SpeakerStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;

  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->buf = buffer;
  AUDIO_SpeakerMute( 0,  speaker->node.audio_description->audio_mute , node_handle);
  AUDIO_SpeakerSetVolume( 0,  speaker->node.audio_description->audio_volume_db_256 , node_handle);
  speaker->node.state = AUDIO_NODE_STARTED;
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerStop
  *         Stop speaker node
  * @param  node_handle: speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  AUDIO_SpeakerStop( uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;

  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->node.state = AUDIO_NODE_STOPPED;

  return 0;
}

 /**
  * @brief  AUDIO_SpeakerChangeFrequency
  *         change frequency then stop speaker node
  * @param  node_handle: speaker node handle must be Started
  * @retval 0 if no error
  */  
static int8_t  AUDIO_SpeakerChangeFrequency( uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;

  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->node.state = AUDIO_NODE_STOPPED;
  AUDIO_SpeakerInitInjectionsParams(speaker);
  return 0;
}

 /**
  * @brief  AUDIO_SpeakerInitInjectionsParams
  *         Stop speaker node
  * @param  speaker: speaker node handle must be Started
  * @retval 0 if no error
  */
static void  AUDIO_SpeakerInitInjectionsParams( AUDIO_SpeakerNode_t* speaker)
{
  speaker->packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(speaker->node.audio_description);
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
  if(speaker->node.audio_description->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)
  {
    speaker->packet_length_max_44_1 = speaker->packet_length + AUDIO_SAMPLE_LENGTH(speaker->node.audio_description);
  }
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K*/
 }
 /**
  * @brief  AUDIO_SpeakerMute
  *         set Mute value to speaker
  * @param  channel_number: channel number
* @param  mute: mute value (0 : mute , 1 unmute)
  * @param  node_handle: speaker node handle must be Started
  * @retval  : 0 if no error
  */ 
static int8_t  AUDIO_SpeakerMute( uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;

  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->node.audio_description->audio_mute = mute;

  return 0;
}

 /**
  * @brief  AUDIO_SpeakerSetVolume
  *         set Volume value to speaker
  * @param  channel_number: channel number
  * @param  volume_db_256:  volume value in db
  * @param  node_handle:    speaker node handle must be Started
  * @retval 0 if no error
  */ 
static int8_t  AUDIO_SpeakerSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  speaker->node.audio_description->audio_volume_db_256 = volume_db_256;
  
  return 0;
}
 /**
  * @brief  AUDIO_SpeakerStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_SpeakerStartReadCount( uint32_t node_handle)
{
    return 0;    
}
 /**
  * @brief  AUDIO_MicGetLastReadCount
  *         read the count of bytes read in last ms
  * @param  node_handle: mic node handle must be started
  * @retval  :  number of read bytes , 0 if  an error
  */    

static uint16_t  AUDIO_SpeakerGetLastReadCount( uint32_t node_handle)
{
  int read_bytes;
  AUDIO_SpeakerNode_t* speaker;
  
  speaker = (AUDIO_SpeakerNode_t*)node_handle;
  read_bytes = AUDIO_SpeakerUpdateBuffer()/(speaker->node.audio_description->resolution);
    
  return read_bytes;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
