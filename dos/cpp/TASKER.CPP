#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "tasker.hpp"


task::task(scheduler& s): owner(s), task_frame(NULL) {
  owner.add_task(this);                //add current task to scheduler
}   //task::task


void task::initialize(void) {
  unsigned char stackspace[stacksize]={1};   //leave scheduler breathing room
  frame_top=(void*)_SP;                //get top of task's stack
  task_main();                         //see ya!
}   //task::initialize


void task::suspend(void) {
  frame_bottom=(void*)_SP;             //about to suspend - calculate size..
  frame_size=(int)frame_top-(int)frame_bottom;  //..of current stack usage
//  if (task_frame) { delete task_frame; }        //???delete old
  if (!task_frame) task_frame=(void*)new char[frame_size];
  memcpy(task_frame,frame_bottom,frame_size);   //save away the stack
  if (!setjmp(task_environment))       //store our place/activation record
    longjmp(owner.scheduler_environment,1);     //go to scheduler if we're..
}   //task::suspend                             //..not being jumped to


void task::resume(void) {
  memcpy(frame_bottom,task_frame,frame_size);   //put stack back in place
  longjmp(task_environment,1);                  //jump to task
}   //task::resume


void scheduler::add_task(task* t) {
  if (count<sz) tsk[count++]=t;
}   //scheduler::add_task


void scheduler::run_tasks(void) {
  setjmp(scheduler_environment);
  index=(index+1)%count;
  if (!tsk[index]->initialized())
    tsk[index]->initialize();
  else
    tsk[index]->resume();
}   //scheduler::run_tasks

