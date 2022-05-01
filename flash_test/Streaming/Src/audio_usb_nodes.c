/**
  ******************************************************************************
  * @file    audio_usb_nodes.c
  * @author  MCD Application Team 
  * @brief   Usb input output and Feature unit implementation.
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
#include "usb_audio.h"
#include "audio_usb_nodes.h"

/* External variables --------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#if USE_USB_AUDIO_CLASS_10
#if (defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES)
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
#define USE_AUDIO_USB_MULTI_FREQUENCIES 1
#else /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES */
#error "USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES must be defined to support multi-frequencies"
#endif /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES */
#endif /*(defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES) */
#endif /* USE_USB_AUDIO_CLASS_10 */

#define DEBUG_USB_NODES  0  /* set to 1  to debug USB input for playback */
#if DEBUG_USB_NODES
#define USB_INPUT_NODE_DEBUG_BUFFER_SIZE 1000
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t time;
  uint16_t write;
  uint16_t read;
  uint16_t error;
} AUDIO_USBInputBufferDebugStats_t;
#endif /* DEBUG_USB_NODES */

/* Private function prototypes -----------------------------------------------*/
static int8_t     USB_AudioStreamingInputOutputDeInit(uint32_t node_handle);
static int8_t     USB_AudioStreamingInputOutputStart( AUDIO_CircularBuffer_t* buffer, uint16_t threshold ,uint32_t node_handle);
static int8_t     USB_AudioStreamingInputOutputStop( uint32_t node_handle);
static uint16_t   USB_AudioStreamingInputOutputGetMaxPacketLength(uint32_t node_handle);
#if USE_USB_AUDIO_CLASS_10
static int8_t     USB_AudioStreamingInputOutputGetState(uint32_t node_handle);
#endif /*USE_USB_AUDIO_CLASS_10*/
static int8_t     USB_AudioStreamingInputOutputRestart( uint32_t node_handle);
#if USE_USB_AUDIO_PLAYBACK
static int8_t     USB_AudioStreamingInputDataReceived( uint16_t data_len,uint32_t node_handle);
static uint8_t*   USB_AudioStreamingInputGetBuffer(uint32_t node_handle, uint16_t* max_packet_length);
#endif /* USE_USB_AUDIO_PLAYBACK*/
#if  USE_USB_AUDIO_RECORDING
static uint8_t*   USB_AudioStreamingOutputGetBuffer(uint32_t node_handle, uint16_t* max_packet_length);
#endif /* USE_USB_AUDIO_RECORDING*/

