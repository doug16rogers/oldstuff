#ifndef __SYMUTIL_HPP
#define __SYMUTIL_HPP

#include <stdio.h>
#include <string.h>


#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned int
#endif


#define SIZE           0x70


typedef int (*CompareFunc)(void* struct1,void* struct2);


typedef struct _SYMBOL_NODE {
  struct _SYMBOL_NODE* top;
  struct _SYMBOL_NODE* left;
  struct _SYMBOL_NODE* right;
  BYTE   is_left;
  BYTE   is_right;
  BYTE   data[2];
} SYMBOL_NODE;


class SymbolTable {
protected:
  SYMBOL_NODE* base;
  SYMBOL_NODE* curr;
  CompareFunc compare;
  int size;
  int count;
public:
  void* Insert(void* sym);
  void* Find(void* sym);
  void* First(void);
  void* Next(void);
  void* Last(void);
  void* Prev(void);
  void  Purge(void);
  int   Count(void) { return count; }
  int   Size(void) { return size; }
  void* Curr(void) { if (curr==NULL) return NULL; else return curr->data; }

  void* operator << (void* sym) { return Insert(sym); }

  SymbolTable(int _size=SIZE,CompareFunc _compare=(CompareFunc)strcmp);
  ~SymbolTable() { Purge(); }
};   //class SymbolTable


#endif
