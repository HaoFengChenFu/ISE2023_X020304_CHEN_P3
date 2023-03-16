#include "SNTP.h"

/****************************************
			id del Thread
****************************************/
osThreadId_t tid_ThSNTP;  

/****************************************
			Otras variables
****************************************/
const  NET_ADDR4 ntp_server = { NET_ADDR_IP4 , 0, 217, 79, 179, 106 };

/****************************************
			Prototipo funciones
****************************************/
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
void STNP_Thread (void *argument); 

/****************************************
			Timer virtual
****************************************/
osTimerId_t timer_SNTP;
static uint32_t exec;
osStatus_t status;


/*******************************************************************************************
			Funcion que accede al servidor por defecto del Net_Config_SNTP_Client 

			Para eso se usa el NULL y se devuelve el resultado en segundos en la callback
*******************************************************************************************/
void get_SNTP_Time(void) 
{
  if (netSNTPc_GetTime(NULL, time_callback) == netOK) {
    printf ("SNTP request sent.\n");
  }
  else {
    printf ("SNTP not ready or bad parameters.\n");
  }
}
/**************************************************************************************
			Callback de netSNTPc_GetTime
***************************************************************************************/
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) 
{
  if (seconds == 0) {
    printf ("Server not responding or bad response.\n");
  }
  else {
    printf ("%d seconds elapsed since 1.1.1970\n", seconds);
    getLocalTime(seconds);
  }
}

/**************************************************************************************
			Funcion que convierte los segundos en la hora local
***************************************************************************************/
void getLocalTime(uint32_t seconds)
{
  time_t rawtime = seconds;
  struct tm ts;
  char buf[80];
  
  ts = *localtime(&rawtime);
  strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
	
	// Añadido    A lo mejor hay que pasarlo a (uint8_t)		**************************************************************
	Set_RTC_Time(ts.tm_hour+1, ts.tm_min, ts.tm_sec);				// Hora + 1 porque el que devuelve está retrasado 1 hora
	Set_RTC_Date(ts.tm_year-100, ts.tm_mon+1, ts.tm_wday, ts.tm_yday-18);
	
	
	
	// A lo mejor por las lineas de antes esto ya no hace falta porque se ha configurado los datos en el rtc
	//strftime(buf, sizeof(buf), "Date %Y-%m-%d\n", &ts);
	//LCD_symbolToLocalBuffer_L1(buf, strlen(buf));
	
	//strftime(buf, sizeof(buf), "Time %H:%M:%S %Z\n", &ts);
	//LCD_symbolToLocalBuffer_L2(buf, strlen(buf));
  //printf("%s\n", buf);
}

// Iniciacion del thread ***********************************************************************************************************************************
int Init_ThSNTP (void) {
  tid_ThSNTP = osThreadNew(STNP_Thread, NULL, NULL);
  if (tid_ThSNTP == NULL) {
    return(-1);
  }
	
  return(0);
}

// Iniciacion del timer virtual ***********************************************************************************************************************************

/**************************************************************************************
			Callback del timer virtual
***************************************************************************************/
static void TimerV_Callback(void const *arg)			// Callback creada para el timer virtual
{
	osDelay(5000);
	get_SNTP_Time();
	osThreadFlagsSet(tid_ThSNTP, 1);
	
}

/**************************************************************************************
			Inicialización del timer virtual
***************************************************************************************/
int Init_timer_SNTP (void)
{
	exec = 8U;
	timer_SNTP = osTimerNew((osTimerFunc_t)&TimerV_Callback, osTimerOnce, &exec, NULL);				// Se ha elegido periodic para que se lance cada 3 minutos
	if(timer_SNTP != NULL){
		//status = osTimerStart(timer_id, 1000U);		// Esta parte es solo para verificar que no da errores
		if( status != osOK){
			return -1;
		}
	}
	return NULL;
}

/**************************************************************************************
			Thread donde se pone el timer virtual para que se lance cada 3 minutos
***************************************************************************************/
void STNP_Thread(void *argument) 
{
	while(1){
		osDelay(5000);
		get_SNTP_Time();
	}
	
	
	// Intento con un timer virtual, acaba en el bucle de osRTXError
	
//	Init_timer_SNTP();		// Iniciamos el timer virtual aqui para que no haya errores
//	status = osTimerStart(timer_SNTP, 10000);		// Esperamos 3 minutos (180000)
//	
//  while (1) {
//		status = osTimerStart(timer_SNTP, 5000);		// Esperamos 3 minutos
//		osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
//  }
	
	
}
