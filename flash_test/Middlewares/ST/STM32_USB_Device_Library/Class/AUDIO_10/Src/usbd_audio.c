/**
  ******************************************************************************
  * @file    usbd_audio.c
  * @author  MCD Application Team 
  * @brief   This file provides the Audio core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                AUDIO Class  Description
  *          ===================================================================
 *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           It is new implementation of USB audio class which supports more features.
  *           This driver implements the following aspects of the specification:
  *             - Standard AC Interface Descriptor management
  *             - 2 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *           
  *
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
#include "usbd_ctlreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_AUDIO 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_AUDIO_Private_TypesDefinitions
  * @{
  */ 
   typedef enum
{
  USBD_AUDIO_DATA_EP,
  USBD_AUDIO_FEEDBACK_EP,
  USBD_AUDIO_INTERRUPT_EP
}USBD_AUDIO_EpUsageTypeDef;
    /* Structure define ep:  description and state */
    typedef struct
{
  union
  {
    USBD_AUDIO_EP_DataTypeDef* data_ep;
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK  
    USBD_AUDIO_EP_SynchTypeDef* sync_ep;
#endif /* USBD_SUPPORT_AUDIO_OUT_FEEDBACK */ 
  }ep_description;
  USBD_AUDIO_EpUsageTypeDef ep_type;
  uint8_t open; /* 0 closed , 1 open */
  uint16_t max_packet_length; /* the max packet length */
  uint16_t tx_rx_soffn;
}USBD_AUDIO_EPTypeDef;

/* Structure define audio class data */
typedef struct 
{
  USBD_AUDIO_FunctionDescriptionfTypeDef aud_function; /* description of audio function */
  USBD_AUDIO_EPTypeDef ep_in[USBD_AUDIO_MAX_IN_EP]; /*  list of IN EP */
  USBD_AUDIO_EPTypeDef ep_out[USBD_AUDIO_MAX_OUT_EP]; /*  list of OUT EP */ 

  /* Strcture used for control handeling */
  struct
  {
    union
    {
      USBD_AUDIO_ControlTypeDef *controller; /* related Control Unit */
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
      USBD_AUDIO_EP_DataTypeDef* data_ep; /* related Data End point */
#endif /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES */
    } entity;
    uint8_t request_target;
    uint8_t data[USB_MAX_EP0_SIZE];  /* buffer to receive request value or send response */
    uint32_t len; /* used length of data buffer */
    uint16_t  wValue;/* wValue of request which is specific for each control*/
    uint8_t  req;/* the request type specific for each unit*/
  }last_control;
}USBD_AUDIO_HandleTypeDef;

