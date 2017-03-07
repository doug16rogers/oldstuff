#include "vecutil.hpp"


void vecerror(char* s) { cout <<"RUNTIME ERROR: "<<s<<'\n'; exit(1); }

static Vec _rtnVec(40);                //for calcs


//---------------------------------------------------------------------------
Vec::Vec()                             //constructor
{
  siz=0;                               //start with none
  x=new double[DEF_SIZ];               //get memory for vector
  loadcoord=0;                         //currently loading first coord
  if (!x) return;                      //oops! no mem
  siz=DEF_SIZ;                         //set size
  Fill();                              //fill'er up wid nils
}   //Vec::Vec(int)


//---------------------------------------------------------------------------
Vec::Vec(int n)                        //constructor
{
  if (n<=0) n=DEF_SIZ;                 //bad sizension, use 3
  siz=0;                               //start with none
  x=new double[n];                     //get memory for vector
  loadcoord=0;                         //currently loading first coord
  if (!x) return;                      //oops! no mem
  siz=n;                               //set size
  Fill();                              //fill'er up wid nils
}   //Vec::Vec(int)


//---------------------------------------------------------------------------
Vec::~Vec()                            //destructor
{
  if (x) delete x;                     //kill the coords
}   //Vec::~Vec(void)


//---------------------------------------------------------------------------
void Vec::Fill(double val) {           //fill with value
  register int i;                      //index register
  for (i=0;i<siz;i++) x[i]=val;        //store it
  loadcoord=0;                         //currently loading first coord
}   //Vec::Fill


//---------------------------------------------------------------------------
void Vec::Show(void)                   //display as text
{
  cout <<'('<<x[0];
  for (int i=1;i<siz;i++) cout <<','<<x[i];
  cout <<')';
}   //Vec::Show(void)


//---------------------------------------------------------------------------
void Vec::operator =(Vec& v)           //assignment
{
  int m=MIN(siz,v.siz);                //check size
  for (int i=0;i<m;i++) x[i]=v.x[i];   //copy each one that fits
  for (;i<siz;i++) x[i]=0.0;           //fill the rest with zeros
  loadcoord=0;                         //currently loading first coord
}   //Vec::=(Vec&)


//---------------------------------------------------------------------------
void Vec::operator =(double a)         //assignment, fill with double
{
  Fill(a);                             //fill'er up
  loadcoord=0;                         //currently loading first coord
}   //Vec::=(double)


//---------------------------------------------------------------------------
Vec& Vec::operator +=(Vec& v)          //add to
{
  int m=MIN(siz,v.siz);                //find number to copy
  for (int i=0;i<m;i++) x[i]+=v.x[i];  //add to
  return *this;
}   //Vec::+=(Vec&)


//---------------------------------------------------------------------------
Vec& Vec::operator -=(Vec& v)          //subtract from
{
  int m=MIN(siz,v.siz);                //find number to copy
  for (int i=0;i<m;i++) x[i]-=v.x[i];  //subtract from
  return *this;
}   //Vec::+=(Vec&)


//---------------------------------------------------------------------------
Vec& Vec::operator *=(double a)        //scalar product
{
  for (int i=0;i<siz;i++) x[i]*=a;     //multiply through
  loadcoord=0;                         //currently loading first coord
  return *this;
}   //Vec::*=(double a)


//---------------------------------------------------------------------------
Vec& Vec::operator /=(double a)        //scalar quotient
{
  loadcoord=0;                         //currently loading first coord
  if (a==0.0) return *this;            //can't divide by 0
  for (int i=0;i<siz;i++) x[i]/=a;     //multiply through
  return *this;
}   //Vec::/=(double a)


//---------------------------------------------------------------------------
double Vec::operator !(void)           //scalar length
{
  double dot=0.0;
  for (int i=0;i<siz;i++) dot += x[i]*x[i];   //get dot product
  return sqrt(dot);                    //return square root of dot product
}   //!(Vec&)


//---------------------------------------------------------------------------
Vec& Vec::operator <<(double a)
{
  if (loadcoord>=siz) loadcoord=0;
  x[loadcoord++]=a;
  return *this;
}   //Vec::<<(double)


//---------------------------------------------------------------------------
Vec operator +(Vec& u,Vec& v)          //vector addition
{
  int i;

  _rtnVec.siz=MAX(u.siz,v.siz);
  if (u.siz<v.siz) {                            //if u is smaller..
    for (i=0;i<u.siz;i++) _rtnVec.x[i]=u.x[i]+v.x[i]; //..copy up to u's coords
    for (;i<_rtnVec.siz;i++) _rtnVec.x[i]=v.x[i];           //..finish with v's
  }
  else {                                        //otherwise v is smaller
    for (i=0;i<v.siz;i++) _rtnVec.x[i]=u.x[i]+v.x[i]; //..copy up to v's coords
    for (;i<_rtnVec.siz;i++) _rtnVec.x[i]=u.x[i];           //..finish with u's
  }
  return _rtnVec;
}   //Vec +(Vec&,Vec&)


//---------------------------------------------------------------------------
Vec operator -(Vec& u,Vec& v)          //vector addition
{
  int i;

  _rtnVec.siz=MAX(u.siz,v.siz);
  if (u.siz<v.siz) {                            //if u is smaller..
    for (i=0;i<u.siz;i++) _rtnVec.x[i]=u.x[i]-v.x[i]; //..copy up to u's coords
    for (;i<_rtnVec.siz;i++) _rtnVec.x[i]=-v.x[i];    //..finish with v's
  }
  else {                                        //otherwise v is smaller
    for (i=0;i<v.siz;i++) _rtnVec.x[i]=u.x[i]-v.x[i]; //..copy up to v's coords
    for (;i<_rtnVec.siz;i++) _rtnVec.x[i]=u.x[i];     //..finish with u's
  }
  return _rtnVec;
}   //Vec -(Vec&,Vec&)


//---------------------------------------------------------------------------
double operator *(Vec& u,Vec& v)       //inner (dot) product
{
  double dot=0.0;                      //dot product
  int m=MAX(u.siz,v.siz);              //get smallest sizension
  for (int i=0;i<m;i++) dot += u.x[i]*v.x[i];  //calculate dot product
  return dot;
}   //double *(Vec&,Vec&)


//---------------------------------------------------------------------------
Vec operator *(Vec& v,double a)        //scalar product
{
  _rtnVec.siz=v.siz;
  _rtnVec=v;                           //copy it
  _rtnVec*=a;                          //scalar product
  return _rtnVec;                      //give it to 'em
}   //Vec *(Vec&,double)


//---------------------------------------------------------------------------
double operator >>(Vec& u,Vec& v)      //angle between vectors, radians
{
  double dot=u*v;                      //dot product of u,v
  double times=!u*!v;                  //product of lengths
  if (times==0.0) return M_PI_2;       //pi/2 if they're orthogonal
  return acos(dot/times);              //give'm the number
}   //double *(Vec&,Vec&)


//---------------------------------------------------------------------------
ostream& operator<<(ostream& out,Vec& v)  //for cout use
{
  out << '(' << v.x[0];
  for (int i=1;i<v.siz;i++) out <<','<<v.x[i];
  out << ')';
  return out;
}   //operator<<(ostream& out,Vec& v)

