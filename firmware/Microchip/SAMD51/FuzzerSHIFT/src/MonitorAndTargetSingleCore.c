/*
 * SHiFT
 * Alejandro Mera 2022
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include "fuzzing.h"
//#include "usb_device.h"
//#include "usbd_cdc_if.h"
#include "stdbool.h"
#include "ATParser.h"
#include "McuASANconfig.h"
#include "ConfigFuzzing.h"
//#include "stdio.h"

#include "usb_start.h"

#define PRINTF_ALIAS_STANDARD_FUNCTION_NAMES 0
#define PRINTF_ALIAS_STANDARD_FUNCTION_NAMES_SOFT 1
#include "printf/printf/printf.h"


//#include "freertos_tasks_c_additions.h"
//extern USBD_HandleTypeDef hUsbDeviceFS;FAULT_NONE_RTOS
/*
extern CRC_HandleTypeDef hcrc;
extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart3;
 */
extern uint32_t __user_heap_start__[];


const char * EX_str[]=
{
"EX_NO_EX",
"EX_ASAN_DOUBLE_FREE",
"EX_ASAN_USE_AFTER_FREE",
"EX_ASAN_INVALID_FREE",
"EX_ASAN_RW_OVERFLOW",
"EX_ASAN_DEL_MISMATCH",
"EX_ASAN_REALLOC_MISMATCH",
"EX_ASAN_FAIL_POISON",
"EX_MPU_VIOLATION",
"EX_CORTEX_DIV0",
"EX_CORTEX_UNA_ACCESS",
"EX_CORTEX_BUS_FAULT",
"EX_CORTEX_UNDEF_INST",
"EX_HARDFAULT"
};

/** ARMv7 MPU Details:
 *
 * - ARMv7 MPU requires that the size of a MPU region is a power of 2.
 * - Smallest supported region size is 32 bytes.
 * - Start address of a region must be aligned to an integer multiple of the
 *   region size. For example, if the region size is 4 KB(0x1000), the starting
 *   address must be N x 0x1000, where N is an integer.
 */







/**
 * @brief Size of the shared memory region.
 */
#define SHARED_MEMORY_SIZE 32

#define MPUTESTENABLED 0



#if DUALCOREFUZZ == 0
//uint8_t AFLfuzzerRegion[AFLINPUTREGION_SIZE ] __attribute__( ( aligned( AFLINPUTREGION_SIZE ) ) );
extern uint8_t AFLfuzzerRegion[AFLINPUTREGION_SIZE ] __attribute__( ( aligned( AFLINPUTREGION_SIZE ) ) );
#endif

uint32_t error_cnt;

/**
 * Since stack of a task is protected using MPU, it must satisfy MPU
 * requirements as mentioned at the top of this file.
 */

static StackType_t targetTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );
static StackType_t FuzzerTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );


// Entry function for the fuzzer
void vStartMPUDemo( void );

// The targte task
static void targetTask( void * pvParameters );
// The furzzer task
static void fuzzerTask( void * pvParameters );

// Helper to launch targte tasks
static void spawnNewTarget();


#if INCLUDE_HeapMPU == 1
void callbackInvalidFree()
{
	INVALID_FREE;
}
#endif







