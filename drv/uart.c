#include "main.h" 
#include "stdarg.h"


UART_RECV_BUFF NB_Recv_Buff;
UART_RECV_BUFF CMD_Recv_Buff;
unsigned char UartBuff[uart_rx_size];
unsigned char CMDBuff[USART_BUFF_SIZE];    //存放协议指令

FlagStatus Uart_RxErr_ChkEx(UART_Type* UARTx)
{
	if( (SET == LL_UART_IsActiveFlag_PERR(UARTx))
	||(SET == LL_UART_IsActiveFlag_FERR(UARTx))
	||(SET == LL_UART_IsActiveFlag_RXERR(UARTx)) )
	{
		return SET;
	}
	else
	{
		return RESET;
	}	
}

void Uart_RxErr_ClrEx(UART_Type* UARTx)
{
	LL_UART_ClearFlag_PERR(UARTx);
	LL_UART_ClearFlag_FERR(UARTx);
	LL_UART_ClearFlag_RXERR(UARTx);	
}



void Uartx_Init(UART_Type* UARTx)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_UART_InitTypeDef UART_InitStruct = {0};    
    
	switch((uint32_t)UARTx)
	{
		case UART0_BASE:
			//PA13:UART0-RX   PA14:UART0-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;
			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
			//PA2:UART0-RX   PA3:UART0-TX
//			GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
//			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//			GPIO_InitStruct.Pull = DISABLE;
//			GPIO_InitStruct.RemapPin = DISABLE;
//			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
		
			UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
			/*NVIC中断配置*/
			NVIC_DisableIRQ(UART0_IRQn);
			NVIC_SetPriority(UART0_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART0_IRQn);
			break;
		
		case UART1_BASE:
			//PB13:UART1-RX   PB14:UART1-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;			
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
			//PC2:UART1-RX   PC3:UART1-TX
//			GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
//			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//			GPIO_InitStruct.Pull = DISABLE;
//			GPIO_InitStruct.RemapPin = DISABLE;
//			LL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
				
			UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
			/*NVIC中断配置*/
			NVIC_DisableIRQ(UART1_IRQn);
			NVIC_SetPriority(UART1_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART1_IRQn);
			break;
			
		case UART4_BASE:
			//PB2:UART4-RX   PB3:UART4-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;			
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
			//PA0:UART4-RX   PA1:UART4-TX
//			GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
//			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//			GPIO_InitStruct.Pull = DISABLE;
//			GPIO_InitStruct.RemapPin = DISABLE;
//			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
			/*NVIC中断配置*/
			NVIC_DisableIRQ(UART4_IRQn);
			NVIC_SetPriority(UART4_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART4_IRQn);
			break;
		
		case UART5_BASE:
			//PD0:UART5-RX   PD1:UART5-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;			
			LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
			//PC4:UART5-RX   PC5:UART5-TX
//			GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5;
//			GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//			GPIO_InitStruct.Pull = DISABLE;
//			GPIO_InitStruct.RemapPin = DISABLE;
//			LL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
			/*NVIC中断配置*/
			NVIC_DisableIRQ(UART5_IRQn);
			NVIC_SetPriority(UART5_IRQn,2);//中断优先级配置
			NVIC_EnableIRQ(UART5_IRQn);
			break;
				
		default:
			break;
    }
	UART_InitStruct.BaudRate = 9600;								//波特率
	UART_InitStruct.DataWidth = LL_UART_DATAWIDTH_8B;				//数据位数
	UART_InitStruct.StopBits = LL_UART_STOPBITS_1;					//停止位
	UART_InitStruct.Parity = LL_UART_PARITY_NONE;					//奇偶校验
	UART_InitStruct.TransferDirection = LL_UART_DIRECTION_NONE;	//接收-发送使能
	UART_InitStruct.InfraredModulation = DISABLE;	    
	LL_UART_Init(UARTx, &UART_InitStruct);
	
	LL_UART_DisableIT_ShiftBuffEmpty(UARTx);  //关闭发送中断
	LL_UART_EnableIT_ReceiveBuffFull(UARTx);  //打开接收中断
	LL_UART_EnableDirectionTx(UARTx);   	  //打开发送使能
	LL_UART_EnableDirectionRx(UARTx);		  //打开接收使能
    LL_UART_Enable_NegtiveEdgeWakeup(UARTx);  //打开串口唤醒
}



