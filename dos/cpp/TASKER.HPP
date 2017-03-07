#ifndef __TASKER_HPP
#define __TASKER_HPP


#include <setjmp.h>

class scheduler;


class task {
  enum { stacksize=0x20 };
  jmp_buf task_environment;
  scheduler& owner;
  void* task_frame;
  void* frame_top;
  void* frame_bottom;
  unsigned long frame_size;
public:
  int initialized(void) { return (int)task_frame; }
  task(scheduler& s);
  virtual ~task(void) { if (task_frame) delete task_frame; }
  void initialize(void);
  virtual void task_main(void)=0;
  void suspend(void);
  void resume(void);
};   //class task


class scheduler {
  const sz;
  task** tsk;
  int count;
  int index;
  jmp_buf scheduler_environment;
public:
  scheduler(int size=10) : sz(size),count(0),index(-1) { tsk=new task*sz; }
  ~scheduler() { delete tsk; }
  void add_task(task* t);
  void run_tasks(void);
  friend class task;
};   //class scheduler


#endif
