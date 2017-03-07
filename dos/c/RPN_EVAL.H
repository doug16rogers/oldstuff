#ifndef __RPN_EVAL_H
#define __RPN_EVAL_H


#ifndef RPN_STACK_SIZE
#define RPN_STACK_SIZE 16
#endif

#define RPN_NO_ERROR   "no error"


/*-------------------------------------------------------------------------*\
 | Procedure:   PopRPNStack
 | Description: Deletes the top value from the RPN stack by moving the next
 |              values up n notches.  Returns the initial top value.
\*-------------------------------------------------------------------------*/

double PopRPNStack(            /*value on top of stack*/
  int elements_to_pop);        /*number of values to pop off*/


/*-------------------------------------------------------------------------*\
 | Procedure:   PushRPNStack
 | Description: Pushes a new value onto the top of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void PushRPNStack(             /*no output*/
  double value);               /*value to push onto top of stack*/


/*-------------------------------------------------------------------------*\
 | Procedure:   ClearRPNStack
 | Description: Clears (sets to 0.0) each element of the RPN calculator's
 |              stack.
\*-------------------------------------------------------------------------*/

void ClearRPNStack(void);      /*no output*/


/*-------------------------------------------------------------------------*\
 | Procedure:   SetRPNError
 | Description: Sets the RPN error string to the value provided.  Allows
 |              printf-style arguments.  Returns 0.0 for your convenience.
\*-------------------------------------------------------------------------*/

double SetRPNError(char* format,...);  /*0.0*/


/*-------------------------------------------------------------------------*\
 | Procedure:   GetRPNError
 | Description: Returns a constant pointer to the latest RPN error setting.
\*-------------------------------------------------------------------------*/

const char* GetRPNError(void); /*latest RPN calculator error message*/


/*-------------------------------------------------------------------------*\
 | Procedure:   EvaulateRPNString
 | Description: Translates and evaluates a string containing functions,
 |              operators and variables.  Returns the final value of the
 |              calculation, which is done in Reverse Polish Notation
 |              (RPN).  All operators must be separated by whitespace.
\*-------------------------------------------------------------------------*/

double EvaluateRPNString(      /*final value after evaluation*/
  char* string);               /*string to use as input*/


#endif
