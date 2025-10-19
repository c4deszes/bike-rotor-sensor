
#define SERCOM0_USART_READ_BUFFER_SIZE      128
#define SERCOM0_USART_RX_INT_DISABLE()      SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_RXC_Msk
#define SERCOM0_USART_RX_INT_ENABLE()       SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk

void static SERCOM0_USART_ErrorClear( void )
{
    uint8_t  u8dummyData = 0;

    /* Clear error flag */
    SERCOM0_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_ERROR_Msk;

    /* Clear all errors */
    SERCOM0_REGS->USART_INT.SERCOM_STATUS = SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk  | SERCOM_USART_INT_STATUS_ISF_Msk ;

    /* Flush existing error bytes from the RX FIFO */
    while((SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        u8dummyData = SERCOM0_REGS->USART_INT.SERCOM_DATA;
    }

    /* Ignore the warning */
    (void)u8dummyData;
}

USART_ERROR SERCOM0_USART_ErrorGet( void )
{
    USART_ERROR errorStatus = USART_ERROR_NONE;

    errorStatus = (USART_ERROR) (SERCOM0_REGS->USART_INT.SERCOM_STATUS & (SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk  | SERCOM_USART_INT_STATUS_ISF_Msk ));

    if(errorStatus != USART_ERROR_NONE)
    {
        SERCOM0_USART_ErrorClear();
    }

    return errorStatus;
}


/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */
static inline bool SERCOM0_USART_RxPushByte(uint8_t rdByte)
{
    uint32_t tempInIndex;
    bool isSuccess = false;

    tempInIndex = sercom0USARTObj.rdInIndex + 1;

    if (tempInIndex >= SERCOM0_USART_READ_BUFFER_SIZE)
    {
        tempInIndex = 0;
    }

    if (tempInIndex == sercom0USARTObj.rdOutIndex)
    {
        /* Queue is full - Report it to the application. Application gets a chance to free up space by reading data out from the RX ring buffer */
        if(sercom0USARTObj.rdCallback != NULL)
        {
            sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_BUFFER_FULL, sercom0USARTObj.rdContext);

            /* Read the indices again in case application has freed up space in RX ring buffer */
            tempInIndex = sercom0USARTObj.rdInIndex + 1;

            if (tempInIndex >= SERCOM0_USART_READ_BUFFER_SIZE)
            {
                tempInIndex = 0;
            }
        }
    }

    /* Attempt to push the data into the ring buffer */
    if (tempInIndex != sercom0USARTObj.rdOutIndex)
    {
        SERCOM0_USART_ReadBuffer[sercom0USARTObj.rdInIndex] = rdByte;
        sercom0USARTObj.rdInIndex = tempInIndex;
        isSuccess = true;
    }
    else
    {
        /* Queue is full. Data will be lost. */
    }

    return isSuccess;
}

/* This routine is only called from ISR. Hence do not disable/enable USART interrupts. */

size_t SERCOM0_USART_Read(uint8_t* pRdBuffer, const size_t size)
{
    size_t nBytesRead = 0;
    uint32_t rdOutIndex;
    uint32_t rdInIndex;

    while (nBytesRead < size)
    {
        SERCOM0_USART_RX_INT_DISABLE();

        rdOutIndex = sercom0USARTObj.rdOutIndex;
        rdInIndex = sercom0USARTObj.rdInIndex;

        if (rdOutIndex != rdInIndex)
        {
            pRdBuffer[nBytesRead++] = SERCOM0_USART_ReadBuffer[sercom0USARTObj.rdOutIndex++];

            if (sercom0USARTObj.rdOutIndex >= SERCOM0_USART_READ_BUFFER_SIZE)
            {
                sercom0USARTObj.rdOutIndex = 0;
            }
            SERCOM0_USART_RX_INT_ENABLE();
        }
        else
        {
            SERCOM0_USART_RX_INT_ENABLE();
            break;
        }
    }

    return nBytesRead;
}

void static SERCOM0_USART_ISR_ERR_Handler( void )
{
    USART_ERROR errorStatus = USART_ERROR_NONE;

    errorStatus = (SERCOM0_REGS->USART_INT.SERCOM_STATUS &
                  (SERCOM_USART_INT_STATUS_PERR_Msk |
                  SERCOM_USART_INT_STATUS_FERR_Msk |
                  SERCOM_USART_INT_STATUS_BUFOVF_Msk | SERCOM_USART_INT_STATUS_ISF_Msk
                  ));

    if(errorStatus != USART_ERROR_NONE)
    {
        SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_ERROR_Msk;

        if(sercom0USARTObj.rdCallback != NULL)
        {
            sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_READ_ERROR, sercom0USARTObj.rdContext);
        }

        /* In case of errors are not cleared by client using ErrorGet API */
        SERCOM0_USART_ErrorClear();
    }
}

void static SERCOM0_USART_ISR_RX_Handler( void )
{

    if (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXBRK_Msk)
    {
        /* Clear the receive break interrupt flag */
        SERCOM0_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_RXBRK_Msk;

        sercom0USARTObj.rdCallback(SERCOM_USART_EVENT_BREAK_SIGNAL_DETECTED, sercom0USARTObj.rdContext);
    }
    if (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        if (SERCOM0_USART_RxPushByte( SERCOM0_REGS->USART_INT.SERCOM_DATA) == true)
        {
            SERCOM0_USART_ReadNotificationSend();
        }
        else
        {
            /* UART RX buffer is full */
        }
    }
}

void SERCOM0_USART_InterruptHandler( void )
{
    if(SERCOM0_REGS->USART_INT.SERCOM_INTENSET != 0)
    {
        /* Checks for data register empty flag */
        if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_DRE_Msk) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk))
        {
            SERCOM0_USART_ISR_TX_Handler();
        }

        /* Checks for receive complete empty flag */
        if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & (SERCOM_USART_INT_INTENSET_RXC_Msk | SERCOM_USART_INT_INTENSET_RXBRK_Msk)) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & (SERCOM_USART_INT_INTFLAG_RXC_Msk | SERCOM_USART_INT_INTFLAG_RXBRK_Msk)))
        {
            SERCOM0_USART_ISR_RX_Handler();
        }

        /* Checks for error flag */
        if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_ERROR_Msk) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk))
        {
            SERCOM0_USART_ISR_ERR_Handler();
        }
    }
}