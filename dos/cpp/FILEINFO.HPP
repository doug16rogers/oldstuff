#ifndef __fileinfo_hpp
#define __fileinfo_hpp
/*****************************************************************************
*
*  TITLE:        File Information
*
*  DESCRIPTION:  The module "FileInfo"
*                provides a class for getting and setting
*                information about files.
*
*  *k "%n"
*  FILE NAME:    ""
*
*  *k "%v"
*  VERSION:      ""
*
*  REFERENCE:    r
*
*****************************************************************************/
#include <stdio.h>
#include <dir.h>
#include <dos.h>

#include "date.hpp"
#include "icitypes.h"
#include "timeoday.hpp"


/*****************************************************************************
*
*  TITLE:        File Information
*
*  DESCRIPTION:  The class "File_Info"
*                provides data and methods for access
*                to operating system file attributes, such
*                as name, size, etc.
*
*  REFERENCE:    r
*
*****************************************************************************/

class File_Info
{
public:

   char name[10];
   char extension[4];
   char* path;
   long size;
   Date date;
   Time_Of_Day time;
   UINT8 attribute;

public:

   void Extract_Name_And_Extension(
      const char* from_string);

public:

   File_Info(void)
   {
      name[0] = 0;
      extension[0] = 0;
      path = NULL;
      attribute = 0;
      size = 0L;
   }

public:

   File_Info(
      char* path,
      struct ffblk* file_block)
   {
      this->path = path;
      this->name[0] = 0;
      this->extension[0] = 0;
      Load(file_block);
   }

public:

   File_Info(
     char* path,
     char* name)
   {
      this->path = path;
      Extract_Name_And_Extension(name);
      Update_Stats();
   }

public:

   ~File_Info()
   {
   }

public:

   void Load(

      struct ffblk* file_block);

public:

   const char* Name_And_Extension(void);

public:

   void Update_Stats(

      UINT8 only_attributes = (FA_ARCH | FA_HIDDEN | FA_RDONLY));

};   // class File_Info


#endif