static int8_t USB_AudioStreamingFeatureUnitDInit(uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitStart(AUDIO_USBFeatureUnitCommands_t* commands, uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitStop( uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitGetMute(uint16_t channel,uint8_t* mute, uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitSetMute(uint16_t channel,uint8_t mute, uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitSetCurVolume(uint16_t channel, uint16_t volume, uint32_t node_handle);
static int8_t USB_AudioStreamingFeatureUnitGetCurVolume(uint16_t channel, uint16_t* volume, uint32_t node_handle);
#if USE_USB_AUDIO_CLASS_10
static int8_t USB_AudioStreamingFeatureUnitGetStatus(uint32_t node_handle);
#endif /*USE_USB_AUDIO_CLASS_10*/
#if USE_USB_AUDIO_CLASS_10
#ifdef USE_AUDIO_USB_MULTI_FREQUENCIES  
static int8_t  USB_AudioStreamingInputOutputGetCurFrequency(uint32_t* freq, uint32_t node_handle);
static int8_t  USB_AudioStreamingInputOutputSetCurFrequency(uint32_t freq,uint8_t*  usb_ep_restart_is_required , uint32_t node_handle);
static uint32_t  USB_AudioStreamingGetNearestFrequency(uint32_t freq,  uint32_t* freq_table, int freq_count);
#endif /*USE_AUDIO_USB_MULTI_FREQUENCIES*/
/* Private variables --------------------------------------------------------*/
#ifdef USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES
/* declare table of all supported frequencies, to select frequency when set frequency control is received */
 uint32_t USB_AUDIO_CONFIG_PLAY_FREQENCIES[USB_AUDIO_CONFIG_PLAY_FREQ_COUNT]=
{
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_192_K
USB_AUDIO_CONFIG_FREQ_192_K,
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_192_K */
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_96_K
USB_AUDIO_CONFIG_FREQ_96_K,
#endif /* USB_AUDIO_CONFIG_PLAY_USE_FREQ_96_K */
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_48_K
USB_AUDIO_CONFIG_FREQ_48_K,
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_48_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K
USB_AUDIO_CONFIG_FREQ_44_1_K,
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_44_1_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_16_K
USB_AUDIO_CONFIG_FREQ_16_K,
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_16_K*/
#if USB_AUDIO_CONFIG_PLAY_USE_FREQ_8_K
USB_AUDIO_CONFIG_FREQ_8_K,
#endif /*USB_AUDIO_CONFIG_PLAY_USE_FREQ_8_K*/
};
#endif /* USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES*/
#ifdef USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES
 uint32_t USB_AUDIO_CONFIG_RECORD_FREQENCIES[USB_AUDIO_CONFIG_RECORD_FREQ_COUNT]=
{
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_192_K
USB_AUDIO_CONFIG_FREQ_192_K,
#endif /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_192_K */
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K
USB_AUDIO_CONFIG_FREQ_96_K,
#endif /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K */
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_48_K
USB_AUDIO_CONFIG_FREQ_48_K,
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_48_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K
USB_AUDIO_CONFIG_FREQ_44_1_K,
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_32_K
USB_AUDIO_CONFIG_FREQ_32_K,
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_32_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_16_K
USB_AUDIO_CONFIG_FREQ_16_K,
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_16_K*/
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_8_K
USB_AUDIO_CONFIG_FREQ_8_K,
#endif /*USB_AUDIO_CONFIG_RECORD_USE_FREQ_8_K*/
};
#endif /* USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES*/
#endif /* USE_USB_AUDIO_CLASS_10 */

#if DEBUG_USB_NODES
static AUDIO_USBInputBufferDebugStats_t stats_buffer [USB_INPUT_NODE_DEBUG_BUFFER_SIZE];
static int stats_count=0;
extern __IO uint32_t uwTick;
#endif /* DEBUG_USB_NODES */

/* Functions ---------------------------------------------------------*/
#if USE_USB_AUDIO_PLAYBACK
/**
  * @brief  USB_AudioStreamingInputInit Initializes the USB Audio input node it represents a USB Input terminal
  *         
  * @param  data_ep (OUT):            List of information (like endpoint number, maxpacket size and supported controls) 
  *                                   and callbacks to communicate with the USB Audio Class module.
  * @param  audio_desc(IN):         Supported audio properties.
  * @param  session_handle(IN):     the mother session handle
  * @param  node_handle(IN):        the node handle, node must be already allocated
  * @retval 0 if error
  */
 int8_t  USB_AudioStreamingInputInit(USBD_AUDIO_EP_DataTypeDef* data_ep,
                                              AUDIO_Description_t* audio_desc,
                                              AUDIO_Session_t* session_handle,  uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t * input_node;
  
  input_node = (AUDIO_USBInputOutputNode_t *)node_handle;
  input_node->node.audio_description = audio_desc;
  input_node->node.session_handle = session_handle;
  input_node->flags = 0;
  input_node->node.state = AUDIO_NODE_INITIALIZED;
  input_node->node.type = AUDIO_INPUT;
  /* set the node  callback wich are called by session */
  input_node->IODeInit = USB_AudioStreamingInputOutputDeInit;
  input_node->IOStart = USB_AudioStreamingInputOutputStart;
  input_node->IORestart = USB_AudioStreamingInputOutputRestart;
  input_node->IOStop = USB_AudioStreamingInputOutputStop;
  input_node->packet_length = AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc);
  /* compute the max packet length */
  #if USE_AUDIO_PLAYBACK_USB_FEEDBACK
  input_node->max_packet_length = AUDIO_MAX_PACKET_WITH_FEEDBACK_LENGTH(audio_desc);
  #else
    input_node->max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc);
  #endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
  /* set data end point callbacks to be called by USB class */
  data_ep->ep_num = USBD_AUDIO_CONFIG_PLAY_EP_OUT;
  data_ep->control_name_map = 0;
  data_ep->control_selector_map = 0;
  data_ep->private_data = node_handle;
  data_ep->DataReceived = USB_AudioStreamingInputDataReceived;
  data_ep->GetBuffer = USB_AudioStreamingInputGetBuffer;
  data_ep->GetMaxPacketLength = USB_AudioStreamingInputOutputGetMaxPacketLength;
#if USE_USB_AUDIO_CLASS_10
  data_ep->GetState = USB_AudioStreamingInputOutputGetState;
#ifdef USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES
  data_ep->control_selector_map = USBD_AUDIO_CONTROL_EP_SAMPL_FREQ;
  data_ep->control_cbk.GetCurFrequency = USB_AudioStreamingInputOutputGetCurFrequency;
  data_ep->control_cbk.SetCurFrequency = USB_AudioStreamingInputOutputSetCurFrequency;
  data_ep->control_cbk.MaxFrequency = USB_AUDIO_CONFIG_PLAY_FREQENCIES[0];
  data_ep->control_cbk.MinFrequency = USB_AUDIO_CONFIG_PLAY_FREQENCIES[USB_AUDIO_CONFIG_PLAY_FREQ_COUNT-1];
  data_ep->control_cbk.ResFrequency = 1; 
#endif /* USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES */
#endif /* USE_USB_AUDIO_CLASS_10 */
  return 0;
}
#endif /* USE_USB_AUDIO_PLAYBACK*/
#if  USE_USB_AUDIO_RECORDING
/**
  * @brief  USB_AudioStreamingOutputInit Initializes the USB Audio Output node it represents a USB output terminal
  *         
  * @param  data_ep (OUT):            List of information (like endpoint number, maxpacket size and supported controls) 
  *                                   and callbacks to communicate with the USB Audio Class module.
  * @param  audio_desc(IN):         Supported audio properties.
  * @param  session_handle(IN):     the mother session handle
  * @param  node_handle(IN):        the node handle, node must be already allocated
  * @retval 0 if error
  */
 int8_t  USB_AudioStreamingOutputInit(USBD_AUDIO_EP_DataTypeDef* data_ep,
                                         AUDIO_Description_t* audio_desc,
                                         AUDIO_Session_t* session_handle,  uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t * output_node;
  output_node=(AUDIO_USBInputOutputNode_t *)node_handle;
  
  output_node->node.audio_description = audio_desc;
  output_node->node.session_handle = session_handle;
  output_node->node.state = AUDIO_NODE_INITIALIZED;
  output_node->node.type = AUDIO_OUTPUT;
#if  USE_AUDIO_RECORDING_USB_NO_REMOVE
  output_node->max_packet_length = AUDIO_MAX_PACKET_WITH_FEEDBACK_LENGTH(audio_desc);
#else /*USE_AUDIO_RECORDING_USB_NO_REMOVE */
  output_node->max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(audio_desc);
#endif /*USE_AUDIO_RECORDING_USB_NO_REMOVE*/
  output_node->packet_length = AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(audio_desc);
  /* allocate and initialize the alternative buffer.It is filled with zero and it is sent to USB host  when no enough data are ready.*/
  output_node->specific.output.alt_buff = (uint8_t *) malloc(output_node->max_packet_length);
  if(output_node->specific.output.alt_buff)
  {
    memset(output_node->specific.output.alt_buff, 0, output_node->max_packet_length);
  }
  else
  {
    Error_Handler();
  }
  output_node->IODeInit = USB_AudioStreamingInputOutputDeInit;
  output_node->IOStart = USB_AudioStreamingInputOutputStart;
  output_node->IOStop = USB_AudioStreamingInputOutputStop;
  output_node->IORestart = USB_AudioStreamingInputOutputRestart;
  /* set data end point callbacks */
  data_ep->ep_num = USB_AUDIO_CONFIG_RECORD_EP_IN;
  data_ep->control_name_map = 0;
  data_ep->control_selector_map = 0;
  data_ep->private_data = node_handle;
  data_ep->DataReceived = 0;
  data_ep->GetBuffer = USB_AudioStreamingOutputGetBuffer;
  data_ep->GetMaxPacketLength = USB_AudioStreamingInputOutputGetMaxPacketLength;
#if USE_USB_AUDIO_CLASS_10
  data_ep->GetState = USB_AudioStreamingInputOutputGetState;

#ifdef USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES
  data_ep->control_selector_map = USBD_AUDIO_CONTROL_EP_SAMPL_FREQ;
  data_ep->control_cbk.GetCurFrequency = USB_AudioStreamingInputOutputGetCurFrequency;
  data_ep->control_cbk.SetCurFrequency = USB_AudioStreamingInputOutputSetCurFrequency;
  data_ep->control_cbk.MaxFrequency = USB_AUDIO_CONFIG_RECORD_FREQENCIES[0];
  data_ep->control_cbk.MinFrequency = USB_AUDIO_CONFIG_RECORD_FREQENCIES[USB_AUDIO_CONFIG_RECORD_FREQ_COUNT-1];
  data_ep->control_cbk.ResFrequency = 1; 
#endif /* USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES */
#endif /* USE_USB_AUDIO_CLASS_10 */
  return 0;
}

#endif /* USE_USB_AUDIO_RECORDING*/
/**
  * @brief  USB_AudioStreamingInputOutputDeInit
  *         De-Initializes the AUDIO usb input node
  * @param  node_handle(IN): the node handle, node must be allocated
  * @retval  0 for no error
  */
 static int8_t  USB_AudioStreamingInputOutputDeInit(uint32_t node_handle)
{
  ((AUDIO_USBInputOutputNode_t *)node_handle)->node.state = AUDIO_NODE_OFF;
  return 0;
}


/**
  * @brief  USB_AudioStreamingInputOutputStart
  *         Start Usb input  or output node: then the node is ready to receive/send packets from the host. should called as soon as set alternate setting to one is received
  * @param  buffer(IN):             the main Circular buffer.
  * @param  threshold(IN):          It is a threshold for Circular buffer. After streaming starts,buffer write is allowed and buffer read is blocked.
  *                             Then when threshold is reached an event is raised to the session and buffer read is unlocked 
  * @param  node_handle(IN):        the node handle, node must be already initialized
  * @retval 0 if no error
  */
static int8_t  USB_AudioStreamingInputOutputStart( AUDIO_CircularBuffer_t* buffer, uint16_t threshold ,uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t * io_node;

  io_node = (AUDIO_USBInputOutputNode_t *)node_handle;
  if((io_node->node.state == AUDIO_NODE_INITIALIZED ) ||(io_node->node.state == AUDIO_NODE_STOPPED))
  {
     io_node->node.state = AUDIO_NODE_STARTED;
     io_node->buf = buffer;
     io_node->buf->rd_ptr = io_node->buf->wr_ptr=0;
     io_node->flags = 0;
     if(io_node->node.type == AUDIO_INPUT)
     {
       io_node->specific.input.threshold = threshold;
     }
     else
     {
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K
       if(io_node->node.audio_description->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)
       {
         io_node->specific.output.packet_44_counter = 0;
       }
#endif /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K */
     }
  }
  return 0;
}

/**
  * @brief  USB_AudioStreamingInputOutputStop
  *         Stops Usb input or output node
  * @param  node_handle: the node handle, node must be initialized
  * @retval  0 for no error
  */
static int8_t  USB_AudioStreamingInputOutputStop( uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t * io_node;
  io_node = (AUDIO_USBInputOutputNode_t *)node_handle;
  io_node->node.state = AUDIO_NODE_STOPPED;
  return 0;
}
/**
  * @brief  USB_AudioStreamingInputOutputRestart
  *         Called when the node restart is required, for example after frequency change
  * @param  node_handle(IN): 
  * @retval  0 for no error
  */
static int8_t  USB_AudioStreamingInputOutputRestart( uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t * io_node;
  io_node = (AUDIO_USBInputOutputNode_t *)node_handle;
  if(io_node->node.state == AUDIO_NODE_STARTED)
  {
    io_node->flags = AUDIO_IO_RESTART_REQUIRED;   /* this flag to stop node when next time USB Audio class calls the node via callback*/
    return 0;
  }
  return 0;
}

#if USE_USB_AUDIO_PLAYBACK
/**
  * @brief  USB_AudioStreamingInputDataReceived
  *         callback called by USB class when new packet is received
  * @param  data_len(IN):           packet length
  * @param  node_handle(IN):        the input node handle, node must be initialized  and started
  * @retval  0 if no error
  */
static int8_t  USB_AudioStreamingInputDataReceived( uint16_t data_len, uint32_t node_handle)
 {
   AUDIO_USBInputOutputNode_t * input_node;
   AUDIO_CircularBuffer_t *buf;
   uint16_t buffer_data_count;
   
   input_node = (AUDIO_USBInputOutputNode_t *)node_handle;
   if(input_node->node.state == AUDIO_NODE_STARTED)
   {
     /* @TODO add overrun detection */
     if(input_node->flags&AUDIO_IO_RESTART_REQUIRED)
     { 
     /* When restart is required ignore the packet and reset buffer */
       input_node->flags = 0;
       input_node->buf->rd_ptr=input_node->buf->wr_ptr = 0;
       return 0;
     }
     
     buf=input_node->buf;
     buf->wr_ptr += data_len;/* increment buffer */

     if((input_node->flags&AUDIO_IO_BEGIN_OF_STREAM) == 0)
     { /* this is the first packet */
       input_node->node.session_handle->SessionCallback(AUDIO_BEGIN_OF_STREAM,(AUDIO_Node_t*)input_node,
                                                        input_node->node.session_handle);   /* send event to mother session */
       input_node->flags |= AUDIO_IO_BEGIN_OF_STREAM;
     }
     else
     {   /* if some sample are in the margin area , then copy them to regular area */
        if(buf->wr_ptr > buf->size)
        {
          buf->wr_ptr -= buf->size;
          memcpy(buf->data, buf->data+buf->size, buf->wr_ptr);
        }
      /* count pending audio samples in the buffer */
      buffer_data_count = AUDIO_BUFFER_FILLED_SIZE(buf); 
      if(buf->wr_ptr == buf->size)
      {
        buf->wr_ptr = 0;
      }
      if(((input_node->flags&AUDIO_IO_THRESHOLD_REACHED) == 0)&&
          (buffer_data_count >= input_node->specific.input.threshold))
      {  
         input_node->node.session_handle->SessionCallback(AUDIO_THRESHOLD_REACHED, (AUDIO_Node_t*)input_node,
                                                         input_node->node.session_handle);   /* inform session that the buffer threshold is reached */
          input_node->flags |= AUDIO_IO_THRESHOLD_REACHED ;
       }
       else
       {
        input_node->node.session_handle->SessionCallback(AUDIO_PACKET_RECEIVED, (AUDIO_Node_t*)input_node,
                                                         input_node->node.session_handle); /* inform session that a packet is received */
       }
     }
    }
   else
   {
     Error_Handler();
   }
   return 0;
 }

/**
  * @brief  USB_AudioStreamingInputGetBuffer
  *         callback called by USB Audio class to get working buffer in order to receive next packet           
  * @param  node_handle(IN):        the input node handle, node must be initialized and started
  * @param  max_packet_length(OUT):  max packet length to be received
  * @retval  0 for no error                          
  */
static uint8_t* USB_AudioStreamingInputGetBuffer(uint32_t node_handle, uint16_t* max_packet_length)
{
  AUDIO_USBInputOutputNode_t* input_node;
  uint16_t buffer_free_size;
  
  input_node = (AUDIO_USBInputOutputNode_t *)node_handle;
#if DEBUG_USB_NODES
  stats_buffer[stats_count].read = input_node->buf->rd_ptr;
  stats_buffer[stats_count].write = input_node->buf->wr_ptr;
  stats_buffer[stats_count].time = uwTick;
  
  stats_count++;
  if(stats_count == USB_INPUT_NODE_DEBUG_BUFFER_SIZE)
  {
    stats_count=0;
  }
#endif /*DEBUG_USB_NODES*/
  *max_packet_length = input_node->max_packet_length;
  if( input_node->node.state == AUDIO_NODE_STARTED)
  {
    /* control of possible overflow */
    buffer_free_size  = AUDIO_BUFFER_FREE_SIZE(input_node->buf);
    
    if(buffer_free_size < input_node->max_packet_length)
    {
      input_node->node.session_handle->SessionCallback(AUDIO_OVERRUN, (AUDIO_Node_t*)input_node,
                                                       input_node->node.session_handle);
    }
    
    if(input_node->flags&AUDIO_IO_RESTART_REQUIRED)
    {
     input_node->flags = 0;
     input_node->buf->rd_ptr = input_node->buf->wr_ptr = 0;
    }
    return input_node->buf->data+input_node->buf->wr_ptr;
  }
  else
  {
    Error_Handler();
    return 0; /* return statement non reachable */
  }
}
#endif /* USE_USB_AUDIO_PLAYBACK*/

#if  USE_USB_AUDIO_RECORDING
/**
  * @brief  USB_AudioStreamingOutputGetBuffer
  *         callback called by USB class to get working buffer to send next packet   
  * @param  node_handle(IN):        the output node handle, node must be initialized and started
  * @param  packet_length(OUT):      max data length to send         
  * @retval  0 if no error     
  */
static uint8_t* USB_AudioStreamingOutputGetBuffer(uint32_t node_handle,uint16_t* packet_length)
{

   AUDIO_USBInputOutputNode_t *output_node;
   uint16_t buffer_data_count;
   AUDIO_CircularBuffer_t *buf;
   uint8_t* packet_data;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
   int8_t sample_add_remove;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */

   output_node = (AUDIO_USBInputOutputNode_t *)node_handle;

   if(output_node->node.state == AUDIO_NODE_STARTED)
   {
     if(output_node->flags&AUDIO_IO_RESTART_REQUIRED)
     {
     /* a restart is required then just reinitialize buffer  and use the alt buffer as no samples are ready*/
       output_node->flags = 0;
       output_node->buf->rd_ptr = 0;
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K
      if(output_node->node.audio_description->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)
      {
        output_node->specific.output.packet_44_counter = 0;
      }
#endif /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K */
       return output_node->specific.output.alt_buff;
     }
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
      output_node->node.session_handle->SessionCallback(AUDIO_PACKET_PLAYED, (AUDIO_Node_t*)output_node,
                                                        output_node->node.session_handle);/* inform session that a packet is sent to the host */
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
#if USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K
/* specific treatment to the fractional frequencies , as the packet haven't the same size */
    if(output_node->node.audio_description->frequency == USB_AUDIO_CONFIG_FREQ_44_1_K)
    {

#ifdef USE_USB_HS
        if(output_node->specific.output.packet_44_counter == 79)
#else /* USE_USB_HS */
      if(output_node->specific.output.packet_44_counter == 9)
#endif /* USE_USB_HS */
      {
        *packet_length = output_node->max_packet_length;
        output_node->specific.output.packet_44_counter = 0;
      }
      else
      {
        output_node->specific.output.packet_44_counter ++;
        *packet_length = output_node->packet_length;
      }
    }
    else
    {
      *packet_length = output_node->packet_length;
    }
#else /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K */
    *packet_length = output_node->packet_length;
#endif /* USB_AUDIO_CONFIG_RECORD_USE_FREQ_44_1_K */
    
     buf = output_node->buf;
      /* @TODO add underrun detection */
     if(!(output_node->flags&AUDIO_IO_BEGIN_OF_STREAM))
     { 
     if(buf->wr_ptr < (buf->size>>1)) /* first threshold is a half of buffer */
      {
        /* buffer is not ready  */
        return output_node->specific.output.alt_buff;
      }
       output_node->node.session_handle->SessionCallback(AUDIO_BEGIN_OF_STREAM, (AUDIO_Node_t*)output_node,
                                                        output_node->node.session_handle);
       output_node->flags |= AUDIO_IO_BEGIN_OF_STREAM;
     }
       /* Check for underrun */
      buffer_data_count = AUDIO_BUFFER_FILLED_SIZE(buf);       
      if(buffer_data_count < *packet_length)
      {
       output_node->node.session_handle->SessionCallback(AUDIO_UNDERRUN, (AUDIO_Node_t*)output_node,
                                                        output_node->node.session_handle);
        return output_node->specific.output.alt_buff;
      }
      else
      {
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
      sample_add_remove = USB_AudioRecordingSynchronizationGetSamplesCountToAddInNextPckt(output_node->node.session_handle);
#if  USE_AUDIO_RECORDING_USB_NO_REMOVE
      *packet_length += sample_add_remove;
      if(*packet_length > output_node->max_packet_length)
      {
        *packet_length = output_node->max_packet_length;
      }
      USB_AudioRecordingSynchronizationNotificationSamplesRead(output_node->node.session_handle, *packet_length);
#else /*USE_AUDIO_RECORDING_USB_NO_REMOVE */
        if(sample_add_remove<0)
        {
            *packet_length += sample_add_remove;
            
        }
        else
        {
          if(sample_add_remove>0)
          {
            buf->rd_ptr += sample_add_remove;
            if(buf->rd_ptr == buf->size)
            {
              buf->rd_ptr = 0;
            }
          }
        }
        USB_AudioRecordingSynchronizationNotificationSamplesRead(output_node->node.session_handle, *packet_length+sample_add_remove);
#endif /*USE_AUDIO_RECORDING_USB_NO_REMOVE*/
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO */
         /* increment read pointer */
        packet_data = buf->data + buf->rd_ptr;
        buf->rd_ptr += *packet_length;
        
        /* manage the packet not aligned */
        if(buf->rd_ptr > buf->size)
        {
          buf->rd_ptr -= buf->size;
          memcpy(buf->data+buf->size, buf->data, buf->rd_ptr);
          if(buf->rd_ptr+buf->size >=USB_AUDIO_CONFIG_RECORD_BUFFER_SIZE)
          {
            Error_Handler();
          }
         }
          
        if(buf->rd_ptr == buf->size)
        {
          buf->rd_ptr = 0;
        }
      }
     return (packet_data);
   }
   else
   {
     /*Should not happen */
     Error_Handler();
     return 0; /* return statement not reachable */
   }
 
}
#endif /* USE_USB_AUDIO_RECORDING*/

/**
  * @brief  USB_AudioStreamingInputOutputGetMaxPacketLength
  *         return max packet length  it is called by the USB Audio Class
  * @param  node_handle(IN): the input node handle, node must be initialized
  * @retval  max packet length
*/
static uint16_t  USB_AudioStreamingInputOutputGetMaxPacketLength(uint32_t node_handle)
{
  
  return ((AUDIO_USBInputOutputNode_t *)node_handle)->max_packet_length;
}

#if USE_USB_AUDIO_CLASS_10
#ifdef USE_AUDIO_USB_MULTI_FREQUENCIES 
/**
  * @brief  USB_AudioStreamingInputOutputGetCurFrequency
  *         return current frequency, called by USB Audio Class
  * @param  node_handle: the usb io node handle, node must be initialized
  * @param  freq(OUT): Current frequency                   
  * @retval  0 if no error 
*/
static int8_t  USB_AudioStreamingInputOutputGetCurFrequency(uint32_t* freq, uint32_t node_handle)
{
 *freq = ((AUDIO_USBInputOutputNode_t *)node_handle) ->node.audio_description->frequency;
 return 0;
}

/**
  * @brief  USB_AudioStreamingInputOutputSetCurFrequency
  *         set  current frequency, if frequency not supported , set nearest frequency , this callback is called by the USB Audio Class
  * @param  freq(IN): frequency to set
  * @param  usb_ep_restart_is_required:
  * @param  node_handle: the usb io node handle, node must be initialized
  * @retval  0 if no error 
*/
static int8_t  USB_AudioStreamingInputOutputSetCurFrequency(uint32_t freq, uint8_t* usb_ep_restart_is_required, uint32_t node_handle)
{
  AUDIO_USBInputOutputNode_t *usb_io_node=(AUDIO_USBInputOutputNode_t *)node_handle;
  AUDIO_Description_t* aud;
  uint32_t best_matched_freq;
 /* search nearest supported frequency */
 aud = usb_io_node->node.audio_description;
 if(aud->frequency != freq)
 {
 #ifdef USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES
 if(usb_io_node->node.type == AUDIO_INPUT)
 {
   
    best_matched_freq = USB_AudioStreamingGetNearestFrequency(freq,USB_AUDIO_CONFIG_PLAY_FREQENCIES,USB_AUDIO_CONFIG_PLAY_FREQ_COUNT);
  if(aud->frequency == best_matched_freq)
  {/* the frequency doesn't changed no need to restart end point */
    *usb_ep_restart_is_required = 0;
    return 0;
  }
  else
  {
    aud->frequency = best_matched_freq;
  }
  /* update the max packet length */
#if USE_AUDIO_PLAYBACK_USB_FEEDBACK
  usb_io_node->max_packet_length = AUDIO_MAX_PACKET_WITH_FEEDBACK_LENGTH(aud);
#else
    usb_io_node->max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(aud);
#endif /* USE_AUDIO_PLAYBACK_USB_FEEDBACK */
 }
 #endif /* USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES*/
 
#ifdef USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES
 if(usb_io_node->node.type == AUDIO_OUTPUT)
 {
   best_matched_freq = USB_AudioStreamingGetNearestFrequency(freq,
                                                         USB_AUDIO_CONFIG_RECORD_FREQENCIES,
                                                         USB_AUDIO_CONFIG_RECORD_FREQ_COUNT);
  if(aud->frequency == best_matched_freq)
  { /* just restart endpoint to receive new packets */
    *usb_ep_restart_is_required = 1;
    return 0;
  }
  else
  {
    aud->frequency = best_matched_freq;
  }
  /* update the max packet length */
#if  USE_AUDIO_RECORDING_USB_NO_REMOVE
  usb_io_node->max_packet_length = AUDIO_MAX_PACKET_WITH_FEEDBACK_LENGTH(aud);
#else /*USE_AUDIO_RECORDING_USB_NO_REMOVE */
   usb_io_node->max_packet_length = AUDIO_USB_MAX_PACKET_SIZE_FROM_AUD_DESC(aud);
#endif /*USE_AUDIO_RECORDING_USB_NO_REMOVE*/
   /* reallocate alternate buffer as packet size changed */
  free(usb_io_node->specific.output.alt_buff);
  usb_io_node->specific.output.alt_buff = (uint8_t *) malloc(usb_io_node->max_packet_length);
   if(usb_io_node->specific.output.alt_buff)
   {
     memset(usb_io_node->specific.output.alt_buff, 0, usb_io_node->max_packet_length);
   }
   else
   {
     Error_Handler();
   }
 }
#endif /* USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES*/
  usb_io_node->packet_length = AUDIO_USB_PACKET_SIZE_FROM_AUD_DESC(aud);
  usb_io_node->node.session_handle->SessionCallback(AUDIO_FREQUENCY_CHANGED,(AUDIO_Node_t*)usb_io_node,
                                                        usb_io_node->node.session_handle);
 *usb_ep_restart_is_required = 1;
 }
 else
 {
   *usb_ep_restart_is_required = 0;
#if  USE_USB_AUDIO_RECORDING
    if(usb_io_node->node.type == AUDIO_OUTPUT)
    {
        *usb_ep_restart_is_required = 1;
    }
#endif /* USE_USB_AUDIO_RECORDING */
 }
 return 0;
}
#endif /*USE_AUDIO_USB_MULTI_FREQUENCIES*/
#endif /* USE_USB_AUDIO_CLASS_10 */

#if (defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES)
/**
  * @brief  USB_AudioStreamingGetNearestFrequency
  *        look in a table of frequency the nearest value to the frequency provided parameter
  * @param  freq(IN)): frequency to approach
  * @param  freq_table(IN): table of frequencies, should be sorted
  * @param  freq_count(IN): the size of the table
  * @retval  nearest frequency value 
*/
static uint32_t  USB_AudioStreamingGetNearestFrequency(uint32_t freq,  uint32_t* freq_table,  int freq_count)
{
  
  if(freq >= freq_table[0])
 {
   return freq_table[0];
 }
 else
 {
    if(freq <= freq_table[freq_count-1])
   {
     return freq_table[freq_count-1];
   }
   else
   {
     for(int i = 1; i<freq_count; i++)
     {
       if(freq >= freq_table[i])
       {
         return ((freq_table[i-1] - freq )<= ( freq - freq_table[i]))?
           freq_table[i-1] : freq_table[i];
       }
     }
   }
 }
 
return 0; 
}
#endif /* (defined USE_AUDIO_USB_PLAY_MULTI_FREQUENCIES)||(defined USE_AUDIO_USB_RECORD_MULTI_FREQUENCIES) */

#if USE_USB_AUDIO_CLASS_10
/**
  * @brief  USB_AudioStreamingInputOutputGetState
  *         return data ep   state   
  * @param  node_handle: the input node handle, node must be initialized
  * @retval  0
*/
static int8_t  USB_AudioStreamingInputOutputGetState(uint32_t node_handle)
{
  return 0;
}
#endif /* USE_USB_AUDIO_CLASS_10 */

/**
  * @brief  USB_AudioStreamingFeatureUnitInit
  *         Initializes control feature unit node
  * @param  usb_control_feature(OUT): structure to communicate with USB Audio Class, it contains information, controls
  *                                   and callbacks to handle controls that target the feature unit
  * @param  audio_defaults(IN):             audio defaults setting
  * @param  unit_id(IN):                    usb unit id
  * @param  node_handle(IN):                the node handle, node must be allocated
  * @retval  0 for no error
  */
 int8_t USB_AudioStreamingFeatureUnitInit(USBD_AUDIO_ControlTypeDef* usb_control_feature,
                                   AUDIO_USBFeatureUnitDefaults_t* audio_defaults, uint8_t unit_id,
                                   uint32_t node_handle)
{
  AUDIO_USB_CF_NodeTypeDef * cf;
 
  cf = (AUDIO_USB_CF_NodeTypeDef*)node_handle;  
  memset(cf,0,sizeof(AUDIO_USB_CF_NodeTypeDef));
  cf->node.state = AUDIO_NODE_INITIALIZED;
  cf->node.type = AUDIO_CONTROL;
  cf->unit_id = unit_id;
  
  cf->CFInit = USB_AudioStreamingFeatureUnitInit;
  cf->CFDeInit = USB_AudioStreamingFeatureUnitDInit;
  cf->CFStart = USB_AudioStreamingFeatureUnitStart;
  cf->CFStop = USB_AudioStreamingFeatureUnitStop;
  cf->CFSetMute = USB_AudioStreamingFeatureUnitSetMute;
#if USE_USB_AUDIO_CLASS_10
  cf->usb_control_callbacks.GetStatus = USB_AudioStreamingFeatureUnitGetStatus;
#endif /*USE_USB_AUDIO_CLASS_10*/
  cf->usb_control_callbacks.GetMute = USB_AudioStreamingFeatureUnitGetMute;
  cf->usb_control_callbacks.SetMute = USB_AudioStreamingFeatureUnitSetMute;
  cf->usb_control_callbacks.GetCurVolume = USB_AudioStreamingFeatureUnitGetCurVolume;
  cf->usb_control_callbacks.SetCurVolume = USB_AudioStreamingFeatureUnitSetCurVolume;
  VOLUME_DB_256_TO_USB(cf->usb_control_callbacks.MaxVolume, audio_defaults->max_volume);
  VOLUME_DB_256_TO_USB(cf->usb_control_callbacks.MinVolume, audio_defaults->min_volume);
  cf->usb_control_callbacks.ResVolume = audio_defaults->res_volume;
  cf->node.audio_description=audio_defaults->audio_description;
  
  /* fill structure used by USB Audio Class module */
  usb_control_feature->id = unit_id;
  usb_control_feature->control_req_map = 0;  
  usb_control_feature->control_selector_map = USBD_AUDIO_FU_MUTE_CONTROL|USBD_AUDIO_FU_VOLUME_CONTROL;
  usb_control_feature->type = USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT;
  usb_control_feature->Callbacks.feature_control = &cf->usb_control_callbacks;
  usb_control_feature->private_data = node_handle;
  return 0;
}


/**
  * @brief  USB_AudioStreamingFeatureUnitDInit              
  *         De-Initialize control feature unit node
  * @param  node_handle: the node handle, node must be Initialized
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitDInit(uint32_t node_handle)
{
  ((AUDIO_USB_CF_NodeTypeDef*)node_handle)->node.state = AUDIO_NODE_OFF;
  return 0;
}

/**
  * @brief  USB_AudioStreamingFeatureUnitStart
  *         start control feature unit node. after start call the feature unit node executes controls like set volume and mute.
  *         If some command are already received and pending, they will be executed in this function
  * @param  commands(IN): list of callback to execute controls like setvolume and mute. this function depends on codec and microphone.
  * @param  node_handle(IN): the node handle, node must be allocated
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitStart(AUDIO_USBFeatureUnitCommands_t* commands, uint32_t node_handle)
{
  AUDIO_USB_CF_NodeTypeDef *cf;
  
  cf = (AUDIO_USB_CF_NodeTypeDef*)node_handle;
  cf->control_cbks = *commands;
  cf->node.state = AUDIO_NODE_STARTED;
  
  if(cf->control_cbks.SetCurrentVolume)
  {
    cf->control_cbks.SetCurrentVolume(0, 
                                      cf->node.audio_description->audio_volume_db_256,
                                      cf->control_cbks.private_data);
  }
  return 0;
}

/**
  * @brief  USB_AudioStreamingFeatureUnitStop
  *         stop control feature node
  * @param  node_handle: the node handle, node must be started
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitStop( uint32_t node_handle)
{
  /* @TODO develop feature */
  AUDIO_USB_CF_NodeTypeDef * cf;
  
  cf = (AUDIO_USB_CF_NodeTypeDef*)node_handle;
  cf->node.state = AUDIO_NODE_STOPPED;
  return 0;
}

/**
  * @brief  USB_AudioStreamingFeatureUnitGetMute
  *         get mute value
  * @param  channel: channel number , 0 for master channel (only this option is supported now)
  * @param  mute: returned mute value
  * @param  node_handle: the Feature node handle, node must be initialized
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitGetMute(uint16_t channel, uint8_t* mute, uint32_t node_handle)
{
  /**@TODO add channel management  */
  *mute = ((AUDIO_USB_CF_NodeTypeDef*)node_handle)->node.audio_description->audio_mute; 
  return 0; 
}

/**
  * @brief  USB_AudioStreamingFeatureUnitSetMute
  *         set mute value
  * @param  channel: channel number , 0 for master channel(only this option is supported now)
  * @param  mute:  mute value
  * @param  node_handle: the Feature node handle, node must be initialized
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitSetMute(uint16_t channel, uint8_t mute, uint32_t node_handle)
{
  AUDIO_USB_CF_NodeTypeDef * cf;
  
  cf = (AUDIO_USB_CF_NodeTypeDef*)node_handle;
  /**@TODO add channel management  */
  
  cf->node.audio_description->audio_mute = mute;
  if((cf->node.state == AUDIO_NODE_STARTED)&&(cf->control_cbks.SetMute))
  {
      cf->control_cbks.SetMute(channel, mute, cf->control_cbks.private_data);
  }
  return 0;
}

