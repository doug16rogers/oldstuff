#ifndef __templist_hpp
#define __templist_hpp

/*****************************************************************************
*
*  TITLE:        Template Lists
*
*  DESCRIPTION:  This module contains template classes for generic lists.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

#include <string.h>


typedef enum
{
  going_backwards = 0,
  going_forwards = 1,
} DIRECTION_FOR_EACH;

/*****************************************************************************
*
*  TITLE:        Copy Of String
*
*  DESCRIPTION:  The function "Copy_Of_String" allocates a copy of the given
*                NUL-terminated string onto the heap and returns a pointer to
*                it.
*
*  REFERENCE:    None.
*
*****************************************************************************/

inline char* Copy_Of_String(char* text)
{
  char* copy = NULL;

  if (text != NULL)
  {
    copy = new char[strlen(text)+1];
    if (copy != NULL) strcpy(copy, text);
  }
  return copy;
}   //Copy_Of_String


/*****************************************************************************
*
*  TITLE:        List Class
*
*  DESCRIPTION:  The class "List" is a generic list of items.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

template <class T>
class List
{

protected:

  int count;
  int size;
  int delta;

  T* base;

  /***************************************************************************
  *
  *  TITLE:        List:: Setup
  *
  *  DESCRIPTION:  The procedure "List:: Setup" initializes the list to the
  *                given initial size.
  *
  *  REFERENCE:    None.
  *
  ***************************************************************************/

protected:

  void Setup(int first, int after)
  {
    count = 0;
    size = first;
    if (size > 0)
    {
      base = new T[size];
    }
    else
    {
      base = NULL;
    }
    delta = after;
  }

  /***************************************************************************
  *
  *  TITLE:        List:: List
  *
  *  DESCRIPTION:  The constructor "List:: List" initializes the list to the
  *                given initial size and delta.
  *
  *  REFERENCE:    None.
  *
  ***************************************************************************/

public:

  List(int first, int after)
  {
    Setup(first, after);
  }

  /***************************************************************************
  *
  *  TITLE:        List:: List
  *
  *  DESCRIPTION:  The copy constructor "List:: List" copies the elements of
  *                the given list.
  *
  *  REFERENCE:    None.
  *
  ***************************************************************************/

public:

  List(const List<T>& list);

  /***************************************************************************
  *
  *  TITLE:        List:: List
  *
  *  DESCRIPTION:  The default constructor "List:: List" initializes the list
  *                to be 10 items long with a delta of 10.
  *
  *  REFERENCE:    None.
  *
  ***************************************************************************/

public:

  List()
  {
    Setup(10, 10);
  }

  /***************************************************************************
  *
  *  TITLE:        List:: ~List
  *
  *  DESCRIPTION:  The destructor "List:: ~List" destroys the given list.
  *
  *  REFERENCE:    None.
  *
  ***************************************************************************/

public:

  virtual ~List()
  {
    if (base != NULL) delete base;
  }


public:

  void operator = (const List<T>& list);        //copy assignment

public:

  virtual T At(int index)
  {
    return base[index];
  }

public:

  virtual void Insert_At(int index, T value);

public:

  virtual void Remove_At(int index);

public:

  virtual void For_Each(void (*function)(T object, void* argument),
                        void* argument,
                        DIRECTION_FOR_EACH direction = going_forwards);

public:

  virtual void Insert(T value)
  {
    Insert_At(count, value);
  }

public:

  virtual void Remove(void)
  {
    if (count > 0) count--;
  }

public:

  virtual void Remove_All(void)
  {
    count = 0;
  }

public:

  virtual int Count(void)
  {
    return count;
  }