void Uartx_open(UART_Type *UARTx)
{
    Uartx_Init(UARTx);
}

void Uartx_close(UART_Type *UARTx)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};  
    
	switch((uint32_t)UARTx)
	{
		case UART0_BASE:
			//PA13:UART0-RX   PA14:UART0-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            GPIOA->DRST = LL_GPIO_PIN_13|LL_GPIO_PIN_14;    //输出低
            
		
//			UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
//			/*NVIC中断配置*/
//			NVIC_DisableIRQ(UART0_IRQn);
//			NVIC_SetPriority(UART0_IRQn,2);//中断优先级配置
//			NVIC_EnableIRQ(UART0_IRQn);
			break;
		
		case UART1_BASE:
			//PB13:UART1-RX   PB14:UART1-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;	
            GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            GPIOB->DRST = LL_GPIO_PIN_13|LL_GPIO_PIN_14;    //输出低
		
			break;
			
		case UART4_BASE:
			//PB2:UART4-RX   PB3:UART4-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;			
			LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            GPIOB->DRST = LL_GPIO_PIN_2|LL_GPIO_PIN_3;    //输出低
		
			break;
		
		case UART5_BASE:
			//PD0:UART5-RX   PD1:UART5-TX
			GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			GPIO_InitStruct.Pull = DISABLE;
			GPIO_InitStruct.RemapPin = DISABLE;			
			LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
            GPIOD->DRST = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
		
			break;
				
		default:
			break;
    }
//	UART_InitStruct.BaudRate = 115200;								//波特率
//	UART_InitStruct.DataWidth = LL_UART_DATAWIDTH_8B;				//数据位数
//	UART_InitStruct.StopBits = LL_UART_STOPBITS_1;					//停止位
//	UART_InitStruct.Parity = LL_UART_PARITY_NONE;					//奇偶校验
//	UART_InitStruct.TransferDirection = LL_UART_DIRECTION_NONE;	//接收-发送使能
//	UART_InitStruct.InfraredModulation = DISABLE;			        
//	LL_UART_Init(UARTx, &UART_InitStruct);
	
	LL_UART_DisableIT_ShiftBuffEmpty(UARTx);   //关闭发送中断
    LL_UART_DisableIT_ReceiveBuffFull(UARTx);  //关闭接收中断
    LL_UART_DisableDirectionTx(UARTx);   	   //关闭发送使能
    LL_UART_DisableDirectionRx(UARTx);		   //关闭接收使能
    
    
}

int Uart1SendData(unsigned char *pSendBuf, unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;
#if ENABLE_LPUART1
        LPUART1->TXBUF = *pSendBuf ++;
        while(SET != LL_LPUART_IsActiveFlag_TXSE(LPUART1))
#else
        UART1->TXBUF = *pSendBuf ++;
        while(SET != LL_UART_IsActiveFlag_TXSE(UART1))
#endif
        {
            i --;
            if(i == 0)
                return 0;
        }
       // LL_UART_TransmitData(UART1, *pSendBuf ++);
        
        Len --;
    }
    return 1;
}

int Uart0SendData(unsigned char *pSendBuf, unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;
        UART0->TXBUF = *pSendBuf ++;
        while(SET != LL_UART_IsActiveFlag_TXSE(UART0))
        {
            i --;
            if(i == 0)
                return 0;
        }
       // LL_UART_TransmitData(UART1, *pSendBuf ++);
        
        Len --;
    }
    return 1;
}

void HexToAscii( unsigned char data, unsigned char *pout )
{
    unsigned char temp = 0;
    
    temp = (data & 0xF0) >> 4;
    if(( 0x0A <= temp) && (temp <= 0x0F)) 
        *pout++ = temp - 0x0A + 'A';
    else if((temp <= 9)) //( 0 <= temp) && 
        *pout++ = temp + '0';
    
    temp = data & 0x0F;
    if(( 0x0A <= temp) && (temp <= 0x0F)) 
        *pout++ = temp - 0x0A + 'A';
    else if((temp <= 9)) //( 0 <= temp) && 
        *pout++ = temp + '0';
}

