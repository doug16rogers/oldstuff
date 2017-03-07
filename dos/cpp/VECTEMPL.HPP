#ifndef __vectempl_hpp
#define __vectempl_hpp


#include <math.h>
#include <stdarg.h>
#include <stdiostr.h>


//###########################################################################
//===========================================================================
//  generic class Vector <class Type, int size>
//---------------------------------------------------------------------------
//  The class Vector provides a generic template for vectors of any type or
//  length.  It provides a set of arithmetic operators.
//===========================================================================
//###########################################################################
template <class Type, int size>
class Vector
{
public:
  Type element[size];

  Vector() { }  //siz = size; }
  Vector(Type first, ...);

  Type& operator [] (int i) { return element[i]; }

  Type length(void);

  Vector<Type, size> operator + (Vector<Type, size>& v);
  Vector<Type, size> operator - (Vector<Type, size>& v);
  Vector<Type, size> operator * (Type scalar);
  Vector<Type, size> operator / (Type scalar);

  Type operator * (Vector<Type, size>& v);

  void operator += (Vector<Type, size>& v);
  void operator -= (Vector<Type, size>& v);
  void operator *= (Vector<Type, size>& v);
  void operator *= (Type scalar);
  void operator /= (Type scalar);

  ostream& out(ostream& os);
};   //class Vector


//===========================================================================
//  Type Vector:: length ()
//---------------------------------------------------------------------------
//  Returns the length (square root of the sum of the squares) of the object.
//===========================================================================
template <class Type, int size>
Type Vector<Type, size>::length(void)
{
  Type sum = 0;
  for (int i = 0; i < size; i++) sum += element[i] * element[i];
  return (Type) sqrt((double) sum);
}


//===========================================================================
//  Vector Vector:: operator + (Vector)
//---------------------------------------------------------------------------
//  Returns the sum of the object and the given Vector.
//===========================================================================
template <class Type, int size>
Vector<Type, size> Vector<Type, size>::operator + (Vector<Type, size>& v)
{
  Vector<Type, size> sum;
  for (int i = 0; i < size; i++) sum[i] = element[i] + v[i];
  return sum;
}


//===========================================================================
//  Vector Vector:: operator - (Vector)
//---------------------------------------------------------------------------
//  Returns the difference of the object and the given Vector.
//===========================================================================
template <class Type, int size>
Vector<Type, size> Vector<Type, size>::operator - (Vector<Type, size>& v)
{
  Vector<Type, size> difference;
  for (int i = 0; i < size; i++) difference[i] = element[i] - v[i];
  return difference;
}


//===========================================================================
//  Type Vector:: operator * (Vector)
//---------------------------------------------------------------------------
//  Returns the inner (dot) product of the object with the Vector provided.
//===========================================================================
template <class Type, int size>
Type Vector<Type, size>::operator * (Vector<Type, size>& v)
{
  Type dot_product = 0;
  for (int i = 0; i < size; i++) dot_product += element[i] * v[i];
  return dot_product;
}


//===========================================================================
//  Vector Vector:: operator * (Type)
//---------------------------------------------------------------------------
//  Returns the result of multiplying each element of the vector by the
//  scalar provided.
//===========================================================================
template <class Type, int size>
Vector<Type, size> Vector<Type, size>::operator * (Type scalar)
{
  Vector<Type,size> product;
  for (int i = 0; i < size; i++) product[i] = element[i] * scalar;
  return product;
}


//===========================================================================
//  Vector Vector:: operator / (Type)
//---------------------------------------------------------------------------
//  Returns the result of dividing each element of the vector by the scalar
//  provided.
//===========================================================================
template <class Type, int size>
Vector<Type, size> Vector<Type, size>::operator / (Type scalar)
{
  Vector<Type,size> quotient;
  for (int i = 0; i < size; i++) quotient[i] = element[i] / scalar;
  return quotient;
}



//===========================================================================
//  Vector:: operator += (Vector)
//---------------------------------------------------------------------------
//  Adds each element of the vector provided to each element.
//===========================================================================
template <class Type, int size>
void Vector<Type, size>::operator += (Vector<Type, size>& v)
{
  for (int i = 0; i < size; i++) element[i] += v[i];
}


//===========================================================================
//  Vector:: operator -= (Vector)
//---------------------------------------------------------------------------
//  Subtracts each element of the vector provided from each element.
//===========================================================================
template <class Type, int size>
void Vector<Type, size>::operator -= (Vector<Type, size>& v)
{
  for (int i = 0; i < size; i++) element[i] -= v[i];
}


//===========================================================================
//  Vector:: operator *= (Vector)
//---------------------------------------------------------------------------
//  Multiplies each element by each element of the Vector provided.
//===========================================================================
template <class Type, int size>
void Vector<Type, size>::operator *= (Vector<Type, size>& v)
{
  for (int i = 0; i < size; i++) element[i] *= v[i];
}


//===========================================================================
//  Vector:: operator *= (Type)
//---------------------------------------------------------------------------
//  Multiplies each element by the scalar provided.
//===========================================================================
template <class Type, int size>
void Vector<Type, size>::operator *= (Type scalar)
{
  for (int i = 0; i < size; i++) element[i] *= scalar;
}


//===========================================================================
//  Vector:: operator /= (Type)
//---------------------------------------------------------------------------
//  Divides each element by the scalar provided.
//===========================================================================
template <class Type, int size>
void Vector<Type, size>::operator /= (Type scalar)
{
  for (int i = 0; i < size; i++) element[i] /= scalar;
}


