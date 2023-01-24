#ifndef TASK_WORK_STEALING_THREAD_POOL_H
#define TASK_WORK_STEALING_THREAD_POOL_H 

#if defined (__i386__) || defined(__x86_64__)
  #define pause_or_yield  __builtin_ia32_pause
#elif __aarch64__ 
  //__yield();
  #define pause_or_yield() asm volatile("yield" ::: "memory")
#else
    static_assert(0!=0, "Unknown CPU architecture");
#endif

typedef struct{
  void* args;
  void (*func)(void* args);
} task_t;

#endif

