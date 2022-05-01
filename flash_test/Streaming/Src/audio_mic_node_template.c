/**
  ******************************************************************************
  * @file    audio_mic_node_template.c
  * @author  MCD Application Team 
  * @brief   template of mic node implementation.
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
#include "usbd_audio.h"
#include "audio_mic_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* externals variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* list of Mic Callbacks */
static int8_t  TEMPLATE_AUDIO_MicDeInit(uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicStop( uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicChangeFrequency( uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicMute(uint16_t channel_number,  uint8_t mute, uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256, uint32_t node_handle);
static int8_t  TEMPLATE_AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
static int8_t  TEMPLATE_AUDIO_MicStartReadCount( uint32_t node_handle);
static uint16_t TEMPLATE_AUDIO_MicGetLastReadCount( uint32_t node_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
/* Private variables ---------------------------------------------------------*/
/* exported functions ---------------------------------------------------------*/
/**
  * @brief  TEMPLATE_AUDIO_MicInit
  *         Initializes the audio mic node 
  * @param  audio_description:   audio parameters
  * @param  session_handle:     session handle
  * @param  node_handle:        mic node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  TEMPLATE_AUDIO_MicInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle,
                       uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;

  mic   = (AUDIO_MicNode_t*)node_handle;
  memset(mic, 0, sizeof(AUDIO_MicNode_t));
  mic->node.type                = AUDIO_INPUT;
  mic->node.state               = AUDIO_NODE_INITIALIZED;
  mic->node.session_handle      = session_handle;
  mic->node.audio_description   = audio_description;
  mic->MicDeInit                = TEMPLATE_AUDIO_MicDeInit;
  mic->MicStart                 = TEMPLATE_AUDIO_MicStart;
  mic->MicStop                  = TEMPLATE_AUDIO_MicStop;
  mic->MicChangeFrequency       = TEMPLATE_AUDIO_MicChangeFrequency;
  mic->MicMute                  = TEMPLATE_AUDIO_MicMute;
  mic->MicSetVolume             = TEMPLATE_AUDIO_MicSetVolume;
  mic->MicGetVolumeDefaultsValues = TEMPLATE_AUDIO_MicGetVolumeDefaultsValues;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  mic->MicStartReadCount        = TEMPLATE_AUDIO_MicStartReadCount;
  mic->MicGetReadCount          = TEMPLATE_AUDIO_MicGetLastReadCount;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
  mic->volume                           = 0; /* add here the volume in db_256 */
  mic->packet_length                    = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_description);
  /* add specific mic init operations here */
  return 0;
}
/* private functions ---------------------------------------------------------*/
/**
  * @brief  TEMPLATE_AUDIO_MicDeInit
  *         De-Initializes the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicDeInit(uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state != AUDIO_NODE_OFF)
  {
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
      TEMPLATE_AUDIO_MicStop(node_handle);
    }
/* add Mic denit operations here*/
    mic->node.state = AUDIO_NODE_OFF;
  }
  
    return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_MicStart
  *         Start the audio mic node 
  * @param  buffer:      Audio data buffer       
  * @param  node_handle: mic node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  mic=(AUDIO_MicNode_t*)node_handle;

  if(mic->node.state != AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STARTED;
    mic->buf        = buffer;
  }
  /* add  Mic start code here*/
    return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_MicStop
  *         stop the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicStop( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;
  mic = (AUDIO_MicNode_t*)node_handle;

  if(mic->node.state == AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STOPPED;
  }
    /* add  Mic stop code here*/
    return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_MicChangeFrequency
  *         change mic frequency 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicChangeFrequency( uint32_t node_handle)
{
    
  /* add Mic change frequency  code here*/
  
    return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_MicMute
  *         mute  mic 
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicMute(uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
    /* add  Mic mute code here*/
  return 0;
}

/**
  * @brief  TEMPLATE_AUDIO_MicSetVolume
  *         set  mic volume 
  * @param  channel_number : which channel to set the volume (not used currently)
  * @param  volume_db_256  : description is missing
  * @param  node_handle      mic  node handle
  * @retval 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
      /* add  Mic set volume code here*/
  return 0;
}
/**
  * @brief  TEMPLATE_AUDIO_MicGetVolumeDefaultsValues
  *         get  mic volume mx min & resolution value  in db
  * @param  vol_max            
  * @param  vol_min            
  * @param  vol_res             
  * @param  node_handle         
  * @retval 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle)
{
    /* Change next values */
  *vol_max = 0;
  *vol_min = 0;
  *vol_res = 0;
  return 0;
}

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
/**
  * @brief  TEMPLATE_AUDIO_MicStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  TEMPLATE_AUDIO_MicStartReadCount( uint32_t node_handle)
{
  /* Change next code to initialize codec readen samples bytes counter */
  return 0;
} 
/**
  * @brief  TEMPLATE_AUDIO_MicGetLastReadCount
  *         read the number of bytes that was read in the last 1ms 
  * @param  node_handle: mic node handle must be started
  * @retval number of read bytes, 0 if an error
  */    
static uint16_t  TEMPLATE_AUDIO_MicGetLastReadCount( uint32_t node_handle)
{
  /* return codec readen samples bytes counter */
    return 0;
}
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