//===========================================================================
//  Vector:: Vector (Type, ...)
//---------------------------------------------------------------------------
//  This constructor of Vector loads the scalars of type Type into the
//  elements.  Make sure all values (size of them) are provided.
//===========================================================================
template <class Type, int size>
Vector<Type, size>::Vector(Type first, ...)
{
  va_list va;
  int i;
  Type value;

  va_start(va, first);
  element[0] = first;
  for (i = 1; i < size; i++)
  {
    element[i] = va_arg(va, Type);
  }
  va_end(va);
}


//===========================================================================
//  Vector:: out (ostream&)
//---------------------------------------------------------------------------
//  Writes the vector to the ostream as text, using curly braces and commas
//  as delimiters.
//===========================================================================
template <class Type, int size>
ostream& Vector<Type, size>::out (ostream& os)
{
  os << "{ " << element[0];
  for (int i = 1; i < size; i++)
  {
    os << ", " << element[i];
  }
  os << " }";
  return os;
}



//===========================================================================
//  generic class Square_Matrix <class Type, int size>
//---------------------------------------------------------------------------
//  Class Square_Matrix provides a square matrix of the given type and of the
//  given size x size.
//===========================================================================
template <class Type, int size>
class Square_Matrix
{
public:
  Vector<Type,size> row[size];

  Square_Matrix() { };
  Square_Matrix(Type first, ...) { };
  Square_Matrix(Vector<Type,size> first, ...);

  Vector<Type,size> col(int i);
  void              col(int i, Vector<Type,size>& v);

  void Transpose(void);

  ostream& out(ostream& os);
};   //class Square_Matrix


//===========================================================================
//  Square_Matrix:: col (int i)
//---------------------------------------------------------------------------
//  Returns a vector which is the specified column of the matrix.
//===========================================================================
template <class Type, int size>
Vector<Type,size> Square_Matrix<Type,size>::col(int i)
{
  Vector<Type,size> v;
  for (int k = 0; k < size; k++) v[k] = row[k][i];
  return v;
}


//===========================================================================
//  Square_Matrix:: col (int i, Vector& v)
//---------------------------------------------------------------------------
//  Sets the given column in the matrix to the vector provided.
//===========================================================================
template <class Type, int size>
void Square_Matrix<Type,size>::col(int i, Vector<Type,size>& v)
{
  for (int k = 0; k < size; k++) row[k][i] = v[k];
}


//===========================================================================
//  Square_Matrix:: Transpose
//---------------------------------------------------------------------------
//  Transposes the matrix (reflects it across its diagonal).
//===========================================================================
template <class Type, int size>
void Square_Matrix<Type,size>::Transpose(void)
{
  for (int i = 1; i < size; i++)
    for (int k = 0; k < i; k++)
    {
      Type temp = row[i][k];
      row[i][k] = row[k][i];
      row[k][i] = temp;
    }
}


//===========================================================================
//  Square_Matrix:: Square_Matrix (Type, ...)
//---------------------------------------------------------------------------
//  This constructor for Square_Matrix loads size x size scalars into the
//  matrix.  Make sure all of the values are provided.
//===========================================================================
template <class Type, int size>
Square_Matrix<Type, size>::Square_Matrix(Type first, ...)
{
  va_list va;

  va_start(va, first);

  row[0][0] = first;
  for (int c = 1; c < size; c++)
  {
    row[0][c] = va_arg(va, Type);
  }

  for (int r = 1; r < size; r++)
    for (int c = 0; c < size; c++)
    {
      row[r][c] = va_arg(va, Type);
    }

  va_end(va);
}


//###########################################################################
//===========================================================================
//  Square_Matrix:: Square_Matrix (Vector, ...)
//---------------------------------------------------------------------------
//  This constructor for Square_Matrix loads size Vector's into the rows of
//  the matrix.  Make sure all of the rows are provided.
//===========================================================================
//###########################################################################
template <class Type, int size>
Square_Matrix<Type, size>::Square_Matrix(Vector<Type,size> first, ...)
{
  typedef Vector<Type,size> vec;
  va_list va;

  va_start(va, first);
  row[0] = first;
  for (int r = 1; r < size; r++)
  {
    row[r] = va_arg(va, vec);
  }
  va_end(va);
}


//===========================================================================
//  Square_Matrix:: out (ostream&)
//---------------------------------------------------------------------------
//  Writes the matrix out to the ostream as text, using curly braces and
//  commas as delimiters.
//===========================================================================
template <class Type, int size>
ostream& Square_Matrix<Type, size>::out (ostream& os)
{
  os << "{ ";
  row[0].out(os);
  for (int i = 1; i < size; i++)
  {
    os << ", ";
    row[i].out(os);
  }
  os << " }";
  return os;
}



//===========================================================================
//  predefined types...
//---------------------------------------------------------------------------
//  The following typedef's are included for convenience:
//===========================================================================

typedef Vector<double,3> Vec3;          //3-dimensional vector
typedef Square_Matrix<double,3> Mat3;   //3x3 matrix





//===========================================================================
//  TEST PROGRAM
//---------------------------------------------------------------------------
//  The following is a simple test program using the vector and matrix
//  classes.
//===========================================================================
#if 0

Vec3 v0(2.0, 4.0, 4.0), v1(2.0, 0.0, -2.0);
Vec3 u;

Mat3 m( Vec3(4.0,5.0,6.0), Vec3(7.0,8.0,9.0), Vec3(10.0,11.0,12.0) );


void main(void)
{
  cout << "\n\n========================================================\n";

  u = v0 + v1;

  v0.out(cout);
  cout << " + ";
  v1.out(cout);
  cout << " = ";
  u.out(cout);
  cout << endl;

  m.row[2] *= v1;

  cout << "m = ";
  m.out(cout);
  cout << endl;

  cout << "length(v0) = " << v0.length() << endl;
}

#endif
//===========================================================================


#endif  //if file not already included
