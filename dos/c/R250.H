#ifndef __R250_H
#define __R250_H


#ifndef WORD
  typedef unsigned int WORD;
#endif

#ifndef DWORD
  typedef unsigned long int DWORD;
#endif


typedef void* R250_GENERATOR;   //handle, so no inner workings are exposed


//===========================================================================
//  R250_Create
//---------------------------------------------------------------------------
//  Initializes the R250 random number generator with the given seed.
//  Passes a handle back to the caller.
//---------------------------------------------------------------------------
R250_GENERATOR R250_Create(     //object handle
  int seed);                    //seed -- allows repeatable sequences


//===========================================================================
//  R250_Destroy
//---------------------------------------------------------------------------
//  Destroys the given R250 random number generator.
//---------------------------------------------------------------------------
void R250_Destroy(
  R250_GENERATOR generator);    //handle of the generator to destroy


//===========================================================================
//  R250_Normal_Random
//---------------------------------------------------------------------------
//  Returns a normally (Guassian) distributed random number of type double
//  with the specified mean and standard deviation.
//---------------------------------------------------------------------------
double R250_Normal_Random(      //the normally distributed random number
  R250_GENERATOR generator,     //the generator to use
  double mean,                  //the mu of the normal distribution
  double standard_deviation);   //the sigma of the normal distribution


//===========================================================================
//  R250_Random_Word
//---------------------------------------------------------------------------
//  Returns a random 32-bit value.
//---------------------------------------------------------------------------
DWORD R250_Random_Word(         //random on [0,0xFFFFFFFF]
  R250_GENERATOR generator);    //the generator to use


//===========================================================================
//  R250_Random_To
//---------------------------------------------------------------------------
//  Returns a random 16-bit value from 0 to topval-1.
//---------------------------------------------------------------------------
WORD  R250_Random_To(
  R250_GENERATOR generator,     //the generator to use
  WORD topval);                 //returns random on [0,topval-1]


//===========================================================================
//  R250_Random_On_Unit_Interval
//---------------------------------------------------------------------------
//  Returns a random double value on [0.0 .. 1.0].
//---------------------------------------------------------------------------
double R250_Random_On_Unit_Interval(    //random on [0.0,1.0], double
  R250_GENERATOR generator);            //the generator to use


//===========================================================================
//  R250_Random_On_Interval
//---------------------------------------------------------------------------
//  Returns a uniform random number of type double on the specified
//  interval.
//---------------------------------------------------------------------------
double R250_Random_On_Interval( //double precision float on interval
  R250_GENERATOR generator,     //the generator to use
  double lower_bound,           //lower bound of interval
  double upper_bound);          //upper bound of interval


#endif