static void targetTask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;
    uint8_t firstRun;

   #if DUALCOREFUZZ == 0
   //uint16_t *paflbitmap =  (uint16_t *)DTCMRAMORIGIN;
   Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
   #endif

    AFLfuzzer.xTypeEx = EX_NO_EX;
    firstRun=1;

   	xTaskNotifyIndexed(AFLfuzzer.xTaskFuzzer,2,1,eSetValueWithOverwrite); //notify the fuzzer task the target is ready
    for( ; ; )
	{

    	ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the data coming from the fuzzer task

    	if(firstRun)
    	{
    		//AFLfuzzer.bRXcomplete = false;
    		//AFLfuzzer.inputLength = 0;
    		AFLfuzzer.previousGuard = 0;
    		//RingZeroes(&AFLfuzzer.inputAFL);
    		memset(AFLfuzzer.aflbmp,0,AFL_BITMAP_SIZE*sizeof(uint16_t));
    		memset(AFLfuzzer.afldiff,0, AFL_BITMAP_SIZE/4 *sizeof(tuple_t)); //we need on extra tuple to clean which is used for the CRC
    		AFLfuzzer.indexdif = 0;
    		firstRun=0;
    	}

	
		//test(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4 );
		
        mainparser(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4);
        
        /*
        input_cursor = 0;
		// since if input_cursor goes beyond the input buffer, read will just return \n so the buffer won't overflow
		while(input_cursor < (pAFLfuzzer->inputAFL.u32availablenopad-4))
		{
			cat_service(&cat_obj);
		}
        */
        xTaskNotifyIndexed(AFLfuzzer.xTaskFuzzer,0,FAULT_NONE_RTOS,eSetValueWithOverwrite);//notify that the test finished
		//HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	}
}



static void spawnNewTarget( )
{
   #if DUALCOREFUZZ == 0
   //uint16_t *paflbitmap =  (uint16_t *)DTCMRAMORIGIN;
   Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
   #endif

   TaskParameters_t targetTaskParameters =
   {
        	.pvTaskCode		= targetTask,
        	.pcName			= "Target",
        	.usStackDepth	= configMINIMAL_STACK_SIZE,
        	.pvParameters	= NULL,
        	.uxPriority		= 5 | portPRIVILEGE_BIT,
        	.puxStackBuffer	= targetTaskStack,
        	.xRegions		=	{
        			                { AFLfuzzerRegion, AFLINPUTREGION_SIZE, portMPU_REGION_READ_WRITE }, // AFL bitmap, diff buffer, TX diff buffer, this region is shareable
									{__user_heap_start__, 16*1024, portMPU_REGION_READ_WRITE  },
   							 	    { ( void * )0x20020000, 64*1024, portMPU_REGION_READ_WRITE },     // shadow memory

									//{0,0,0}
        						 }
   };


   //create a new task
   xTaskCreateRestricted( &( targetTaskParameters ), &AFLfuzzer.xTaskTarget );



}



