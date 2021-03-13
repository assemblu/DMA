/*
 * Author: Emirhan Gocturk
 * Date: 14 March 2021
 * Description: Bugfix that STM does not deliver 
 * for well over a year by now. When F4 MCUs are
 * in full-duplex i2s mode, txrxcplt isn't fired.
 * The below function is a fix to this bug. 
 * 
 * Enjoy.
 */


/**
  *   DMA I2S transmit receive process complete callback
  *   hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  *  None
  */
static void I2SEx_TxRxDMACplt(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* if DMA is configured in DMA_NORMAL mode */
  if (hdma->Init.Mode == DMA_NORMAL)
  {
    if (hi2s->hdmarx == hdma)
    {
      /* Disable Rx DMA Request */
      if (((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_TX) || \
          ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_SLAVE_TX))
      {
        CLEAR_BIT(I2SxEXT(hi2s->Instance)->CR2, SPI_CR2_RXDMAEN);
      }
      else
      {
        CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_RXDMAEN);
      }

      hi2s->RxXferCount = 0U;

      if (hi2s->TxXferCount == 0U)
      {
        hi2s->State = HAL_I2S_STATE_READY;

//        /* Call user TxRx complete callback */
//#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
//        hi2s->TxRxCpltCallback(hi2s);
//#else
//        HAL_I2SEx_TxRxCpltCallback(hi2s);
//#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
      }
    }

    if (hi2s->hdmatx == hdma)
    {
      /* Disable Tx DMA Request */
      if (((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_TX) || \
          ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_SLAVE_TX))
      {
        CLEAR_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);
      }
      else
      {
        CLEAR_BIT(I2SxEXT(hi2s->Instance)->CR2, SPI_CR2_TXDMAEN);
      }

      hi2s->TxXferCount = 0U;

      if (hi2s->RxXferCount == 0U)
      {
        hi2s->State = HAL_I2S_STATE_READY;

//        /* Call user TxRx complete callback */
//#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
//        hi2s->TxRxCpltCallback(hi2s);
//#else
//        HAL_I2SEx_TxRxCpltCallback(hi2s);
//#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
      }
    }
  }
  /* Call user TxRx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
        hi2s->TxRxCpltCallback(hi2s);
#else
        HAL_I2SEx_TxRxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}