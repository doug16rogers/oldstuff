#include <iostream.h>
#include <iomanip.h>
#include <math.h>
#include <values.h>


typedef struct { double x; double fx; } double_point;
typedef double (*realfunc)(double x);          //func of one variable
typedef double (*arrayfunc)(double_point p[],int n); //func of one var


double zero(realfunc f,realfunc f_,double x)   //returns zero of f
{
  double x2=MAXDOUBLE;
  double x1=MAXDOUBLE;
  double fx;

  while (1) {
    x2=x1;
    x1=x;
    fx=f(x);
    x=x-fx/f_(x);
    if ((x==x1)||(x==x2)) break;
  }
  return x;
}   //zero

double inline _fabs(double x) { if (x<0.0) return -x; else return x; }


double derivL(realfunc f,double x,double h)
{
  double nh=h;
  double f2_x=MAXDOUBLE;
  double f1_x=MAXDOUBLE/3.0;
  double fx=f(x);
  double f_x=(fx-f(x-nh))/nh;
  while (_fabs(f_x-f1_x)<_fabs(f1_x-f2_x)) {
    f2_x=f1_x;
    f1_x=f_x;
    nh=nh/2.0;
    if ((x-nh)==x) break;
    f_x=(fx-f(x-nh))/nh;
  }   //while
  return f1_x;
}   //derivL


double derivR(realfunc f,double x,double h)
{
  double nh=h;
  double f2_x=MAXDOUBLE;
  double f1_x=MAXDOUBLE/3.0;
  double fx=f(x);
  double f_x=(f(x+nh)-fx)/nh;
  while (_fabs(f_x-f1_x)<_fabs(f1_x-f2_x)) {
    f2_x=f1_x;
    f1_x=f_x;
    nh=nh/2.0;
    if ((x+nh)==x) break;
    f_x=(f(x+nh)-fx)/nh;
  }   //while
  return f1_x;
}   //derivR


double derivM(realfunc f,double x,double h)
{
  double f2_x=MAXDOUBLE;
  double f1_x=MAXDOUBLE/3.0;
  double f_x=(f(x+h)-f(x-h))/(h+h);
  while (_fabs(f_x-f1_x)<_fabs(f1_x-f2_x)) {
    f2_x=f1_x;
    f1_x=f_x;
    h /= 2.0;
    if ((x+h)==(x-h)) break;
    f_x=(f(x+h)-f(x-h))/(h+h);
  }   //while
  return f1_x;
}   //derivM


void derivpoints(realfunc f,double_point f_[],int n,double x0,double xnm1)
{
  double h=(xnm1-x0)/(double)n;
  f_[0].x=x0;
  f_[n-1].x=xnm1;
  for (int i=1;i<(n-1);i++) {
    f_[i].x=f_[i-1].x+h;
  }
  f_[0].fx=derivR(f,f_[0].x,h);
  for (i=1;i<n;i++) {
    f_[i].fx=derivL(f,f_[i].x,h);
  }
}   //derivpoints


void sortpoints(double_point p[],int n)
{
  int i=0;
  double_point dp;
  n--;
  while (i<n) {
    if (p[i].x>p[i+1].x) {
      dp=p[i+1];
      p[i+1]=p[i];
      p[i]=dp;
      if (i) i--;
    }
    else {
      i++;
    }
  }
}   //sortpoints


double interpolate(double_point p[],int n,double x);




void main(void)
{
  cout << "--------------------------------------------------------------\n";

  cout.setf(ios::left, ios::adjustfield);
  cout.setf(ios::showpoint, ios::showpoint);
  cout << setprecision(10) << setfill('0');

  realfunc f = atan;
  cout << "f(x) = atan(x)\n";

#define START  0.0
#define END    9.0 //(M_PI/3)
#define STEP   ((END-START)/40)

  cout << "-----x------  ----f(x)----  -calc f'(x)-  -real f'(x)-  ----error---\n";
  for (double x = START; (x <= END); x += STEP)
  {
    double cdfx = derivM(f,x,STEP/2);
    double adfx = (1.0/(x*x+1.0));
    cout         << setw(12) << x
         << "  " << setw(12) << f(x)
         << "  " << setw(12) << cdfx
         << "  " << setw(12) << adfx
         << "  " << setw(12) << _fabs(cdfx-adfx)
         << "\n";
  }   //for
}   //main