/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Private_Defines
  * @{
  */
#define AUDIO_UNIT_CONTROL_REQUEST 0x01
#define AUDIO_EP_REQUEST 0x02
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK 
#define USBD_AUDIO_SOF_COUNT_FEEDBACK_BITS 7
#define USBD_AUDIO_SOF_COUNT_FEEDBACK (1 << USBD_AUDIO_SOF_COUNT_FEEDBACK_BITS)
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Macros
  * @{
  */ 

                                         
/**
  * @}
  */ 




/** @defgroup USBD_AUDIO_Private_FunctionPrototypes
  * @{
  */

static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t AUDIO_REQ(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
static uint8_t AUDIO_EP_REQ(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
#endif /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES*/
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK
static  unsigned get_usb_full_speed_rate(unsigned int rate, unsigned char * buf);
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */

static uint8_t  USBD_AUDIO_SetInterfaceAlternate(USBD_HandleTypeDef *pdev,uint8_t as_interface_num,uint8_t new_alt);

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef  USBD_AUDIO = 
{
  USBD_AUDIO_Init,
  USBD_AUDIO_DeInit,
  USBD_AUDIO_Setup,
  USBD_AUDIO_EP0_TxReady,  
  USBD_AUDIO_EP0_RxReady,
  USBD_AUDIO_DataIn,
  USBD_AUDIO_DataOut,
  USBD_AUDIO_SOF,
  USBD_AUDIO_IsoINIncomplete,
  USBD_AUDIO_IsoOutIncomplete,      
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc, 
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetDeviceQualifierDesc,
};
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END=
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

static uint8_t *USBD_AUDIO_CfgDesc=0;
static uint16_t USBD_AUDIO_CfgDescSize=0;
/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_AUDIO_Init
  *         Initialize the AUDIO interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index , not used
  * @retval status
  */
static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx)
{
  /* Allocate Audio structure */
  USBD_AUDIO_HandleTypeDef   *haudio;
  USBD_AUDIO_InterfaceCallbacksfTypeDef * aud_if_cbks;
  
  haudio = USBD_malloc(sizeof (USBD_AUDIO_HandleTypeDef));
  if(haudio == NULL)
  {
    return USBD_FAIL; 
  }
  else
  {
    memset(haudio, 0, sizeof(USBD_AUDIO_HandleTypeDef));
    aud_if_cbks = (USBD_AUDIO_InterfaceCallbacksfTypeDef *)pdev->pUserData;
    /* Initialize the Audio output Hardware layer */
    if (aud_if_cbks->Init(&haudio->aud_function,aud_if_cbks->private_data)!= USBD_OK)
    {
      USBD_free(pdev->pClassData);
      pdev->pClassData = 0;
      return USBD_FAIL;
    }
  }
  pdev->pClassData = haudio;
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Init
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index, not used 
  * @retval status
  */
static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{
    USBD_AUDIO_HandleTypeDef   *haudio;
    USBD_AUDIO_InterfaceCallbacksfTypeDef * aud_if_cbks;
    
    haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
    aud_if_cbks =  (USBD_AUDIO_InterfaceCallbacksfTypeDef *)pdev->pUserData;
    
    /* Close open EP */
    for(int i=1;i < USBD_AUDIO_MAX_IN_EP; i++)
    {
      if(haudio->ep_in[i].open)
      {
        USBD_LL_CloseEP(pdev, i|0x80);
        haudio->ep_in[i].open = 0;
      }
    }
    for(int i=1;i < USBD_AUDIO_MAX_OUT_EP; i++)
    {
      if(haudio->ep_out[i].open)
      {
        USBD_LL_CloseEP(pdev, i);
        haudio->ep_out[i].open = 0;
      }
    }
  /* DeInit  physical Interface components */
  if(haudio != NULL)
  {
   aud_if_cbks->DeInit(&haudio->aud_function,aud_if_cbks->private_data);
    USBD_free(haudio);
    pdev->pClassData = NULL;
  }
  
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SetInterfaceAlternate
  *         Set the Alternate interface of a streaming interface
  * @param  pdev: device instance
  * @param  as_interface_num: audio streaming interface number
  * @param  new_alt: new alternate number
  * @retval status
  */
static uint8_t  USBD_AUDIO_SetInterfaceAlternate(USBD_HandleTypeDef *pdev,uint8_t as_interface_num,uint8_t new_alt)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  USBD_AUDIO_AS_InterfaceTypeDef* pas_interface;
  USBD_AUDIO_EPTypeDef * ep;
  
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  pas_interface = &haudio->aud_function.as_interfaces[as_interface_num];
  ep = (pas_interface->data_ep.ep_num&0x80)?&haudio->ep_in[pas_interface->data_ep.ep_num&0x0F]:
                                            &haudio->ep_out[pas_interface->data_ep.ep_num];
  
  
  /* close old alternate interface */
  if(new_alt==0)
  {
    /* close all opned ep */
    if (pas_interface->alternate!=0)
    {
        /* @TODO : Close related End Points */
      if(ep->open)
      {
        USBD_LL_CloseEP(pdev, ep->ep_description.data_ep->ep_num);
        ep->open=0;
      }
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK  
      if(pas_interface->synch_enabled)
      {
        /* close synch ep */
          ep=&haudio->ep_in[pas_interface->synch_ep.ep_num&0x0F];
          if(ep->open)
          {
            USBD_LL_CloseEP(pdev, ep->ep_description.sync_ep->ep_num);
            ep->open = 0;
          }
      }
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
    }
    pas_interface->SetAS_Alternate(new_alt,pas_interface->private_data);
    pas_interface->alternate=0;
  }
  /* start new  alternate interface */
  else
  {
    /* prepare EP */
    ep->ep_description.data_ep=&pas_interface->data_ep;
    
    /* open the data ep */
    pas_interface->SetAS_Alternate(new_alt,pas_interface->private_data);
    pas_interface->alternate=new_alt;
    ep->max_packet_length=ep->ep_description.data_ep->GetMaxPacketLength(ep->ep_description.data_ep->private_data);
    /* open data end point */
    USBD_LL_OpenEP(pdev,
                 ep->ep_description.data_ep->ep_num,
                 USBD_EP_TYPE_ISOC,
                 ep->max_packet_length);             
     ep->open = 1;
     
     /* get usb working buffer */ 
    ep->ep_description.data_ep->buf= ep->ep_description.data_ep->GetBuffer(ep->ep_description.data_ep->private_data,
                                                                           &ep->ep_description.data_ep->length);        
    
    if(ep->ep_description.data_ep->ep_num&0x80)  /* IN EP */
    {
      USBD_LL_FlushEP(pdev, ep->ep_description.data_ep->ep_num);
      ep->tx_rx_soffn = USB_SOF_NUMBER();
      USBD_LL_Transmit(pdev, 
                        ep->ep_description.data_ep->ep_num,
                        ep->ep_description.data_ep->buf,
                        ep->ep_description.data_ep->length);
    }
    else/* OUT EP */
    {
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK 
        uint32_t rate;
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */  
    /* Prepare Out endpoint to receive 1st packet */ 
    USBD_LL_PrepareReceive(pdev,
                           ep->ep_description.data_ep->ep_num,
                           ep->ep_description.data_ep->buf,                        
                           ep->max_packet_length); 
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK 
    if(pas_interface->synch_enabled)
      {
           USBD_AUDIO_EP_SynchTypeDef* sync_ep; /* synchro ep description */
           ep = &haudio->ep_in[pas_interface->synch_ep.ep_num&0x0F];
           sync_ep = &pas_interface->synch_ep;
           ep->ep_description.sync_ep = sync_ep;
           ep->max_packet_length = AUDIO_FEEDBACK_EP_PACKET_SIZE;
           ep->ep_type = USBD_AUDIO_FEEDBACK_EP;
           /* open synchro ep */
           USBD_LL_OpenEP(pdev, sync_ep->ep_num,
                 USBD_EP_TYPE_ISOC, ep->max_packet_length);             
            ep->open = 1;
            rate = sync_ep->GetFeedback(sync_ep->private_data);
            get_usb_full_speed_rate(rate,sync_ep->feedback_data);
            ep->tx_rx_soffn = USB_SOF_NUMBER();
            USBD_LL_Transmit(pdev, sync_ep->ep_num,
                             sync_ep->feedback_data, ep->max_packet_length);
      }
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */   
    }
  }
  return USBD_OK;
}
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK
/**
  * @brief   get_usb_full_speed_rate
  *         Set feedback value from rate 
  * @param  rate: 
  * @param  buf: 
  * @retval 
  */
static  unsigned get_usb_full_speed_rate(unsigned int rate, unsigned char * buf)
{
        uint32_t freq =  ((rate << 13) + 62) / 125;
        buf[0] =    freq>> 2;
        buf[1] =    freq>> 10;
        buf[2] =    freq>> 18;
return 0;
 }
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */ 

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint8_t ret = USBD_OK;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    if((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE)
    {
      switch (req->bRequest)
      {
      case USBD_AUDIO_REQ_GET_CUR:
      case USBD_AUDIO_REQ_GET_MIN:
      case USBD_AUDIO_REQ_GET_MAX:
      case USBD_AUDIO_REQ_GET_RES:
      case USBD_AUDIO_REQ_SET_CUR:
           AUDIO_REQ(pdev, req);
        break;
        
      default:
        USBD_CtlError (pdev, req);
        ret = USBD_FAIL; 
      }
    }
    else
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
    {

      switch (req->bRequest)
      {
      case USBD_AUDIO_REQ_SET_CUR:
           AUDIO_EP_REQ(pdev, req);
        break;
        
      default:
        USBD_CtlError (pdev, req);
        ret = USBD_FAIL; 
      }
    }
#else /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES*/
    {
     USBD_CtlError (pdev, req);
        ret = USBD_FAIL;
    }
#endif /*USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES*/
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:      
      if( (req->wValue >> 8) == USBD_AUDIO_DESC_TYPE_CS_DEVICE)
      {
        pbuf = USBD_AUDIO_CfgDesc + 18;
        len = MIN(USBD_AUDIO_DESC_SIZ , req->wLength);
        
        
        USBD_CtlSendData (pdev, 
                          pbuf,
                          len);
      }
      break;
      
    case USB_REQ_GET_INTERFACE :
      {
        for(int i=0;i<haudio->aud_function.as_interfaces_count;i++)
        {
            if((uint8_t)(req->wIndex)==haudio->aud_function.as_interfaces[i].interface_num)
            {
              USBD_CtlSendData (pdev,
                        (uint8_t *)&(haudio->aud_function.as_interfaces[i].alternate),
                        1);
              return USBD_OK;
            }
        }
        USBD_CtlError (pdev, req);
        ret = USBD_FAIL; 
      }
      break;
      
    case USB_REQ_SET_INTERFACE :
      {
        for(int i=0;i<haudio->aud_function.as_interfaces_count;i++)
        {
            if((uint8_t)(req->wIndex)==haudio->aud_function.as_interfaces[i].interface_num)
            {
              if((uint8_t)(req->wValue)==haudio->aud_function.as_interfaces[i].alternate)
              {
                /* Nothing to do*/
                return USBD_OK;
              }
              else
              {               
                /*Alternate is changed*/
                return USBD_AUDIO_SetInterfaceAlternate(pdev,i,(uint8_t)(req->wValue));
              }
            }
        } 

        
        if(((uint8_t)(req->wIndex) ==0)&&((uint8_t)(req->wValue))==0)
        {
          /* Audio Control Control interface, only alternate zero is accepted  */     
                return USBD_OK;
        }
          /* Call the error management function (command will be nacked */
          USBD_CtlError (pdev, req);
          ret = USBD_FAIL; 
      } 
      break;      
      
    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL;     
    }
  }
  return ret;
}


/**
  * @brief  USBD_AUDIO_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length)
{
  *length = USBD_AUDIO_CfgDescSize;
  return USBD_AUDIO_CfgDesc;
}

/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
  USBD_AUDIO_EPTypeDef * ep;

   ep = &((USBD_AUDIO_HandleTypeDef*) pdev->pClassData)->ep_in[epnum&0x7F];
   if(ep->open)
   {
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK    
      if(ep->ep_type==USBD_AUDIO_DATA_EP)
      {
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
     
          ep->ep_description.data_ep->buf = ep->ep_description.data_ep->GetBuffer(ep->ep_description.data_ep->private_data,
                                                                                  &ep->ep_description.data_ep->length);
          ep->tx_rx_soffn = USB_SOF_NUMBER();
          USBD_LL_Transmit(pdev, 
                      epnum|0x80,
                      ep->ep_description.data_ep->buf,
                      ep->ep_description.data_ep->length);     
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK 
     }
     else
     if(ep->ep_type==USBD_AUDIO_FEEDBACK_EP)
     {
       
       uint32_t rate; 
       USBD_AUDIO_EP_SynchTypeDef* sync_ep=ep->ep_description.sync_ep;
       rate = sync_ep->GetFeedback(sync_ep->private_data);
       get_usb_full_speed_rate(rate,sync_ep->feedback_data);
       ep->tx_rx_soffn = USB_SOF_NUMBER();
       USBD_LL_Transmit(pdev, 
            epnum|0x80,
            sync_ep->feedback_data,
            AUDIO_FEEDBACK_EP_PACKET_SIZE);
     }
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */

    }
   else
   {
     /* Should not be reproduced */
     USBD_error_handler();
   }
  
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  uint16_t *tmpdata;
  
 
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData; 
  if(haudio->last_control.req == 0x00)
  {
    /* @TODO Manage this error */
    return USBD_OK;
  }
  if(haudio->last_control.request_target == AUDIO_UNIT_CONTROL_REQUEST)
  {
    USBD_AUDIO_ControlTypeDef *ctl;
    ctl=haudio->last_control.entity.controller;
    switch(ctl->type)
    {
    case USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT:
         {
            uint16_t selector = HIBYTE(haudio->last_control.wValue);
            USBD_AUDIO_FeatureControlCallbacksTypeDef* feature_control = ctl->Callbacks.feature_control;
          switch(selector)
          {
                  case USBD_AUDIO_CONTROL_FEATURE_UNIT_MUTE:
                    {
                      /* @TODO treat multi channel case and error when req! of GetCur*/  
                      if(feature_control->SetMute)
                      {
                        feature_control->SetMute(LOBYTE(haudio->last_control.wValue),
                                                                haudio->last_control.data[0], ctl->private_data);
                      }
                      break;
                    }
                  case USBD_AUDIO_CONTROL_FEATURE_UNIT_VOLUME:
                     {
                   
                        /* @TODO check the len uses cases and control req->wLength*/

                       switch(haudio->last_control.req)
                        {
                        case USBD_AUDIO_REQ_SET_CUR:
                              if(feature_control->SetCurVolume)
                              {
                            	  tmpdata = (uint16_t*) &(haudio->last_control.data);
                                  feature_control->SetCurVolume(LOBYTE(haudio->last_control.wValue),
                                                                               *tmpdata,
                                                                               ctl->private_data);
                              }
                              break;
                        default :
                              
                                USBD_error_handler();
                        }
                         break;
                       }
                  
                default :
                 
                          USBD_error_handler();
                }
          break;
         }
                 
  default : /* switch(ctl->type)*/
            USBD_error_handler();
                             
    }
  }
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
  else
  {
    USBD_AUDIO_EP_DataTypeDef* data_ep = haudio->last_control.entity.data_ep;
    uint16_t selector = HIBYTE(haudio->last_control.wValue);
    if(selector == USBD_AUDIO_CONTROL_EP_SAMPL_FREQ)
    {
      /* @TODO check the len uses cases and control req->wLength*/
        switch(haudio->last_control.req)
        {
          case USBD_AUDIO_REQ_SET_CUR:
            if(data_ep->control_cbk.SetCurFrequency)
            {
              uint8_t restart_interface = 0;
                data_ep->control_cbk.SetCurFrequency(AUDIO_FREQ_FROM_DATA(haudio->last_control.data),
                                                     &restart_interface, data_ep->private_data);
        for(int i=0; i<haudio->aud_function.as_interfaces_count; i++)
        {
            if(data_ep == &haudio->aud_function.as_interfaces[i].data_ep)
            {
 /* update sampling rate for syenchronization EP */
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK
              if(haudio->aud_function.as_interfaces[i].synch_enabled)
              {
                uint32_t rate; 
                rate = haudio->aud_function.as_interfaces[i].synch_ep.GetFeedback(
                                 haudio->aud_function.as_interfaces[i].synch_ep.private_data);
                get_usb_full_speed_rate(rate,haudio->aud_function.as_interfaces[i].synch_ep.feedback_data);
              }
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
              if(restart_interface)
              {
                if(haudio->aud_function.as_interfaces[i].alternate != 0)
                {
                  int alt = haudio->aud_function.as_interfaces[i].alternate;
                  USBD_AUDIO_SetInterfaceAlternate(pdev, i, 0);
                  USBD_AUDIO_SetInterfaceAlternate(pdev, i, alt);
                }
              }
              break;
            }
        }

            }
            break;
          default :
            
              USBD_error_handler();
         }
     }
    else
    {
       USBD_error_handler();
    }
  }
#endif /* USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES */
  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev)
{
  /* Only OUT control data are processed */
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev)
{
    USBD_AUDIO_HandleTypeDef   *haudio;
  
 
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData; 
  
  for(int i=0;i<haudio->aud_function.as_interfaces_count;i++)
  {
      if(haudio->aud_function.as_interfaces[i].alternate!=0)
      {
        if(haudio->aud_function.as_interfaces[i].SofReceived)
          
        {
          haudio->aud_function.as_interfaces[i].SofReceived(haudio->aud_function.as_interfaces[i].private_data);
        }
      }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
 USBD_AUDIO_EPTypeDef   *ep;
 USBD_AUDIO_HandleTypeDef   *haudio;
 uint16_t current_sof;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
 /* @TODO check if the feedback is responsible of event */
  for(int i = 1; i<USBD_AUDIO_MAX_IN_EP; i++)
  {
    ep = &haudio->ep_in[i];
    current_sof = USB_SOF_NUMBER();
    if((ep->open) && IS_ISO_IN_INCOMPLETE_EP(i,current_sof, ep->tx_rx_soffn))
    {
      epnum = i|0x80;
      USB_CLEAR_INCOMPLETE_IN_EP(epnum);
      USBD_LL_FlushEP(pdev, epnum);
      ep->tx_rx_soffn = USB_SOF_NUMBER();
#if USBD_SUPPORT_AUDIO_OUT_FEEDBACK  
     if(ep->ep_type==USBD_AUDIO_FEEDBACK_EP)
      {
        USBD_LL_Transmit(pdev, 
                         epnum,
                         ep->ep_description.sync_ep->feedback_data,
                         ep->max_packet_length);
        continue;
      }
     else
#endif /*USBD_SUPPORT_AUDIO_OUT_FEEDBACK */
     if(ep->ep_type==USBD_AUDIO_DATA_EP)
      {
        USBD_LL_Transmit(pdev, 
                      epnum,
                      ep->ep_description.data_ep->buf,
                      ep->ep_description.data_ep->length);
      }
     else
     {
       USBD_error_handler();
     }
    
    }
  }
  return 0;
}
/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */

static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
  
  USBD_AUDIO_EPTypeDef * ep;
  uint8_t *pbuf ;
  uint16_t packet_length;


  ep=&((USBD_AUDIO_HandleTypeDef*) pdev->pClassData)->ep_out[epnum];

  if(ep->open)
  {
    /* get received length */
    packet_length = USBD_LL_GetRxDataSize(pdev, epnum);
    /* inform user about data reception  */
    ep->ep_description.data_ep->DataReceived(packet_length,ep->ep_description.data_ep->private_data);
     
    /* get buffer to receive new packet */  
    pbuf=  ep->ep_description.data_ep->GetBuffer(ep->ep_description.data_ep->private_data,&packet_length);                               
    /* Prepare Out endpoint to receive next audio packet */
     USBD_LL_PrepareReceive(pdev,
                            epnum,
                            pbuf,
                            packet_length);
    }
    else
    {
      USBD_error_handler();
    }
    
  
    return USBD_OK;
}

