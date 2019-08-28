
#include <cmsis_os.h>                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

void AuxThread (void const *argument);		// thread function
osThreadId tid_AuxThread;									// user thread ID
osThreadDef (AuxThread, osPriorityNormal, 1, 0);

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if(!tid_Thread) return(-1);
	
	tid_AuxThread = osThreadCreate(osThread(AuxThread), NULL);
	if(!tid_AuxThread) return (-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
		
		__nop();
    ; // Insert thread code here...
    osThreadYield();                                            // suspend thread
  }
}

void AuxThread (void const *argument) {

  while (1) {
		__nop();
    ; // Insert thread code here...
    osThreadYield();                                            // suspend thread
  }
}