public:

  virtual int Size(void)
  {
    return size;
  }

};   //template class List



  template <class T>
  List<T>::List(const List<T>& list)    //copy constructor
  {
    base = new T[list.size];

    if (base == NULL)
    {
      size = 0;
      count = 0;
      delta = 0;
    }
    else
    {
      size = list.size;
      count = list.count;
      delta = list.delta;

      for (int i = 0; i < count; i++)
        base[i] = list.base[i];
    }
  }

  template <class T>
  void List<T>:: operator = (const List<T>& list)    //copy assignment
  {
    base = new T[list.size];

    if (base != NULL)
    {
      size = list.size;
      count = list.count;
      delta = list.delta;

      for (int i = 0; i < count; i++)
        base[i] = list.base[i];
    }
  }

    //
    //  Call For_Each(Kill, NULL) to delete each object in the list..
    //

  template <class T>
  void Kill(T object, void* arg)
  {
    delete object;
  }



  template <class T>
  void List<T>:: For_Each(void (*function)(T object, void* argument),
                          void* argument,
                          DIRECTION_FOR_EACH direction)
  {
    int i;

    if (direction == going_forwards)
    {
      for (i = 0; i < count; i++)
        function(base[i], argument);
    }
    else
    {
      for (i = count - 1; i >= 0; i--)
        function(base[i], argument);
    }
  }   //List<T>:: For_Each

  template <class T>
  void List<T>::Insert_At(int index, T value)
  {
    if ((index > count) || (index < 0)) return;

    if ((count == size) && (delta > 0))
    {
      T* new_base = new T[size + delta];
      if (new_base != NULL)
      {
        memcpy(new_base, base, sizeof(T) * size);
        size += delta;
        delete base;
        base = new_base;
      }
    }
    if (count < size)
    {
      for (int i = count; i > index; i--)
      {
        base[i] = base[i-1];
      }
      base[index] = value;
      count++;
    }
  }   //List<T>:: Insert_At

  template <class T>
  void List<T>:: Remove_At(int index)
  {
    if ((index < 0) || (index >= count)) return;

    if ((count > 0) && (index < count))
    {
      count--;
      for (int i = index; i < count; i++)
      {
        base[i] = base[i+1];
      }
    }
  }   //List<T>:: Remove_At


/*****************************************************************************
*
*  TITLE:        Pointer List Class
*
*  DESCRIPTION:  The class "Pointer_List" is a list of pointers to items.
*                The items are deleted when the list is destroyed.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

template <class T>
class Pointer_List : public List<T*>
{
public:
  Pointer_List(int first, int after) :
      List<T*>(first, after)
  {
  }

  Pointer_List(void) :
      List<T*>()
  {
  }

  virtual void Purge(void);

  ~Pointer_List()
  {
    Purge();
    List<T*>::~List<T*>();
  }

};   //template class Pointer_List


  template <class T>
  void Pointer_List<T>::Purge(void)
  {
    for (int i = 0; i < count; i++)
    {
      delete base[i];
    }
    count = 0;
  }


/*****************************************************************************
*
*  TITLE:        Sorted List Class
*
*  DESCRIPTION:  The class "Sorted_List" is a generic list of items.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    None.
*
*****************************************************************************/

template <class T>
class Sorted_List : public List<T>
{
protected:
  char duplicates;

public:
  Sorted_List(int first, int after, int _duplicates = 0) :
      List<T>(first, after)
  {
    duplicates = _duplicates;
  }

  Sorted_List(int _duplicates = 0) :
      List<T>()
  {
    duplicates = _duplicates;
  }

  ~Sorted_List()
  {
//    List<T>::~List<T>();
  }

  virtual int Compare(T object1, T object2);  //you must provide one!

  virtual void Insert(T object);

  virtual void Remove(T object);

  virtual int Search(T object, int& index);

};   //template class Pointer_Sorted_List

  template <class T>
  void Sorted_List<T>::Insert(T object)
  {
    int place;
    if (Search(object, place))
    {
      if (duplicates)
      {
        Insert_At(place+1, object);
      }
    }
    else
    {
      Insert_At(place, object);
    }
  }

  template <class T>
  void Sorted_List<T>::Remove(T object)
  {
    int place;
    if (Search(object, place))
    {
      Remove_At(place);
    }
  }

  template <class T>
  int Sorted_List<T>::Search(T object, int& index)
  {
    index = 0;
    if (count == 0) return 0;

    int bottom = 0;
    int middle = count / 2;
    int top = count;
    int result;

    do
    {
      result = Compare(object, base[middle]);

      if (result == 0) break;

      if (result > 0)   //object > middle object
      {
        bottom = middle + 1;
      }
      else
      {
        top = middle;
      }
      middle = (bottom + top) / 2;
    } while (bottom < top);

    index = middle;
    return (result == 0);
  }


//
//  // Sample: List of strings and sorted list of strings...
//
//
//  typedef List<char*> String_List;
//
//  typedef Sorted_List<char*> Sorted_String_List;
//
//
//
//  int Sorted_String_List:: Compare(char* obj1, char* obj2)
//  {
//    return strcmp(obj1, obj2);
//  }
//

#endif
