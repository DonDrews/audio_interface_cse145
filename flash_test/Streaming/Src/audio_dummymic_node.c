
/**
  ******************************************************************************
  * @file    audio_dummymic_node.c
  * @author  MCD Application Team 
  * @brief   mic node implementation.
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
#include "audio_mic_node.h"
#include "usb_audio.h"



/* Private defines -----------------------------------------------------------*/
#define DUMMY_MIC_VOLUME_RES_DB_256     256 /* 1 db 1 * 256 = 256*/ 
#define DUMMY_MIC_VOLUME_MAX_DB_256     8192 /* 32db == 32*256 = 8192*/
#define DUMMY_MIC_VOLUME_MIN_DB_256     -8192 /* -32db == -32*256 = -8192*/

/* Private function prototypes -----------------------------------------------*/
/* list of Mic Callbacks */

static int8_t  AUDIO_MicDeInit(uint32_t node_handle);
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle);
static int8_t  AUDIO_MicStop( uint32_t node_handle);
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute , uint32_t node_handle);
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle);
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle);
/* exported functions ---------------------------------------------------------*/

/**
  * @brief  AUDIO_MicInit
  *         Initializes the audio mic node 
  * @param  audio_description: audio parameters
  * @param  session_handle:   session handle
  * @param  node_handle:      mic node handle must be allocated
  * @retval  : 0 if no error
  */
 int8_t  AUDIO_DUMMY_MicInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle,
                       uint32_t node_handle)
{
   AUDIO_MicNode_t* mic;
  
  mic                             = (AUDIO_MicNode_t*)node_handle;
  memset(mic, 0, sizeof(AUDIO_MicNode_t));
  mic->node.type                  = AUDIO_INPUT;
  mic->node.state                 = AUDIO_NODE_INITIALIZED;
  mic->node.session_handle        = session_handle;
  mic->node.audio_description     = audio_description;
  mic->MicDeInit                  = AUDIO_MicDeInit;
  mic->MicStart                   = AUDIO_MicStart;
  mic->MicStop                    = AUDIO_MicStop;
  mic->MicChangeFrequency         = AUDIO_MicChangeFrequency;
  mic->MicMute                    = AUDIO_MicMute;
  mic->MicSetVolume               = AUDIO_MicSetVolume;
  mic->MicGetVolumeDefaultsValues = AUDIO_MicGetVolumeDefaultsValues;
  mic->volume                     = audio_description->audio_volume_db_256;
  mic->packet_length              = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_description);
  /* @TO add init function for MIC here */
  return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicDeInit
  *         De-Initializes the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state != AUDIO_NODE_OFF)
  {
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
      AUDIO_MicStop(node_handle);
    }
    mic->node.state = AUDIO_NODE_OFF;
  }
  
    return 0;
}

/**
  * @brief  AUDIO_MicStart
  *         Start the audio mic node 
  * @param  buffer:      
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  mic = (AUDIO_MicNode_t*)node_handle;

  if(mic->node.state != AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STARTED;
    mic->buf        = buffer;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicStop
  *         stop the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStop( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;
  mic=(AUDIO_MicNode_t*)node_handle;

  if(mic->node.state == AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STOPPED;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicChangeFrequency
  *         change mic frequency 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;
  
  mic=(AUDIO_MicNode_t*)node_handle;
  if(mic->node.state == AUDIO_NODE_STARTED)
  {
    AUDIO_MicStop(node_handle);
    AUDIO_MicStart(mic->buf, node_handle);
  }
  
    return 0;
}
/**
  * @brief  AUDIO_MicMute
  *         mute  mic 
  * @param  channel_number:   Channel number to mute
  * @param  mute:  1 to mute , 0 to unmute 
  * @param  node_handle:  mic node handle
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicMute(uint16_t channel_number, uint8_t mute, uint32_t node_handle)
{
  /* @TODO check if really mic is muted */
  /* @TO ADD here */
  
  return 0;
}

/**
  * @brief  AUDIO_MicSetVolume
  *         set  mic volume 
  * @param  channel_number: channel number to set volume 
  * @param  volume_db_256:  volume value 
  * @param  node_handle:  mic node handle 
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  ((AUDIO_MicNode_t*)node_handle)->volume = volume_db_256;
    
  /* @TO ADD set volume function here */
  
  return 0;
}

/**
  * @brief  AUDIO_MicGetVolumeDefaultsValues
  *         get  mic volume mx min & resolution value  in db
  * @param  vol_max:   
  * @param  volume_db_256:
  * @param  vol_min:
  * @param  node_handle:
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle)
{
  *vol_max = DUMMY_MIC_VOLUME_RES_DB_256;
  *vol_min = DUMMY_MIC_VOLUME_MIN_DB_256;
  *vol_res = DUMMY_MIC_VOLUME_RES_DB_256;
  return 0;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
