#include <stdiostr.h>
#include <math.h>
#include <values.h>

#define S 1.0
#define K 1.0
#define Y 1.0

double s=S,k=K,y=Y;
double a=1.0;
double b=Y/2.0;
double c=S/K/Y;

char verbose=0;


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
    if (verbose) printf("x=%21.18G\tfx=%21.18G\n",x,fx);
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




double_point dd[]={
  {    2.00,    4.00 },
  {    4.00,   16.00 },
  {    6.00,   36.00 },
  {    7.00,   49.00 },
  {    5.00,   25.00 },
  {    3.00,    9.00 },
  {    1.00,    1.00 },
  {   24.00,   16.00 },
  {   26.00,   36.00 },
  {   27.00,   49.00 },
  {   25.00,   25.00 },
  {   23.00,    9.00 },
  {   21.00,    1.00 },
  {   12.00,    4.00 },
  {   14.00,   16.00 },
  {   16.00,   36.00 },
  {   17.00,   49.00 },
  {   15.00,   25.00 },
  {   13.00,    9.00 },
  {   11.00,    1.00 }
};   //dd
#define DD_SIZE (sizeof(dd)/sizeof(dd[0]))


double myfunc(double x)                //function to find zero of
{
  return x*x*x/3.0;//*pow(a+x,b) - c;
}   //myfunc


double myfunc_(double x)               //derivative of zero function
{
  return (a+(b+1.0)*x);//*pow(a+x,b-1.0);
}   //myfunc_




int  loadarg(char* sw)
{
  if ((*sw=='-')||(*sw=='/')) sw++;
  switch (*sw++) {
  case '?': return 0;
  case 'y': case 'Y': sscanf(sw,"%lG",&y); break;
  case 's': case 'S': sscanf(sw,"%lG",&s); break;
  case 'k': case 'K': sscanf(sw,"%lG",&k); break;
  case 'v': case 'V':
    if (!*sw) verbose=1;
    else if (*sw=='+') verbose=1;
    else if (*sw=='-') verbose=0;
    else { printf("bad value for verbose flag \"%s\"\n",sw); return 0; }
    break;
  default:
    sw--;
    printf("unknown switch \"%s\"\n",sw);
    return 0;
  }   //switch
  return 1;
}   //loadarg


int  help(int k)
{
  printf(
    "zero -- finds g for (y,s,k) such that  g*(1+g)**(y/2) = s/(k*y)\n"
    "\n"
    "use -y<yval> -s<sval> -k<kval> to set y, s and k\n"
    "defaults are y=%G,s=%G,k=%G\n"
    "use -v to set verbose listing flag\n"
    "use -? for this listing\n"
    ,Y,S,K
  );   //printf
  return k;
}   //help


int  main(int argc,char* argv[])
{

  for (argv++,argc--;argc;argc--,argv++) if (!loadarg(*argv)) return help(1);

  a=1.0;
  b=y/2.0;
  c=s/k/y;

//  printf("g(y=%G,s=%G,k=%G)=%G\n",y,s,k,zero(myfunc,myfunc_,0));

  printf("\n==============================\n");
  printf("before...\n");
  for (int i=0;i<DD_SIZE;i++) printf("f(%G)=%G\n",dd[i].x,dd[i].fx);
  sortpoints(dd,DD_SIZE);
  printf("after...\n");
  for (i=0;i<DD_SIZE;i++) printf("f(%G)=%G\n",dd[i].x,dd[i].fx);

  printf("Point\tLeft\tRight\n");
  for (i=0;i<DD_SIZE;i++) {
    a=derivL(myfunc,dd[i].x,1.0);
    b=derivR(myfunc,dd[i].x,1.0);
    printf("%G\t%G\t%G\n",dd[i].x,a,b);
  }

  a=1.0; b=2.0;
  printf("calling deriv_points on [%G..%G] for %u points...\n",a,b,DD_SIZE);
  derivpoints(myfunc,dd,DD_SIZE,a,b);
  for (i=0;i<DD_SIZE;i++) {
    printf("%G\t%G\n",dd[i].x,dd[i].fx);
  }
}   //main

