#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "r250.h"

#define SIZE           250
#define PREV           103

#define LOWORD(x)      (*((WORD*)&(x)))
#define HIWORD(x)      (*(((WORD*)&(x))+1))


typedef struct
{
  int initial_seed;     //seed used to create initial values in R250 array
  int index;            //index of second R250 array element
  DWORD sequence[SIZE]; //sequence of R250 values
} R250;



//===========================================================================
//  R250_Create
//---------------------------------------------------------------------------
//  Initializes the R250 random number generator with the given seed.
//  Passes a handle back to the caller.
//---------------------------------------------------------------------------
R250_GENERATOR R250_Create(     //object handle
  int seed)                     //seed -- allows repeatable sequences
{
  int i;
  DWORD bit_to_set = 1;
  DWORD temporary_value;
  R250* r250;

  r250 = (R250*) malloc(sizeof(R250));

  if (r250 != NULL)
  {
    r250->initial_seed = seed;
    srand(seed);

      //---------------------------------------------------------------------
      //  Put initial random numbers into sequence, using standard
      //  (but not very random) C random number generator.
      //---------------------------------------------------------------------

    for (i = 0; i < SIZE; i++)
    {
      HIWORD(temporary_value) = (rand() << 1) ^ rand(); //MSW
      LOWORD(temporary_value) = (rand() << 1) ^ rand(); //LSW
      r250->sequence[i] = temporary_value;
    }

      //---------------------------------------------------------------------
      //  Make sure there is a basis -- that is, that all bits will
      //  change during generation of random numbers.
      //---------------------------------------------------------------------

    bit_to_set = 1;

    for (i = 0; i < 32; i++)
    {
      r250->sequence[i]   |=  bit_to_set;       //this word has the bit set,
      r250->sequence[i+1] &= ~bit_to_set;       //the next one has it reset
      bit_to_set <<= 1;
    }   //for basis-ensuring

    r250->index = 0;
  }

  return (R250_GENERATOR) r250;

}   //R250_Create


//===========================================================================
//  R250_Destroy
//---------------------------------------------------------------------------
//  Destroys the given R250 random number generator.
//---------------------------------------------------------------------------
void R250_Destroy(
  R250_GENERATOR generator)     //handle of the generator to destroy
{
  R250* r250 = (R250*) generator;

  if (r250 != NULL)
  {
    free(r250);
  }
}   //R250_Destroy


//===========================================================================
//  R250_Random_Word
//---------------------------------------------------------------------------
//  Returns a random 32-bit value.
//---------------------------------------------------------------------------
DWORD R250_Random_Word(         //random on [0,0xFFFFFFFF]
  R250_GENERATOR generator)     //the generator to use
{
  R250* r250 = (R250*) generator;
  int index;
  DWORD value;

  index = r250->index;

  if (index >= PREV)
  {
    value = r250->sequence[index] ^ r250->sequence[index - PREV];
  }
  else
  {
    value = r250->sequence[index] ^ r250->sequence[index + (SIZE - PREV)];
  }

  r250->sequence[index] = value;

  index++;
  if (index >= SIZE) index = 0;

  r250->index = index;

  return value;

}   //R250_Random_Word


//===========================================================================
//  R250_Normal_Random
//---------------------------------------------------------------------------
//  Returns a normally (Guassian) distributed random number of type double
//  with the specified mean and standard deviation.
//---------------------------------------------------------------------------
double R250_Normal_Random(      //the normally distributed random number
  R250_GENERATOR generator,     //the generator to use
  double mean,                  //the mu of the normal distribution
  double standard_deviation)    //the sigma of the normal distribution
{
    //-----------------------------------------------------------------------
    //  This algorithm is taken from "Seminumerical Algorithms",
    //  Volume 2 of "The Art of Computer Programming", second edition,
    //  page 117, by Donald Knuth.
    //-----------------------------------------------------------------------

  double uniform_1, uniform_2;  //uniformly distributed random numbers
  double sum_of_squares;
  double unit_normal;

  do
  {
    uniform_1 = R250_Random_On_Interval(generator, -1.0, +1.0);
    uniform_2 = R250_Random_On_Interval(generator, -1.0, +1.0);
    sum_of_squares = (uniform_1 * uniform_1) + (uniform_2 * uniform_2);
  } while (sum_of_squares >= 1.0);

  unit_normal = uniform_1 *     //could also use (and return) uniform_2
                     sqrt(-2.0 * log(sum_of_squares) / sum_of_squares);

  return (standard_deviation * unit_normal) + mean;

}   //R250_Normal_Random


