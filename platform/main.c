#include "typedefs.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "gpio_defs.h"
#include "led.h"
#include "radio.h"

/*******************************************************
; Mode                      = Advanced
; Part Number               = CMT2300A
; Frequency                 = 433.920 MHz
; Xtal Frequency            = 26.0000 MHz
; Demodulation              = GFSK
; AGC                       = On
; Data Rate                 = 9.6 kbps
; Deviation                 = 20.0 kHz
; Tx Xtal Tol.              = 20 ppm
; Rx Xtal Tol.              = 20 ppm
; TRx Matching Network Type = 20 dBm
; Tx Power                  = +20 dBm
; Gaussian BT               = 0.5
; Bandwidth                 = Auto-Select kHz
; Data Mode                 = Packet
; Whitening                 = Disable
; Packet Type               = Fixed Length
; Payload Length            = 32
*********************************************************/

#define RF_RX_TIMEOUT    1000*60*60      //60min

#define RF_PACKET_SIZE   32               /* Define the payload size here */
volatile u32 g_nRfRxtimeoutCount = 0;
static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

char str[32];
u32 g_nRecvCount=0,g_nSendCount=0;


u8 Radio_Send_FixedLen(const u8 pBuf[], u8 len)
{
	u32 delay;
	CMT2300A_GoStby();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearTxFifo();
	CMT2300A_EnableWriteFifo();
	
	CMT2300A_WriteFifo(pBuf, len); // д TX_FIFO
	CMT2300A_GoTx(); 
	delay = 1000;
	while(1)
	{
		if(CMT2300A_ReadGpio3())
		{
			CMT2300A_ClearInterruptFlags();
			CMT2300A_GoSleep();
			system_delay_ms(100);
			g_nSendCount++; 
			printf("send: %d\n", g_nSendCount);
			return 1;
		}
		
		if(delay==0){
			printf("send: Timeout!!\n");
			RF_Init(1); 
			return 0;
		}
		
		system_delay_100us(2);
		delay--;
	}
}


u8 Radio_Recv_FixedLen(u8 pBuf[], u8 len)
{
#ifdef ENABLE_ANTENNA_SWITCH
	if(CMT2300A_ReadGpio3())  /* Read INT2, PKT_DONE */
#else
	if(CMT2300A_ReadGpio1()) /* Read INT1, SYNC OK */
	{
		/******/
		printf("SYNC OK\n");
	}
	if(CMT2300A_ReadGpio2())  /* Read INT2, PKT_DONE */
#endif	
	{
		if(CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG))  /* Read PKT_OK flag */
		{
			CMT2300A_GoStby();
			CMT2300A_ReadFifo(pBuf, len);
			CMT2300A_ClearRxFifo();
			CMT2300A_ClearInterruptFlags();
			CMT2300A_GoRx();
			return 1;
		}else{
			printf("CMT2300A_MASK_PKT_OK_FLG\n");
		}
	}
	return 0;
}




/* Main application entry point */
int main(int argc, char **argv)
{
	int i;

	if(argc != 2){
		return -1;
	}

	int master = atoi(argv[1]);

	if(master){
		for(i=0; i<RF_PACKET_SIZE; i++) 
			g_txBuffer[i] = 1+i;
	}

    RF_Init(master);
	led_init();
	if(FALSE == CMT2300A_IsExist()) {
		printf("CMT2300A not found!\n");
		while(1);
	} else {
		if(master){
			printf("CMT2300A ready TX\n");
		}else{
			printf("CMT2300A ready RX\n");
		}
	}

	if(master == 0){
		CMT2300A_GoStby();
		/* Must clear FIFO after enable SPI to read or write the FIFO */
    	CMT2300A_EnableReadFifo();
		CMT2300A_ClearInterruptFlags();
		CMT2300A_ClearRxFifo();
		CMT2300A_GoRx();
	}
	
	int count = 0;
	while(1) 
	{
		if(led_is_on(LED_ALL)){
			led_off(LED_ALL);
		}else{
			led_on(LED_ALL);
		}

		if(master){
			int *p = (int *)g_txBuffer;
			*p = count++;
			Radio_Send_FixedLen(g_txBuffer, RF_PACKET_SIZE);
			system_delay_ms(1000);
		}else{
			if(Radio_Recv_FixedLen(g_rxBuffer, RF_PACKET_SIZE))
			{
				g_nRfRxtimeoutCount=0; 
				int *p = (int *)g_rxBuffer;
				printf("recv: %d\n", *p);
				for(i=0;i<RF_PACKET_SIZE;i++) {
				   printf("%02x ", g_rxBuffer[i]);
				   g_rxBuffer[i]=0;
				}
				printf("\n");
			}

			if(g_nRfRxtimeoutCount>RF_RX_TIMEOUT)
			{
				g_nRfRxtimeoutCount=0;
				CMT2300A_GoSleep();
				CMT2300A_GoStby();
				CMT2300A_ClearInterruptFlags();
				CMT2300A_ClearRxFifo();
				CMT2300A_GoRx();
			}
			g_nRfRxtimeoutCount++;
			system_delay_ms(50);
		}
	}
}


