#if OSG_OS_NAME == OSG_OS_CMSIS_RTOS2
    //#define RTOS_PATH ../../rtos/rtos2/src/
    #include "../../rtos/rtos2/src/rtos-BaseHw.c"
    #if OSG_OS_USE_MESSAGES == 1
        #include "../../rtos/rtos2/src/drivers/rtos-MessageQueue.c"
    #endif
    #if OSG_OS_USE_MUTEX == 1
        #include "../../rtos/rtos2/src/drivers/rtos-Mutex.c"
    #endif
    #if OSG_OS_USE_THREAD == 1
        #include "../../rtos/rtos2/src/drivers/rtos-Thread.c"
    #endif
    #if OSG_OS_USE_TIMER == 1
        #include "../../rtos/rtos2/src/drivers/rtos-Timer.c"
    #endif
#else
    #error "Unrecognized RTOS macro"
#endif

// https://stackoverflow.com/questions/9096201/concatenate-string-in-c-include-filename
//#define RTOS_STRINGIFY(X) #X
//#define RTOS_FILE2(A, B) RTOS_STRINGIFY(A ## B)
//#define RTOS_FILE_PATHX(A, B) RTOS_FILE2(A, B)
//#define RTOS_FILE_PATH(A) RTOS_FILE_PATHX(RTOS_PATH, A)
//#define RTOS_FILE(A) RTOS_FILE_PATH(A)

//#include RTOS_FILE(rtos-BaseHw.c)

//#if OSG_OS_USE_TIMER == 1
//    #include RTOS_FILE(drivers/rtos-Timer.c)
//#endif

//#if OSG_OS_USE_MUTEX == 1
//    #include RTOS_FILE(drivers/rtos-Mutex.c)
//#endif

//#if OSG_OS_USE_THREAD == 1
//    #include RTOS_FILE(drivers/rtos-Thread.c)
//#endif
