#ifndef MTH_MATRIX_H
#define MTH_MATRIX_H

#include "mthVector.h"

/** \file mthMatrix.h
  * \brief Small compile-time and run-time linear algebra matrices. */

namespace mth {

/** \brief compile-time (static) matrix */
template <class T, unsigned M=0, unsigned N=0>
class Matrix : public Array<Vector<T,N>,M>
{
  public:
    /** \brief mutable index operator
      * \details an index operator (i,j) is provided so that
      * compile time matrices and runtime-sized matrices can be used
      * interchangebly, without worrying about changing index syntax */
    T& operator()(unsigned i, unsigned j) {return (*this)[i][j];}
    /** \brief immutable index operator
      * \details see the mutable index operator details */
    T const& operator()(unsigned i, unsigned j) const {return (*this)[i][j];}
    /** \brief default constructor */
    Matrix() {}
    /** \brief add two matrices */
    Matrix<T,M,N> operator+(Matrix<T,M,N> const& b) const
    {
      Matrix<T,M,N> r;
      for (unsigned i=0; i < M; ++i)
        r.elems[i] = this->elems[i] + b.elems[i];
      return r;
    }
    /** \brief subtract two matrices */
    Matrix<T,M,N> operator-(Matrix<T,M,N> const& b) const
    {
      Matrix<T,M,N> r;
      for (unsigned i=0; i < M; ++i)
        r.elems[i] = this->elems[i] - b.elems[i];
      return r;
    }
    /** \brief multiply by a scalar */
    Matrix<T,M,N> operator*(T const& s) const
    {
      Matrix<T,M,N> r;
      for (unsigned i=0; i < M; ++i)
        r.elems[i] = this->elems[i] * s;
      return r;
    }
    /** \brief divide by a scalar */
    Matrix<T,M,N> operator/(T const& s) const
    {
      Matrix<T,M,N> r;
      for (unsigned i=0; i < M; ++i)
        r.elems[i] = this->elems[i] / s;
      return r;
    }
    /** \brief multiply a matrix by a vector */
    Vector<T,M> operator*(Vector<T,N> const& b) const
    {
      Vector<T,M> r;
      for (unsigned i=0; i < M; ++i)
        r[i] = this->elems[i] * b;
      return r;
    }
    /** \brief multiply two matrices
      * \details the extra template parameter generates
      * code for all possible combinations of matrix sizes */
    template <unsigned O>
    Matrix<T,M,O> operator*(Matrix<T,N,O> const& b) const
    {
      Matrix<T,M,O> r;
      for (unsigned i=0; i < M; ++i)
      for (unsigned j=0; j < O; ++j)
      {
        r[i][j] = this->elems[i][0]*b[0][j];
        for (unsigned k=1; k < N; ++k)
          r[i][j] += this->elems[i][k]*b[k][j];
      }
      return r;
    }
};

/** \brief run-time (dynamic) matrix
  * \details a runtime sized equivalent of mth::Matrix<T,N>.
  * This class is meant to be used for small dense matrices
  * whose size is not known at compile time. For large, sparse,
  * or parallel matrices, look outside of mth.
  *
  * Note that the template specialization for M=0, N=0 does not
  * inherit from an Array of Vectors as the compile-time matrix
  * does. Rather, a linear access model is used, where matrix
  * elements are stored in a single dynamic array */
template <class T>
class Matrix<T,0,0>
{
  public:
    /** \brief default constructor - no allocation */
    Matrix() {}
    /** \brief construct m by n elements */
    Matrix(unsigned m, unsigned n) : columns(n), elems(m*n) {}
    /** \brief get the number of rows */
    unsigned rows() const {return elems.size()/columns;}
    /** \brief get the number of columns */
    unsigned cols() const {return columns;}
    /** \brief resize to m by n elements */
    void resize(unsigned m, unsigned n)
    {
      columns = n;
      elems.resize(m*n);
    }
    /** \brief mutable index operator */
    T& operator()(unsigned i, unsigned j)
    {
      return elems[i*columns + j];
    }
    /** \brief immutable index operator */
    T const& operator()(unsigned i, unsigned j) const
    {
      return elems[i*columns + j];
    }
    /** \brief add a matrix to this matrix */
    Matrix<T>& operator+=(Matrix<T> const& b)
    {
      for (unsigned i=0; i < this->elems.size(); ++i)
        this->elems[i] += b.elems[i];
      return *this;
    }
    /** \brief subtract a matrix from this matrix */
    Matrix<T>& operator-=(Matrix<T> const& b)
    {
      for (unsigned i=0; i < this->elems.size(); ++i)
        this->elems[i] -= b.elems[i];
      return *this;
    }
    /** \brief multiply this matrix by a scalar */
    Matrix<T>& operator*=(T const& s)
    {
      for (unsigned i=0; i < this->elems.size(); ++i)
        this->elems[i] *= s;
      return *this;
    }
    /** \brief divide this matrix by a scalar */
    Matrix<T>& operator/=(T const& s)
    {
      for (unsigned i=0; i < this->elems.size(); ++i)
        this->elems[i] /= s;
      return *this;
    }
  protected:
    unsigned columns;
    Array<double> elems;
};

}

#endif
