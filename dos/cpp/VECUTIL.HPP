#include <iostream.h>
#include <stdlib.h>
#include <math.h>


void vecerror(char* s);


int inline MIN(int x,int y) { if (x<y) return x; else return y; }
int inline MAX(int x,int y) { if (x>y) return x; else return y; }
double inline MIN(double x,double y) { if (x<y) return x; else return y; }
double inline MAX(double x,double y) { if (x>y) return x; else return y; }


//===========================================================================
class Vec {                            //multi-sizensional point
private:                               //enforce privacy
  int siz;                             //number of sizensions
  double* x;                           //coordinates
  int loadcoord;                       //currently loading coordinates
public:                                //this is the user portion
#define DEF_SIZ 3
  Vec();                               //constructor, 3d
  Vec(int n);                          //allows more than 3d
  ~Vec();                              //destructor
  void Fill(double val=0.0);           //fill with value
  void Show(void);                     //write out
  double& operator [](int i) {         //element of
    if ((i<0)||(i>=siz)) vecerror("vector index range");  //check index
    return x[i];                       //giv'm da val
  }   //Vec::[](double)
  void operator =(Vec& v);             //assignment operator
  void operator =(double a);           //assignment operator, fill
  Vec& operator +=(Vec& v);            //add to
  Vec& operator -=(Vec& v);            //subtract from
  Vec& operator *=(double a);          //scalar product
  Vec& operator /=(double a);          //scalar quotient
  Vec& operator <<(double a);          //put values into vector
  double operator !(void);             //scalar length
  friend Vec operator +(Vec& u,Vec& v);      //vector addition
  friend Vec operator -(Vec& u,Vec& v);      //vector subtraction
  friend double operator *(Vec& u,Vec& v);   //inner (dot) product
  friend Vec operator *(Vec& u,double a);    //scalar product
  friend Vec operator *(double a,Vec& u) { return u*a; }  //scalar product
  friend double operator >>(Vec& u,Vec& v);  //get angle between vectors

  friend ostream& operator<<(ostream& out,Vec& v);  //for cout use
};   //class Vec