/**
  * @brief  USB_AudioStreamingFeatureUnitGetCurVolume
  *         get current volume  value
  * @param  channel:            channel number , 0 for master channel(only this option is supported now)
  * @param  volume:             returned volume value
  * @param  node_handle:        the Feature node handle, node must be initialized
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitGetCurVolume(uint16_t channel, uint16_t* volume, uint32_t node_handle)
{
  /**@TODO add channel management  */
  VOLUME_DB_256_TO_USB(*volume, ((AUDIO_Node_t*)node_handle)->audio_description->audio_volume_db_256);
  return 0; 
}

/**
  * @brief  USB_AudioStreamingFeatureUnitSetCurVolume
  *         set current volume  value
  * @param  channel:            channel number , 0 for master channel(only this option is supported now)
  * @param  volume:             volume value
  * @param  node_handle:        the Feature node handle, node must be initialized
  * @retval  0 for no error
  */
static int8_t USB_AudioStreamingFeatureUnitSetCurVolume(uint16_t channel, uint16_t volume, uint32_t node_handle)
{
  AUDIO_USB_CF_NodeTypeDef* cf;
  
  cf = (AUDIO_USB_CF_NodeTypeDef*)node_handle;
  /**@TODO add channel management  */
  
  VOLUME_USB_TO_DB_256(cf->node.audio_description->audio_volume_db_256, volume);
  if((cf->node.state == AUDIO_NODE_STARTED)&&(cf->control_cbks.SetCurrentVolume))
  {
    cf->control_cbks.SetCurrentVolume(channel, 
                                      cf->node.audio_description->audio_volume_db_256,
                                      cf->control_cbks.private_data);
  }
  return 0;
}