uint16_t *indexdifP;
uint8_t  bufferDMA[550];
static void fuzzerTask( void * pvParameters )
{
	/* Unused parameters. */
	( void ) pvParameters;
	uint32_t notificationvalue;
	uint32_t numberofcycles;
	int i;


    #if DUALCOREFUZZ == 0
	//uint16_t *paflbitmap =  (uint16_t *)DTCMRAMORIGIN;
    Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
    uint16_t *paflbitmap =  pAFLfuzzer->aflbmp;
    #endif

    indexdifP = &AFLfuzzer.indexdif;
    
    


	//initialize the USB CDC example provide by Microchip to start receiving data
   // usb_init();

     

	error_cnt = 0;

	/**NOTIFICATION INDEXES**
	 * 0: Notification from target to fuzzer after test execution
	 * 1: Notification from USB USART driver to the fuzzer task when data has arrived
	 * 2: Notification from target to fuzzer to signal readiness to execute the test
	 * ********/

	/**Lets poison the AFLfuzzer structure**/
	uint32_t *HelperShadow =  (uint32_t *)( ((uint32_t)pAFLfuzzer>>3) + McuASAN_CONFIG_APP_MEM_OFFSET);
	uint32_t HelperShadowSize = AFLINPUTREGION_SIZE>>3;
	memset((void *)HelperShadow, 0xff,HelperShadowSize);
	// the only section that should be accessible by the target is the input buffer, so lets un-poison it
	HelperShadow =  (uint32_t *)( ((uint32_t)(&AFLfuzzer.inputAFL.uxBuffer)>>3) + McuASAN_CONFIG_APP_MEM_OFFSET);
	HelperShadowSize = MAX_BUFFER_INPUT>>3;
	memset((void *)HelperShadow, 0x00,HelperShadowSize);


	numberofcycles = 0;
	spawnNewTarget();
	// wait for the target task notification when ready
	ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT/2);

	for( ; ; )
	{

		// we will wait for a notification on index 1 when fuzzing data has arrived
		ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY);
		{

			 xTaskNotify(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite);

			 //we will wait on Index 0 for notification from target task when testing finished
			 notificationvalue = ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT);

			 if (notificationvalue == 0) // TIMEOUT
			 {
				 AFLfuzzer.aflheader[0] = FAULT_TMOUT;
				 printf("Target timeout, starting a new target process...\n");
			     //We need to kill the target task and spawn a new target
				 vTaskDelete(AFLfuzzer.xTaskTarget);
				 taskYIELD(); //lets the kernel clean the TCB
				 numberofcycles = 0;
				 spawnNewTarget();
				 // wait for the target task notification when ready
				 ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT/2);


			 }
			 else if(notificationvalue == FAULT_ASAN)
			 {

				 i= 0;
				 while(i<McuASAN_MAX_NUMBER_ALLOCS) //clean malloc allocations if any
				 {
					 if(AFLfuzzer.allocs[i])
				     {
						 free(AFLfuzzer.allocs[i]);
						 AFLfuzzer.allocs[i] = NULL;
					 }
					 i++;
				  }
				  notificationvalue = FAULT_CRASH;
				  AFLfuzzer.aflheader[0] = notificationvalue;
				  printf("ASAN violation %s \n", (char *)EX_str[AFLfuzzer.xTypeEx]);
				  vTaskDelete(AFLfuzzer.xTaskTarget);
				  taskYIELD();
 				  spawnNewTarget();
 				  ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT/2);
			 }
			 else if(notificationvalue == FAULT_CRASH)
			 {
				 AFLfuzzer.aflheader[0] = notificationvalue;
				 printf("T. crash at: 0x%x, %s \n", (unsigned int)AFLfuzzer.PCcrash, (char *)EX_str[AFLfuzzer.xTypeEx]);
				 //The target process was already killed in the Fault Handler ISR,
				 //We need to spawn a new target task
				 numberofcycles = 0;
				 spawnNewTarget();
				 taskYIELD(); // let's the kernel clean the TCB
				 // wait for the target task notification when ready
				 ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT/2);

			 }
			 else if (notificationvalue == FAULT_NONE_RTOS)
			 {
				 // we need this because 0 means timeout for the RTOS notification system
				notificationvalue = FAULT_NONE;
				AFLfuzzer.aflheader[0] = notificationvalue;

#if PERSISTENT_MODE
				numberofcycles++;
				if(numberofcycles>=FUZZING_CYCLES_IN_PROCESS)
				{
					numberofcycles=0;
#endif
					i = 0;

					while(i<McuASAN_MAX_NUMBER_ALLOCS)
					{
						 if(AFLfuzzer.allocs[i])
						 {
							 free(AFLfuzzer.allocs[i]);
							 AFLfuzzer.allocs[i] = NULL;
						 }
							 i++;
					}
					vTaskDelete(AFLfuzzer.xTaskTarget);
				    taskYIELD();
					spawnNewTarget();
					ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT/2);

#if PERSISTENT_MODE
				}
