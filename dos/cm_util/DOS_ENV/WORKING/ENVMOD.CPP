//----------------------------------------------------------------------
// File:        envmod.cpp
// Description: Master environment utilities.
//----------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>

#ifndef __LARGE__
#error "must use large memory model"
#endif



//---------------------------------------------------------------------
// Function:    master_env
// Description: Returns a pointer to the master environment.
//              Uses the GETVECT 2Eh technique.
//---------------------------------------------------------------------
char* master_env()
{
  asm mov ax, 0x352e       // get INT 2Eh vector
  asm int 0x21
  asm mov dx, es:[0x002C]  // environment segment
  asm xor ax, ax           // return far ptr in DX:AX
}


//--------------------------------------------------------------------------
// Function:    add_env
// Description: Scans the master environment for the last entry, then
//              adds the passed entry to end of the environment.
//              Returns error status.
// Example:     add_env("TEMP=r:\temp");
//--------------------------------------------------------------------------
int add_env(char* var)
{
  char* cur   = master_env();          // start at master list start
  int   error = 0;
  while (*cur)                         // while not end of vars
  {
    cur += strlen(cur)+1;              // move to next var
  }
  strcpy(cur,var);                     // add var to environment
  cur += strlen(cur)+1;                // set cur to next env entry
  *cur = 0;                            // set last NULL, end of env
  return(error);
}


//--------------------------------------------------------------------------
// Function:    get_env
// Description: Scans the master environment for the passed variable name.
//              A variable match is detected when the charcters in var
//              match the starting characters of an environment string.
//              Returns a pointer to the variable, or NULL if the
//              variable was not found in the master environment.
// Example:     get_env("TEMP=");  Equals on end makes match accurate.
//--------------------------------------------------------------------------
char* get_env(char* var)
{
  int   found = 0;
  char* cur   = master_env();          // start at master list start
  int   var_len = strlen(var);         // length of variable name
  while (!found && *cur)               // while not found and more vars
  {
    if (!strnicmp(cur,var,var_len))    // if var names match
      found = 1;                       // set found flag
    else                               // else no match
      cur += strlen(cur)+1;            // move to next var
  }
  if (!found) cur = 0;                 // return null if not found
  return(cur);
}

//--------------------------------------------------------------------------
// Function:    del_env
// Description: Scans the master environment for the passed variable name.
//              Deletes the variable and compacts the environment if the
//              variable is found.  Returns 0 if no error, >1 if error.
// Example:     del_env("TEMP=");    Equals on end makes match accurate.
//--------------------------------------------------------------------------
int del_env(char* var)
{
  char* cur   = get_env(var);             // find position in environment
  char* next;                             // pointer to next environment var
  if (cur)
  {
    next = cur + strlen(cur)+1;        // next points to next var
    while (*cur)                       // while cur is not last
    {
      strcpy(cur,next);                // overwrite cur with next
    }
  }
  return(0);                      // returns error condition
}


//---------------------------------------------------------------------
// Function:    set_env
// Description: Sets the value of an existing environmental variable.
//              Returns true if successfully set.
// Example:     set_env("TEMP=r:\temp");
//--------------------------------------------------------------------
int set_env(char* var)
{
  char* equal = strchr(var,'=');
  char  first;                          // temp stores first char of value
  if (equal)                            // if equal found
  {
    first  = equal[1];                  // save first cchar of value
    equal[1] = 0;                       // end var name string after = sign
    del_env(var);                       // delete existing var in env
    equal[1] = first;                   // set value back to original
    add_env(var);                       // add variable to environment
  }
  return(equal!=NULL);                  // return true if success
}


int main(int argc, char* argv[])
{
  char* env = master_env();
  printf(" Master environment at %p \n",env);
  if (argc==2)
  {
    set_env(argv[1]);
  }
  else
  {
    printf(" Syntax:  envmod <variable_name=variable_value> \n\n");
  }
}