//===========================================================================
//  R250_Random_To
//---------------------------------------------------------------------------
//  Returns a random 16-bit value from 0 to topval-1.
//---------------------------------------------------------------------------
WORD  R250_Random_To(
  R250_GENERATOR generator,     //the generator to use
  WORD topval)                  //returns random on [0,topval-1]
{
  WORD value;

  if (topval > 1)
  {
    value = ((WORD) R250_Random_Word(generator)) % topval;
  }
  else
  {
    value = ((WORD) R250_Random_Word(generator)) & 1;
  }

  return value;

}   //R250_Random_To


//===========================================================================
//  R250_Random_On_Unit_Interval
//---------------------------------------------------------------------------
//  Returns a random double value on [0.0 .. 1.0].
//---------------------------------------------------------------------------
double R250_Random_On_Unit_Interval(    //random on [0.0,1.0], double
  R250_GENERATOR generator)             //the generator to use
{

  return ((double) (DWORD) R250_Random_Word(generator)) /
         ((double) 0xFFFFFFFFUL);

}   //R250_Random_On_Unit_Interval


//===========================================================================
//  R250_Random_On_Interval
//---------------------------------------------------------------------------
//  Returns a uniform random number of type double on the specified
//  interval.
//---------------------------------------------------------------------------
double R250_Random_On_Interval( //double precision float on interval
  R250_GENERATOR generator,     //the generator to use
  double lower_bound,           //lower bound of interval
  double upper_bound)           //upper bound of interval
{
  double range = (upper_bound - lower_bound);

  return lower_bound + (range * R250_Random_On_Unit_Interval(generator));

}   //R250_Random_On_Interval



#if 0

//===========================================================================
//  main
//---------------------------------------------------------------------------
//  Runs a test program to generate normally-distributed random numbers.
//---------------------------------------------------------------------------

#include <stdio.h>

int main(                       //DOS return/exit code, 0 = success
  int argument_count,           //count of commandline arguments
  char* argument_list[])        //list of commandline arguments
{
  unsigned short i;
  unsigned short count = 100;
  double mean = 0.0;
  double standard_deviation = 1.0;
  R250_GENERATOR generator;

  printf
  (
    "Provides random numbers in a normal distribution.\n"
    "Usage: r250 [mean] [standard_deviation] [count]\n\n"
  );

  if (argument_count > 1)
  {
    if (sscanf(argument_list[1], "%lg", &mean) != 1)
    {
      printf("bad mean \"%s\"\n", argument_list[1]);
      return 1;
    }
  }

  if (argument_count > 2)
  {
    if (sscanf(argument_list[2], "%lg", &standard_deviation) != 1)
    {
      printf("bad standard deviation \"%s\"\n", argument_list[2]);
      return 1;
    }
  }

  if (argument_count > 3)
  {
    if (sscanf(argument_list[3], "%u", &count) != 1)
    {
      printf("bad count \"%s\"\n", argument_list[3]);
      return 1;
    }
  }

  generator = R250_Create(0);
  if (generator == NULL)
  {
    printf("couldn't allocate generator\n");
    return 2;
  }

  for (i = 0; i < count; i++)
  {
    printf("%04u: %12.6lf\n", i,
           R250_Normal_Random(generator, mean, standard_deviation));
  }

  R250_Destroy(generator);

  return 0;

}   //main



#endif