/**
  * @brief  AUDIO_REQ
  *         Handles the Control requests.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static uint8_t AUDIO_REQ(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{  
  USBD_AUDIO_HandleTypeDef   *haudio;
  USBD_AUDIO_ControlTypeDef * ctl = 0;
  uint8_t unit_id,control_selector;
  uint16_t *tmpdata = NULL;
 
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  /* reset last command */
  haudio->last_control.req = 0x00;
  
  /* get the Unit Id */
  unit_id = HIBYTE(req->wIndex);
  
  for (int i = 0;i < haudio->aud_function.control_count; i++)
  {
    if(unit_id == haudio->aud_function.controls[i].id)
    {
      ctl = &haudio->aud_function.controls[i];
      break;
    }     
  }
  
  if(!ctl)
  {
    /* control not supported */
    USBD_CtlError (pdev, req);
    return  USBD_FAIL; 
  }
  
  control_selector = HIBYTE(req->wValue);
  
  if((ctl->control_selector_map & control_selector) == 0)
  {
    /* control not supported */
    USBD_CtlError (pdev, req);
      return  USBD_FAIL; 
  }
  
  if(!(req->bRequest&0x80))
  {
    /* set request */
    /* @TODO check the length */
     haudio->last_control.wValue  = req->wValue;
     haudio->last_control.entity.controller= ctl;
     haudio->last_control.request_target = AUDIO_UNIT_CONTROL_REQUEST;
     haudio->last_control.len = req->wLength;
     haudio->last_control.req = req->bRequest;
     USBD_CtlPrepareRx (pdev,
                        haudio->last_control.data,                                  
                       req->wLength);
      return USBD_OK;   
  }
  
  switch(ctl->type)
  {
    case USBD_AUDIO_CS_AC_SUBTYPE_FEATURE_UNIT:
         {
           USBD_AUDIO_FeatureControlCallbacksTypeDef* feature_control = ctl->Callbacks.feature_control;
          switch(control_selector)
          {
                  case USBD_AUDIO_CONTROL_FEATURE_UNIT_MUTE:
                    {
                      /* @TODO treat multi channel case and error when req! of GetCur*/
                      
                      haudio->last_control.data[0] = 0;
                      if(feature_control->GetMute)
                      {
                        feature_control->GetMute(LOBYTE(req->wValue),
                                                                &haudio->last_control.data[0], ctl->private_data);
                      }
                      /* Send the current mute state */
                      USBD_CtlSendData (pdev, haudio->last_control.data,1);

                      break;
                     }
                  case USBD_AUDIO_CONTROL_FEATURE_UNIT_VOLUME:
                     {
                   
                        /* set request */
                        /* @TODO check the len uses cases and control req->wLength*/
                       
                       tmpdata =  (uint16_t*) &(haudio->last_control.data);
                        switch(req->bRequest)
                        {
                        case USBD_AUDIO_REQ_GET_CUR:
                              tmpdata = 0;
                              if(feature_control->GetCurVolume)
                              {
                                  feature_control->GetCurVolume(LOBYTE(req->wValue),
                                                                (uint16_t*)haudio->last_control.data, ctl->private_data);
                              }
                              break;
                          
                        case USBD_AUDIO_REQ_GET_MIN:
                              tmpdata = (uint16_t*) &(feature_control->MinVolume);
                              break;
                        case USBD_AUDIO_REQ_GET_MAX:
                             tmpdata = (uint16_t*) &(feature_control->MaxVolume);
                              break;
                         
                       case USBD_AUDIO_REQ_GET_RES:
                              tmpdata = (uint16_t*) &(feature_control->ResVolume);
                              break;
                        default :
                                USBD_error_handler();
                        }
                         /* Send the current mute state */
                                USBD_CtlSendData (pdev, (uint8_t*) tmpdata,2);
                         break;
                       }
                  
                default :
                          USBD_error_handler();
                }
          break;
         }
                 
  default : /* switch(ctl->type)*/
            USBD_error_handler();
    }
  return USBD_OK;
}
#if USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES
/**
  * @brief  AUDIO_EP_REQ
  *         Handles the EP requests.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static uint8_t AUDIO_EP_REQ(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  USBD_AUDIO_EP_DataTypeDef* data_ep = 0;
  uint8_t ep_num, control_selector;

  /* get the main structure handle */
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  /* get the EP number */
  ep_num = LOBYTE(req->wIndex);
  
  /* look for registered data EP */
  for (int i = 0;i < haudio->aud_function.as_interfaces_count; i++)
  {
    if(ep_num == haudio->aud_function.as_interfaces[i].data_ep.ep_num)
    {
        data_ep = &haudio->aud_function.as_interfaces[i].data_ep;
        break;
    }     
  }
  
  if(!data_ep)
  {
    /* The EP not found */
    USBD_CtlError (pdev, req);
    return  USBD_FAIL; 
  }
  
  /* get the CS field*/
  control_selector = HIBYTE(req->wValue);
  
  /* check if control is supported */
  if((data_ep->control_selector_map & control_selector) == 0)
  {
    /* control not supported */
    USBD_CtlError (pdev, req);
    return  USBD_FAIL; 
  }
  
  /* for set request we prepare EP for data Stage */
  if(!(req->bRequest&0x80))
  {
    /* set request */
    /* @TODO check the length */
     haudio->last_control.wValue  = req->wValue;
     haudio->last_control.entity.data_ep = data_ep;
     haudio->last_control.request_target = AUDIO_EP_REQUEST;
     haudio->last_control.len = req->wLength;
     haudio->last_control.req = req->bRequest;
     USBD_CtlPrepareRx (pdev,
                        haudio->last_control.data,                                  
                       req->wLength);
      return USBD_OK;   
  }
  
  /* current implementation supports only FREQUENCY control , to support other ones change next code */
  if(control_selector == USBD_AUDIO_CONTROL_EP_SAMPL_FREQ)
  {
    switch(req->bRequest)
    {
      case USBD_AUDIO_REQ_GET_CUR:
      {
        uint32_t freq=0;
        if(data_ep->control_cbk.GetCurFrequency)
        {
            data_ep->control_cbk.GetCurFrequency(&freq, data_ep->private_data);
        }
        AUDIO_FREQ_TO_DATA(freq , haudio->last_control.data)
        break;
      }
      case USBD_AUDIO_REQ_GET_MIN:
            AUDIO_FREQ_TO_DATA(data_ep->control_cbk.MinFrequency , haudio->last_control.data)
			break;
      case USBD_AUDIO_REQ_GET_MAX:
            AUDIO_FREQ_TO_DATA(data_ep->control_cbk.MaxFrequency , haudio->last_control.data)
          break;
          
      /* case USBD_AUDIO_REQ_GET_RES:*/
      default :
             USBD_CtlError (pdev, req);
              return  USBD_FAIL; 
      }
  }
  else
  {
    USBD_error_handler();
  }
               /* Send the current mute state */
   USBD_CtlSendData (pdev, haudio->last_control.data,3);
  return 0;
}
#endif /*USBD_SUPPORT_AUDIO_MULTI_FREQUENCIES*/
/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_AUDIO_DeviceQualifierDesc);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
* @brief  USBD_AUDIO_RegisterInterface
* @param  fops: Audio interface callback
* @retval status
*/
uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                        USBD_AUDIO_InterfaceCallbacksfTypeDef *aifc)
{
  if(aifc != NULL)
  {
    pdev->pUserData= aifc;
    aifc->GetConfigDesc(&USBD_AUDIO_CfgDesc, &USBD_AUDIO_CfgDescSize, aifc->private_data);
    
  }
  return 0;
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
