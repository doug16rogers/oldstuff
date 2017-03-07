#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


#include "tasker.hpp"



class task_a: task {
  char* id;
public:
  task_a(scheduler& s,char *name): task(s), id(name) { }
private:
  void task_main(void);
};   //class task_a


void task_a::task_main(void) {
  for(;;) {
    printf("task %s, line 1\n",id);
    if (getch()==0x1b) exit(1);
    suspend();
    printf("task %s, line 2\n",id);
    if (getch()==0x1b) exit(1);
    suspend();
  }   //for
}   //task_a::task_main


//===========================================================================
void main(void) {
  scheduler s(5);
  task_a a(s,"TaskA"),b(s,"TaskB"),c(s,"TaskC");

  printf("============================================\n");
  s.run_tasks();
}   //main

