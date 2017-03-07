@echo off
rem -------------------------------------------------------------------------
rem --
rem --  TITLE:        Build Module
rem --
rem --  DESCRIPTION:  The batch procedure "BuildMod"
rem --                builds the given utility module by
rem --                compiling each element of it, then
rem --                adding each element to the given
rem --                library.
rem --
rem --  *k"%n"
rem --  FILE NAME:    ""
rem --
rem --  *k"%v"
rem --  VERSION:      ""
rem --
rem --  REFERENCE:    None
rem --
rem -------------------------------------------------------------------------

if %1$==$ goto no_mod

   if not exist %1\working\*.h goto no_head
      copy %1\working\*.h
   :no_head

   if not exist %1\working\*.cpp goto no_cpp
      k:\bc\bin\bcc @turboc.cfg -c -n%1\obj %1\working\*.cpp
      if errorlevel 1 goto error
   :no_cpp

   if not exist %1\working\*.c goto no_c
      k:\bc\bin\bcc @turboc.cfg -c -n%1\obj %1\working\*.c
      if errorlevel 1 goto error
   :no_c

   if not exist %1\working\*.asm goto no_asm
      k:\bc\bin\bcc @turboc.cfg -c -n%1\obj %1\working\*.asm
      if errorlevel 1 goto error
   :no_asm

   w:\rogers\util\dr -T- -S$ -fk:\bc\bin\tlib$/C$%1$-+%%p%%f %1\obj\*.obj > build.bat
   echo on
   call build.bat
   @echo off
   k:\bc\bin\tlib %1, %1.lst

   goto end

:no_mod
   echo No module (CM library) specified.
   goto end

:error
   echo ---------------------------------------------------------------------
   echo --  Errors found during build.
   echo --  Halting.
   echo ---------------------------------------------------------------------
   goto end

:end