//将hex转换为assic后再发送
void HexToAssic_Uart0Send(unsigned char *pSendBuf, unsigned int Len)
{
    uint8_t sendata[2];
    
    uint16_t i;
#if TRAN_PRO == MQTT_PRO    
    
    
    uint8_t sendbuf[4];
    for(i = 0;i<Len;i++)
    {
        HexToAscii(*(pSendBuf+i),sendata);
        sprintf((char *)sendbuf,"%02X%02X",sendata[0],sendata[1]);
        Uart0SendData(sendbuf,4);
    }
#elif TRAN_PRO == LWM2M_PRO   
    for(i = 0;i<Len;i++)
    {    
        HexToAscii(*(pSendBuf+i),sendata);
        Uart0SendData(sendata,2);
    }
#endif    
}


void HexToAssic_Uart1Send(unsigned char *pSendBuf, unsigned int Len)
{
    
    uint16_t i;
    uint8_t sendata[2];
#if TRAN_PRO == MQTT_PRO
    
    uint8_t sendbuf[4];
    for(i = 0;i<Len;i++)
    {
        HexToAscii(*(pSendBuf+i),sendata);
        sprintf((char *)sendbuf,"%02X%02X",sendata[0],sendata[1]);    //继续转   一共要转两次，第一次是nb模块发送的必须是ASCII，平台收到的也必须是ASCII
        Uart1SendData(sendbuf,4);
    }
#elif TRAN_PRO == LWM2M_PRO
    for(i = 0;i<Len;i++)
    {
        HexToAscii(*(pSendBuf+i),sendata);
        Uart1SendData(sendata,2);
    }
#endif
}

CmdType RecvDataFromBuff(UART_RECV_BUFF *CMD,unsigned char *buf)
{
    unsigned short datalen = 0;
    CmdType cmd = Err_Type;
    unsigned short CMD_Len = 0;
    uint8_t dat;
    memset(buf,0,256);
#ifdef FTP_UPDATA
    if(FtpInfo.FtpRunning == 1)
#else
    if(HttpInfo.HttpRunning == 1)
#endif
    {
        return Err_Type;   //文件下载的时候不进行数据处理
    }
    while(CMD->p_read != CMD->p_write)
    {       
        dat = CMD->Recvbuff[CMD->p_read++];
        Uart1SendData(&dat,1);
        
        if(datalen == 0 && dat == 0x68)
        {
           cmd =  PROT_Type;
        }
        else if(datalen == 0 && dat != 0x68)
        {
            cmd =  AT_Type;
        }
        
        buf[datalen++] = dat;
        if(CMD->p_read >= USART_BUFF_SIZE) 
            CMD->p_read = 0;
        
        
        if(cmd == AT_Type)
        {
            if(datalen >=2 && buf[datalen-2] == 0x0D && buf[datalen-1] == 0x0A)
            {
                datalen = 0;
                return AT_Type;
            }
        }
        else if(cmd == PROT_Type)
        {
            if(datalen == 30)
            {
                CMD_Len = buf[29];
                CMD_Len = (CMD_Len<<8) + buf[28];
            }
            
            if(datalen == CMD_Len+3+30 && dat == 0x16)
            {
                return PROT_Type;
            }
        }
        
    }
    return Err_Type;
}

void print(const char fmt[], ...)
{
#ifdef PRINT_INFO_EN    
    unsigned char DebugBuf[200];  // 串口输出是阻塞式的、不会导致多个print函数同时运行，所以这里不会显著增大占空间的使用
    va_list args;
    int res;
    //LL_UART_EnableDirectionTx(UART1);   	  //打开发送使能
    va_start(args, fmt);
    res = vsnprintf((char *)DebugBuf, sizeof(DebugBuf), fmt, args);
    va_end(args);
    // vsnprintf()会自动在格式化之后的字符串的末尾增加'\0'，但res不包含这个字符，所以DebugBuf[N]最多能放N-1个字符，多余的字符将被截断
    if(res > 0)
    {
        Uart1SendData(DebugBuf, strlen((char const*)DebugBuf) );
    }
#else 
 return ;   
#endif
}


void Uart0Overtime()
{
    if(Up_process.UpState != S_Halt)   //上告的时候不处理
        return;
    if(RecvDataFromBuff(&CMD_Recv_Buff,CMDBuff) == PROT_Type)        
        Process_Cmd(CMDBuff,DEBUG_COM);
    task_stop(Uart0Overtime);
}

