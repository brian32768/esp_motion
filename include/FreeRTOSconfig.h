#define configUSE_TIMERS 1

#define configTIMER_SERVICE_TASK_NAME "Tmr Svc"
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2)

