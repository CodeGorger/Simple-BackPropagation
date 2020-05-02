/*
 * Matrix.h
 *
 *  Created on: 12.04.2020
 *      Author: simon
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>


template<typename T>
class Matrix;

template<typename T>
Matrix<T> operator*(const Matrix<T> & lhs, const Matrix<T> & rhs );

template<typename T>
std::vector<T> operator*(const Matrix<T> & lhs, const std::vector<T> & rhs );

template<typename T>
std::vector<T> operator-(const std::vector<T> & lhs, const std::vector<T> & rhs );

template<typename T>
std::vector<T> operator+(const std::vector<T> & lhs, const std::vector<T> & rhs );

template<typename T>
Matrix<T> operator*(const std::vector<T> & lhs, const std::vector<T> & rhs );

template<typename T>
class Matrix
{

private:
	size_t _rowCount;
	size_t _colCount;
	typedef std::vector<T> Row;
	std::vector<Row> _data;

public:
	Matrix(size_t rows, size_t cols);
	virtual ~Matrix();
	void Set(unsigned int i, unsigned int j, T val);
	T Get (unsigned int i, unsigned int j) const;
	size_t GetColumnCount() const { return _colCount; }
	size_t GetRowCount() const { return _rowCount; }

	void Scale(double inFactor);

	Matrix<T> Transpose();
	void AddMatrix(Matrix<T> rhs);

	std::string Print();
};

template <typename T>
Matrix<T> operator*(const Matrix<T> & lhs, const Matrix<T> & rhs )
{
	if(lhs.GetColumnCount()!=rhs.GetRowCount())
	{
		//TODO(Simon): Announce Fail.
		return Matrix<T>(0,0);
	}
	//sd:= same dimension ordinate
	size_t sdo=lhs.GetColumnCount();

	Matrix<T> ret( lhs.GetRowCount(), rhs.GetColumnCount() );

	for( size_t i=0 ; i<lhs.GetRowCount() ; i++ )
	{
		for( size_t j=0 ; j<rhs.GetColumnCount() ; j++ )
		{
			T tmpVal=0;
			for( size_t n=0 ; n<sdo ; n++ )
			{
				tmpVal+=lhs.Get(i,n)*rhs.Get(n,j);
			}
			ret.Set(i, j, tmpVal);
		}
	}
	return ret;
}


template<typename T>
std::vector<T> operator*(const Matrix<T> & lhs, const std::vector<T> & rhs )
{
	if(lhs.GetColumnCount()!=rhs.size())
	{
		//TODO(Simon): Announce Fail.
		return std::vector<T>();
	}

	//sd:= same dimension ordinate
	size_t sdo=rhs.size();

	std::vector<T> ret;
	for( size_t i=0 ; i<lhs.GetRowCount() ; i++ )
	{
		T tmpVal=0;
		for( size_t n=0 ; n<sdo ; n++ )
		{
			tmpVal+=lhs.Get(i,n)*rhs[n];
		}
		ret.push_back(tmpVal);
	}
	return ret;
}


template<typename T>
inline T fastSigmoid(T x)
{
	return 0.5*(x/(1+abs(x))+1);
}

template<typename T>
inline T realSigmoid(T x)
{
	return 1/(1+exp(-x));
}

template<typename T>
inline T fastSigmoidDerivation(T x)
{
	return 1/(2*abs(x)+1);
}

template<typename T>
inline T realSigmoidDerivation(T x)
{
	double e=exp(x);
	return e/((e+1)*(e+1));
}

template<typename T>
inline T indianSigmoidDerivation(T x)
{
	return x*(1-x);
}

template<typename T>
std::vector<T> operator-(const std::vector<T> & lhs, const std::vector<T> & rhs )
{
	size_t min=std::min(lhs.size(),rhs.size());
	//TODO(Simon): If lhs.size()!=rhs.size()-> Error!
	std::vector<T> ret;
	for( size_t i = 0 ; i<min ; i++ )
	{
		ret.push_back(lhs[i]-rhs[i]);
	}
	return ret;
}

template<typename T>
std::vector<T> operator+(const std::vector<T> & lhs, const std::vector<T> & rhs )
{
	size_t min=std::min(lhs.size(),rhs.size());
	//TODO(Simon): If lhs.size()!=rhs.size()-> Error!
	std::vector<T> ret;
	for( size_t i = 0 ; i<min ; i++ )
	{
		ret.push_back(lhs[i]+rhs[i]);
	}
	return ret;
}

template<typename T>
Matrix<T> operator*(const std::vector<T> & lhs, const std::vector<T> & rhs )
{
	Matrix<T> ret(lhs.size(),rhs.size());
	for( size_t ii=0 ; ii<lhs.size() ; ii++ )
	{
		for( size_t jj=0 ; jj<rhs.size() ; jj++ )
		{
			ret.Set(ii, jj, lhs[ii]*rhs[jj]);
		}
	}
	return ret;
}



template<typename T>
Matrix<T>::Matrix(size_t inRows, size_t inCols)
:_rowCount(inRows), _colCount(inCols)
{
	Row tmpRow = std::vector<T>(_colCount, 0);
	for (size_t i=0; i<_rowCount ;i++)
	{
		_data.push_back(tmpRow);
	}
}

template<typename T>
Matrix<T>::~Matrix()
{
}

template<typename T>
void Matrix<T>::Set(unsigned int i_row, unsigned int j_col, T val)
{
	if(i_row>=_rowCount)
	{
		//TODO(Simon): Announce Fail.
	}
	if(j_col>=_colCount)
	{
		//TODO(Simon): Announce Fail.
	}
	_data[i_row][j_col]=val;
}

template<typename T>
T Matrix<T>::Get (unsigned int i_row, unsigned int j_col) const
{
	if(i_row>=_rowCount)
	{
		//TODO(Simon): Announce Fail.
	}
	if(j_col>=_colCount)
	{
		//TODO(Simon): Announce Fail.
	}
	return _data[i_row][j_col];
}


template<typename T>
void Matrix<T>::Scale(double inFactor)
{
	for(size_t ii = 0; ii<_rowCount; ii++)
	{
		for(size_t jj = 0; jj<_colCount; jj++)
		{
			Set(ii, jj, Get(ii, jj)*inFactor);
		}
	}
}


template<typename T>
Matrix<T> Matrix<T>::Transpose()
{
	Matrix<T> ret(_colCount,_rowCount);

	for(size_t ii=0; ii<_rowCount ;ii++)
	{
		for(size_t jj=0; jj<_colCount ;jj++)
		{
			ret.Set(jj,ii,Get(ii,jj));
		}
	}

	return ret;
}



template<typename T>
void Matrix<T>::AddMatrix(Matrix<T> rhs)
{
	if(_rowCount!=rhs._rowCount)
	{
		std::cerr << "AddMatrix (...) failed." << std::endl;
		std::cerr << "Mismatching row count ( " <<
				_rowCount << " != " <<
				rhs._rowCount << " )" << std::endl;
		return;
	}
	if(_colCount!=rhs._colCount)
	{
		std::cerr << "AddMatrix (...) failed." << std::endl;
		std::cerr << "Mismatching column count ( " <<
				_colCount << " != " <<
				rhs._colCount << " )" << std::endl;
		return;
	}
	for(size_t ii=0; ii<_rowCount ;ii++)
	{
		for(size_t jj=0; jj<_colCount ;jj++)
		{
			Set(ii,jj,Get(ii,jj)+rhs.Get(ii,jj));
		}
	}
}

template <typename T>
std::string Matrix<T>::Print()
{
	std::string ret;
	for(size_t ii=0;ii<_rowCount;ii++)
	{

		for(size_t jj=0;jj<_colCount;jj++)
		{

			ret+= std::to_string(Get(ii,jj));
			ret+= " ";

		}
		ret+= "\n";
	}
	return ret;
}


#endif /* MATRIX_H_ */





