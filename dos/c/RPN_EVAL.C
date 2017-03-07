/*-------------------------------------------------------------------------*\
 | Module:      Function Dumper
 | Compiler:    Borland/Turbo C
 | Description: This module contains procedures which assist in the
 |              translation from text and the evaluation of real-valued
 |              functions of a single variable.
\*-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "variable.h"

#include "rpn_eval.h"


/*-------------------------------------------------------------------------*\
 | Data:        function_name_list
 | Description: This data element holds a list of all function names known
 |              to this module.  Functions should be declared like a
 |              MultiFunction, below.
\*-------------------------------------------------------------------------*/

typedef double (*SingleFunction)(double x);
typedef double (*MultiFunction)(double *x);

typedef unsigned long ulong;

double mul_  (double* x) { return x[1] * x[0]; }
double div_  (double* x) { if (x[0] == 0) return 1; else return x[1] / x[0]; }
double add_  (double* x) { return x[1] + x[0]; }
double sub_  (double* x) { return x[1] - x[0]; }
double min_  (double* x) { if (x[1] < x[0]) return x[1]; else return x[0]; }
double max_  (double* x) { if (x[1] > x[0]) return x[1]; else return x[0]; }
double chs_  (double* x) { return -x[0]; }
double and_  (double* x) { return (double) ((ulong)x[1] & (ulong)x[0]); }
double or_   (double* x) { return (double) ((ulong)x[1] | (ulong)x[0]); }
double xor_  (double* x) { return (double) ((ulong)x[1] ^ (ulong)x[0]); }
double neg_  (double* x) { return (double) (~(ulong)x[0]); }

double fabs_ (double* x) { return fabs(x[0]); }
double acos_ (double* x) { return acos(x[0]); }
double asin_ (double* x) { return asin(x[0]); }
double atan_ (double* x) { return atan(x[0]); }
double ceil_ (double* x) { return ceil(x[0]); }
double cos_  (double* x) { return cos(x[0]); }
double cosh_ (double* x) { return cosh(x[0]); }
double exp_  (double* x) { return exp(x[0]); }
double floor_(double* x) { return floor(x[0]); }
double log_  (double* x) { return log(x[0]); }
double log10_(double* x) { return log10(x[0]); }
double sin_  (double* x) { return sin(x[0]); }
double sinh_ (double* x) { return sinh(x[0]); }
double sqrt_ (double* x) { return sqrt(x[0]); }
double tan_  (double* x) { return tan(x[0]); }
double tanh_ (double* x) { return tanh(x[0]); }


typedef struct                 /*functions for calculator...*/
{
  char* name;                  /*text name of function*/
  MultiFunction func;          /*function pointer*/
  int args;                    /*number of arguments (variables) for func*/
} FunctionEntry;

static FunctionEntry function_name_list[] =
{
  { "*",     mul_,   2 },      /*my own additions to math library*/
  { "/",     div_,   2 },
  { "+",     add_,   2 },
  { "-",     sub_,   2 },
  { "min",   min_,   2 },
  { "max",   max_,   2 },
  { "_",     chs_,   1 },
  { "&",     and_,   2 },
  { "|",     or_,    2 },
  { "^",     xor_,   2 },
  { "~",     neg_,   1 },

  { "abs",   fabs_,  1 },      /*from math library*/
  { "acos",  acos_,  1 },
  { "asin",  asin_,  1 },
  { "atan",  atan_,  1 },
  { "ceil",  ceil_,  1 },
  { "cos",   cos_,   1 },
  { "cosh",  cosh_,  1 },
  { "exp",   exp_,   1 },
  { "floor", floor_, 1 },
  { "log",   log_,   1 },
  { "log10", log10_, 1 },
  { "sin",   sin_,   1 },
  { "sinh",  sinh_,  1 },
  { "sqrt",  sqrt_,  1 },
  { "tan",   tan_,   1 },
  { "tanh",  tanh_,  1 },

  { NULL, NULL, 0 }
};   /*function_name_list*/


/*-------------------------------------------------------------------------*\
 | Data:        rpn_stack
 | Description: This is the stack for the RPN calculator.
\*-------------------------------------------------------------------------*/

static double rpn_stack[RPN_STACK_SIZE];


/*-------------------------------------------------------------------------*\
 | Data:        rpn_error_message
 | Description: This string holds the latest error message from the RPN
 |              calculator.
\*-------------------------------------------------------------------------*/

