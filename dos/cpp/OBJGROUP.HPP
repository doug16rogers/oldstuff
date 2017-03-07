/*****************************************************************************
*
*  TITLE:       Object Group Manager
*
*  DESCRIPTION: This module provides a generic object list manager.
*
*  *k "%n"
*  FILE NAME: ""
*
*  *k "%v"
*  VERSION:   ""
*
*****************************************************************************/

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


//===========================================================================
//
//  The CompareFunc function provided by the instance should return 0 if the
//  items are equal, a negative number if item1 is less than item2, and a
//  positive number if item1 is greater than item2.  Think of it as a
//  subtraction: item1 - item2.  Summarizing:
//
//     result < 0 (-)    ---->    item1 < item2
//     result = 0 (0)    ---->    item1 = item2
//     result > 0 (+)    ---->    item1 > item2
//
//---------------------------------------------------------------------------

typedef int  (*CompareFunc)(    //(- 0 +) if item1 (< = >) item2
  void* item1,                  //first item of comparison
  void* item2);                 //second item of comparison

//===========================================================================
//
//  The ForEachFunc is used to be applied to each item in the list, passing
//  the argument to each the function.
//
//---------------------------------------------------------------------------

typedef void (*ForEachFunc)(void* item, void* argument);


/*****************************************************************************
*
*  TITLE:       Object Group Class
*
*  DESCRIPTION: This class provides a group for a generic object.  You should
*               derive a class from this one to return the type you want.
*
*  REFERENCE:   none.
*
*****************************************************************************/

class Object_Group
{

protected:

#define SIZE           0x70

//===========================================================================
//
//  The following structure is used internally by the class to support the
//  binary tree of items.
//
//---------------------------------------------------------------------------

  typedef struct _SYMBOL_NODE
  {
    struct _SYMBOL_NODE* top;
    struct _SYMBOL_NODE* left;
    struct _SYMBOL_NODE* right;
    BYTE   is_left;
    BYTE   is_right;
    BYTE   data[2];
  } SYMBOL_NODE;


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

  SymbolTable(
    int _size=SIZE,
    CompareFunc _compare=(CompareFunc)strcmp);

  ~SymbolTable() { Purge(); }
};   //class SymbolTable


#endif
