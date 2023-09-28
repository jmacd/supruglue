#ifdef __cplusplus
extern "C" {
#endif

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

typedef uint8_t ProcessID;

enum ThreadState {
  STARTING, // Use exec.call.func(exec.call.arg)
  RUNNING,  // Use longjump(exec.run_jump)
};

typedef enum ThreadState ThreadState;

struct _ThreadConfig {
  size_t stackSize;
  uint32_t nice;
};

struct _Thread {
  Thread *next;
  ThreadConfig cfg;
  ThreadState state;
  union {
    jmp_buf run_jump;
    struct {
      ThreadFunc *func;
      void *arg;
    } call;
  } exec;
  uint8_t id;
};

struct _SystemConfig {
  uint32_t defaultStackSize;
};

struct _System {
  SystemConfig cfg;
  jmp_buf return_jump;
  Thread *runnable;
};

SystemConfig DefaultSystemConfig();
ThreadConfig DefaultThreadConfig();

int Init(System *sys, SystemConfig cfg);

int Create(System *sys, Thread *thread, ThreadFunc *func, void *arg, ThreadConfig cfg);

int Run(System *sys);

void Yield();

ProcessID Pid();

#ifdef __cplusplus
}
#endif