static char rpn_error_message[0x80] = RPN_NO_ERROR;


/*-------------------------------------------------------------------------*\
 | Procedure:   GetFunction
 | Description: Translates a function from its name to an entry in the
 |              function list.  Returns NULL on failure.
\*-------------------------------------------------------------------------*/

FunctionEntry* GetFunction(    /*pointer to function for given name*/
  char* name)                  /*name for which a function is sought*/
{
  FunctionEntry* entry = function_name_list;

  while (entry->name != NULL)
  {
    if (EqualUpToWhite(entry->name, name)) return entry;
    entry++;
  }
  return NULL;
}   /*GetFunction*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PopRPNStack
 | Description: Deletes the top value from the RPN stack by moving the next
 |              values up n notches.  Returns the initial top value.
\*-------------------------------------------------------------------------*/

double PopRPNStack(            /*value on top of stack*/
  int elements_to_pop)         /*number of values to pop off*/
{
  int i;
  double top_value = rpn_stack[0];

  for (i = 0; ((i + elements_to_pop) < RPN_STACK_SIZE); i++)
    rpn_stack[i] = rpn_stack[i+elements_to_pop];

  return top_value;
}   /*PopRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PushRPNStack
 | Description: Pushes a new value onto the top of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void PushRPNStack(             /*no output*/
  double value)                /*value to push onto top of stack*/
{
  int i;

  for (i = (RPN_STACK_SIZE - 1); (i > 0); i--)
    rpn_stack[i] = rpn_stack[i-1];
  rpn_stack[0] = value;
}   /*PushRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   ClearRPNStack
 | Description: Clears (sets to 0.0) each element of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void ClearRPNStack(void)       /*no output*/
{
  int i;

  for (i = 0; (i < RPN_STACK_SIZE); i++)
    rpn_stack[i] = 0.0;
}   /*ClearRPNStack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   SetRPNError
 | Description: Sets the RPN error string to the value provided.  Allows
 |              printf-style arguments.  Returns 0.0 for your convenience.
\*-------------------------------------------------------------------------*/

double SetRPNError(char* format,...)   /*0.0*/
{
  va_list va;                  /*variable argument list -- the beauty of c!*/

  va_start(va,format);
  vsprintf(rpn_error_message,format,va);
  va_end(va);
  return 0.0;
}   /*SetRPNError*/


/*-------------------------------------------------------------------------*\
 | Procedure:   GetRPNError
 | Description: Returns a constant pointer to the latest RPN error setting.
\*-------------------------------------------------------------------------*/

const char* GetRPNError(void)  /*latest RPN calculator error message*/
{
  return rpn_error_message;
}   /*GetRPNError*/


/*-------------------------------------------------------------------------*\
 | Procedure:   EvaulateRPNString
 | Description: Translates and evaluates a string containing functions,
 |              operators and variables.  Returns the final value of the
 |              calculation, which is done in Reverse Polish Notation
 |              (RPN).  All operators must be separated by whitespace.
\*-------------------------------------------------------------------------*/

double EvaluateRPNString(      /*final value after evaluation*/
  char* string)                /*string to use as input*/
{
  int i = 0;                   /*index into string*/
  FunctionEntry* entry = NULL; /*entry for function to execute*/
  double value = 1.0;          /*next value*/

  SetRPNError(RPN_NO_ERROR);
  if (string == NULL) return SetRPNError("cannot evaluate (null) string");

  while ((string[i] != 0) && isspace(string[i])) i++;  /*skip whitespace*/
  while (string[i] != 0)
  {
    entry = GetFunction(&string[i]);
    if (entry != NULL)
    {
      value = entry->func(rpn_stack);
      PopRPNStack(entry->args);
      PushRPNStack(value);
    }
    else
    {
      if (VariableDefined(&string[i]))
        value = GetVariable(&string[i]);
      else if (sscanf(&string[i],"%lG",&value) != 1)
        return SetRPNError("error at: \"%s\"",&string[i]);
      PushRPNStack(value);
    }
    while ((string[i] != 0) && !isspace(string[i])) i++;  /*skip to whitespace*/
    while ((string[i] != 0) && isspace(string[i])) i++;   /*skip whitespace*/
  }   /*while there's stuff left to do*/

  return rpn_stack[0];
}   /*EvaluateRPNString*/

