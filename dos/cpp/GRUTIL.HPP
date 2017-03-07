#ifndef __GRUTIL_HPP
#define __GRUTIL_HPP

#include <graphics.h>

extern int Xmax;
extern int Ymax;
extern int Cmax;
extern int Graph_on;

class Graph {
private:
  int err;
public:
  Graph(char* bgi_dir="");
  ~Graph();
  int On(void) { return Graph_on; }
  int Error(void) { return err; }
  char* Message(void) { return grapherrormsg(err); }
};   //class Graph


#endif
