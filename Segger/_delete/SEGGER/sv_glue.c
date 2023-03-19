/*
 * SystemView glue
 * Chernih Evgeny
 * 2021 Triton
 */

#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"

//#include "sys_pmu.h"
extern void _pmuInit_(void);
#define pmuCYCLE_COUNTER 0x80000000U
extern void _pmuStartCounters_(uint32_t counters);
extern uint32_t _pmuGetCycleCount_(void);

extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "CIVL-FreeRTOS"
// The target device name
#define SYSVIEW_DEVICE_NAME     "CortexR4f"
// Frequency of the timestamp. Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (portTIMER_FOR_RUN_TIME_STATS_FREQ_HZ)
// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        portTIMER_FOR_RUN_TIME_STATS_FREQ_HZ
// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x08000000)

//Sends SystemView description strings.
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",D="SYSVIEW_DEVICE_NAME",O=FreeRTOS");

  // so far all values of SEGGER_SYSVIEW_InterruptId are set in FreeRTOSConfig.h
  // they all need to be in sync with this descriptor
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#1=SVC,I#2=SSI");
}

void SEGGER_SYSVIEW_Conf(void) {
  // start cortex-R4 cycle counetr
  _pmuInit_();
  _pmuStartCounters_(pmuCYCLE_COUNTER);
  // start segger's stuff
  SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, 
                      &SYSVIEW_X_OS_TraceAPI, _cbSendSystemDesc);
  SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

// this function is called by segger inside ISR hook
//unsigned int SEGGER_SYSVIEW_InterruptId;
unsigned long SEGGER_SYSVIEW_X_GetInterruptId(void) {
  return SEGGER_SYSVIEW_InterruptId;
}
unsigned long SEGGER_SYSVIEW_X_GetTimestamp(void) {
  return portGET_RUN_TIME_COUNTER_VALUE();
}