#endif

			 }

			 AFLfuzzer.aflheader[1] = (AFLfuzzer.indexdif * sizeof(tuple_t)  + 4 );

			 tuple_t *auxdiff = AFLfuzzer.afldiff + 1; // the first tuple is empty so we can skip it
			 union ubytes_t *crcbytes;
			 crcbytes = (union ubytes_t *)(auxdiff + AFLfuzzer.indexdif); //set CRC bytes * at the end of the tuple array

		    uint32_t uwCRCValue = 0;
			CRC_Calculate((uint32_t *)AFLfuzzer.aflheader, 2*4, &uwCRCValue);
			CRC_Calculate((uint32_t *)auxdiff, AFLfuzzer.indexdif*4, &uwCRCValue);
           	crcbytes->vuint32 = uwCRCValue;

			AFLfuzzer.bTXcomplete = false;

            // transmit header
            cdcdf_acm_write((uint8_t *)AFLfuzzer.aflheader, 8);
            
            // wait for end of transmission
			ulTaskNotifyTakeIndexed(1,pdTRUE, 10);

			AFLfuzzer.bTXcomplete = false;

            // transmit the rest 
            cdcdf_acm_write((uint8_t *)auxdiff, AFLfuzzer.aflheader[1]);
          
						
			ulTaskNotifyTakeIndexed(1,pdTRUE, 10);
			
            AFLfuzzer.bRXcomplete = false;
			AFLfuzzer.inputLength = 0;
			AFLfuzzer.previousGuard = 0;
			RingZeroes(&AFLfuzzer.inputAFL);

			for(uint32_t i=0; i<AFLfuzzer.indexdif; i++)
			{
			  paflbitmap[auxdiff[i].index]=0;
			}
			memset(AFLfuzzer.afldiff,0,(AFLfuzzer.indexdif+2)*sizeof(tuple_t)); //we need on extra tuple to clean which is used for the CRC
			AFLfuzzer.indexdif = 0;

		}

	}
}



void vStartMPUDemo( void )
{

Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;

TaskParameters_t fuzzerTaskParameters =
{
	.pvTaskCode		= fuzzerTask,
	.pcName			= "Fuzzer",
	.usStackDepth	= configMINIMAL_STACK_SIZE,
	.pvParameters	= NULL,
	.uxPriority		= 5 | portPRIVILEGE_BIT, //20 | portPRIVILEGE_BIT,
	.puxStackBuffer	= FuzzerTaskStack,
	.xRegions		=	{
			                { 0, 0, 0 },
							{ 0, 0, 0 },
							{ 0, 0, 0 },
					}
};

	/* Create the fuzzer task */
	xTaskCreateRestricted( &( fuzzerTaskParameters ), &AFLfuzzer.xTaskFuzzer );
	//spawnNewTarget();


}
/*-----------------------------------------------------------*/




/*

void vHandleMemoryFault( uint32_t * pulFaultStackAddress )
{
uint32_t ulPC;
uint16_t usOffendingInstruction;

	// Is this an expected fault?
	//if( ucROTaskFaultTracker[ 0 ] == 1 )
	//{
		// Read program counter.
		ulPC = pulFaultStackAddress[ 6 ];

		// Read the offending instruction.
		usOffendingInstruction = *( uint16_t * )ulPC;

		 // From ARM docs:
		 // If the value of bits[15:11] of the halfword being decoded is one of
		 // the following, the halfword is the first halfword of a 32-bit
		 // instruction:
		 // - 0b11101.
		 // - 0b11110.
		 // - 0b11111.
		 // Otherwise, the halfword is a 16-bit instruction.
		 //

		// Extract bits[15:11] of the offending instruction.
		usOffendingInstruction = usOffendingInstruction & 0xF800;
		usOffendingInstruction = ( usOffendingInstruction >> 11 );

		// Determine if the offending instruction is a 32-bit instruction or
		 // a 16-bit instruction.
		if( usOffendingInstruction == 0x001F ||
			usOffendingInstruction == 0x001E ||
			usOffendingInstruction == 0x001D )
		{
			 //Since the offending instruction is a 32-bit instruction,
			 // increment the program counter by 4 to move to the next
			 // instruction.
			ulPC += 4;
		}
		else
		{
			//Since the offending instruction is a 16-bit instruction,
			// increment the program counter by 2 to move to the next
			// instruction. //
			ulPC += 2;
		}

		// Save the new program counter on the stack.
		pulFaultStackAddress[ 6 ] = ulPC;
}

*/
/*-----------------------------------------------------------*/
