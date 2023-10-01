#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_STACK_SIZE 256
#define DEFAULT_NICE 0

struct _Thread;
struct _ThreadConfig;
struct _System;
struct _SystemConfig;

typedef struct _Thread Thread;
typedef struct _ThreadConfig ThreadConfig;
typedef struct _System System;
typedef struct _SystemConfig SystemConfig;

typedef void(ThreadFunc)(void *);

typedef uintptr_t ThreadID;

enum ThreadState {
  STARTING, // Use exec.call.func(exec.call.arg)
  RUNNING,  // Use longjump(exec.run_jump)
  FINISHED, // Nothing to do
};

typedef enum ThreadState ThreadState;

struct _ThreadConfig {
  size_t stack_size;
  uint32_t nice;
};

struct _Thread {
  Thread *next;
  ThreadConfig cfg;
  ThreadState state;
  uint8_t *stack;

  // exec is a jump buffer or the initial function/arg
  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      void *arg;
    } call;
  } exec;
};

struct _SystemConfig {
  uint32_t default_stack_size;
};

struct _System {
  SystemConfig cfg;
  jmp_buf return_jump;
  void *run_stack_pos;
  Thread *runnable;
  Thread *running;
};

SystemConfig DefaultSystemConfig(void);
ThreadConfig DefaultThreadConfig(void);

int Init(System *sys, SystemConfig cfg);

int Create(System *sys, Thread *thread, uint8_t *stack, ThreadFunc *func, void *arg, ThreadConfig cfg);

int Run(System *sys);

void Yield(System *sys);

ThreadID PID(System *sys);

#ifdef __cplusplus
}
#endif