#if USE_USB_AUDIO_CLASS_10
/**
  * @brief  USB_AudioStreamingFeatureUnitGetStatus          
  *         get Feature unit status
  * @param  node_handle:        the Feature node handle, node must be initialized      
  * @retval 0 for no error
  */
static int8_t  USB_AudioStreamingFeatureUnitGetStatus( uint32_t node_handle )
{
  return 0;
}
#endif /* USE_USB_AUDIO_CLASS_10 */



/**
  * @brief  USB_AudioStreamingInitializeDataBuffer
  *         The circular buffer has the total size of buffer_size. this size is divided to two : the regular size and the margin.
  *         Margin is located at the tail of the circular buffer. Margin is used as some packet have regular size+/-1 sample. 
  * @param  buf:  main circular buffer               
  * @param  buffer_size: whole buffer size when allocated                
  * @param  packet_size:USB Audio packet size 
  * @param  margin: protection area size
  * @retval 0 if no error
  */
  void USB_AudioStreamingInitializeDataBuffer(AUDIO_CircularBuffer_t* buf, 
                                       uint32_t buffer_size, 
                                       uint16_t packet_size, uint16_t margin)
 {
    buf->size = ((int)((buffer_size - margin )
                       / packet_size)) * packet_size; 
    buf->rd_ptr = buf->wr_ptr = 0;
 }
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
