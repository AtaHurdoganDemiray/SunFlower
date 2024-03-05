// sVector.inl

#ifndef __sVector_inl__
#define __sVector_inl__
#include "sVector.h"


	template<class sT , int N>
		sVector<sT,N>::operator const sT*()const
	{
		return m_array;
	};
	template<class sT , int N>
		sVector<sT,N>::operator sT*()
	{
		return m_array;
	};
	template<class sT , int N>
		const sT & sVector<sT,N>::operator [] (int i)const
	{
		if(i>N-1 || i<0)
			throw VException("sVector exception",1);
		return m_array[i];
	};
	template<class sT , int N>
		sT & sVector<sT,N>::operator [] (int i)
	{
		if(i>N-1 || i<0)
			throw VException("sVector exception",1);
		return m_array[i];
	};
	template<class sT , int N>
		sVector<sT,N>::sVector()
	{
	};
	template<class sT , int N>
		sVector<sT,N>::sVector(const sT &defaultvalue)
	{
		for (int i=0;i<N;i++)
			m_array[i]=defaultvalue;
	};
	template<class sT , int N>
		sVector<sT,N>::sVector(const sVector<sT,N> &toCopy)
	{
		for(int i=0;i<N;i++)
			m_array[i]=toCopy.m_array[i];
	};
	template<class sT , int N>
		sVector<sT,N>::sVector (const sT arr[])
	{
		for(int i=0;i<N;i++)
			m_array[i]=arr[i];
	};
	template<class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator = (const sT arr[])
	{
		for (int i=0;i<N;i++)
			m_array[i]=arr[i];
		return *this;
	};
	template<class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator = (const sVector<sT,N> &toCopy)
	{
		for(int i=0;i<N;i++)
			m_array[i]=toCopy.m_array[i];
		return *this;
	};
	//template<int N>
	//	sVector<double,N>::~sVector()
	//{
	//	
	//};
	template<class sT , int N>
		const sT & sVector<sT,N>::GetElement (int i) const
	{
		if(i>N-1 || i<0)
			throw VException("sVector exception",1);
		return m_array[i];
	};
	//template<int N>
	//	const sT operator % (const sVector<double,N> &v1 , const sVector<double,N> &v2)
	//{
	//	vreal sum = 0.0;
	//	for(int i=0;i<N;i++)
	//		sum += (v1.m_array[i] * v2.m_array[i]);
	//	return sum;
	//};
	template<class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator *= (const sT &f)
	{
		for (int i=0;i<N;i++)
			m_array[i] *= f;
		return *this;
	};
	//template<int N>
	//const sVector<double,N> operator * (const sT &f,const sVector<double,N> &v)
	//{
	//	sVector<double,N> result(v);
	//	result *= f;
	//	return result;
	//};
	//template<int N>
	//const sVector<double,N> operator * (const sVector<double,N> &v , const sT &f)
	//{
	//	sVector<double,N> result(v);
	//	result *= f;
	//	return result;
	//};
	template<class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator /= (const sT &f)
	{
		for (int i=0;i<N;i++)
			m_array[i] /= f;
		return *this;
	};
	template<class sT , int N>
		void sVector<sT,N>::SetElement (const sT &value,int i)
	{
		if(i>N-1 || i<0)
			throw VException("sVector exception",1);
		m_array[i] = value;
	};
	template<class sT , int N>
		sT * sVector<sT,N>::GetArray ()
	{
		return m_array;
	};
	template<class sT , int N>
		const sT * sVector<sT,N>::GetArray ()const
	{
		return m_array;
	};
	template<class sT , int N>
		void sVector<sT,N>::Serialize (Bofstream & outfile)const
	{	
		for(int i=0;i<N;i++)
			outfile.write((VChr*) & (m_array[i]) , sizeof(sT));
	};
	template<class sT , int N>
		void sVector<sT,N>::Serialize (Bifstream & infile)
	{
		for(int i=0;i<N;i++)
			infile.read ((VChr*) & (m_array[i]) , sizeof(sT));
	};
	template<class sT , int N>
		bool sVector<sT,N>::operator == (const sVector<sT,N> &toCompare)const
	{
		if(this==&toCompare)
			return true;
		for(int i=0;i<N;i++)
		{
			if ( m_array[i] - toCompare.m_array[i] > EPSILON || m_array[i] - toCompare.m_array[i] < -EPSILON)
				return false;
		}
		return true;
	};
	template<class sT , int N>
		bool sVector<sT,N>::operator != (const sVector<sT,N> &toCompare)const
	{
		return !((*this)==toCompare);
	};
	template<class sT , int N>
		const sVector<sT,N> sVector<sT,N>::operator - ()const
	{
		sVector <sT,N> res (m_array);
		res *= -1.0;
		return res;
	};
	template <class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator += (const sVector<sT,N> &v)
	{
		for(int i=0;i<N;i++)
		{
			m_array[i] += v.m_array[i];
		}
		return *this;
	};
	template <class sT , int N>
		sVector<sT,N> & sVector<sT,N>::operator -= (const sVector<sT,N> &v)
	{
		for(int i=0;i<N;i++)
		{
			m_array[i] -= v.m_array[i];
		}
		return *this;
	};
	template <class sT , int N>
		const sVector<sT,N> sVector<sT,N>::operator / (const sT &f)const
	{
		sVector<sT,N> res (*this);
		res /= f;
		return res;
	};
	//template<int N>
	//	const sVector<double,N> operator + (const sVector<double,N> &v1 , const sVector<double,N> &v2)
	//{
	//	sVector<double,N> res (v1);
	//	res += v2;
	//	return res;
	//};
	//template<int N>
	//	const sVector<double,N> operator - (const sVector<double,N> &v1 , const sVector<double,N> &v2)
	//{
	//	sVector<double,N> res (v1);
	//	res -= v2;
	//	return res;
	//};
	template<class sT , int N>
		void sVector<sT,N>::ReplaceMin (const sVector<sT,N> &p)
	{
		for (int i=0; i<N; i++)
		{
			if(p[i] < m_array[i])
			{
				m_array[i] = p[i];
			}
		}
	};
	template<class sT , int N>
		void sVector<sT,N>::ReplaceMax (const sVector<sT,N> &p)
	{
		for (int i=0; i<N; i++)
		{
			if (p[i] > m_array[i])
			{
				m_array[i] = p[i];
			}
		}
	};
	template<class sT , int N>
		const sVector<sT,N> sVector<sT,N>::operator + (const sVector<sT,N> &v)const
	{
		sVector<sT,N> result (*this);
		result += v;
		return result;
	};
	template<class sT , int N>
		const sVector<sT,N> sVector<sT,N>::operator - (const sVector<sT,N> &v)const
	{
		sVector<sT,N> result (*this);
		result -= v;
		return result;
	};
	template<class sT , int N>
		const sVector<sT,N> sVector<sT,N>::operator * (const sT &f)const
	{
		sVector<sT,N> result (*this);
		result *= f;
		return result;
	};
	template <class sT , int N>
		const sT sVector<sT,N>::operator % (const sVector<sT,N> &v)const
	{
		sT result = (sT)(0);
		for (int i=0;i<N;i++)
		{
			result += (m_array[i] * v.m_array[i]);
		}
		return result;
	};
	template <class sT , int N>
		const sT sVector<sT,N>::GetLength2()const
	{
		sT len2 = 0;		
		for (int i=0;i<N;i++)
		{
			len2 += (m_array[i] * m_array[i]);
		}
		return len2;
	}
	template <class sT , int N>
		const sT sVector<sT,N>::GetLength()const
	{
		return sqrt (GetLength2());
	};
	template <class sT , int N>
		void sVector<sT, N>::NormThis ()
	{
		bool ckAllZero = true;
		for (int i = 0; i < N; ++i)
		{
			if (fabs(m_array[i]) > 1.0e-9)
			{
				ckAllZero = false;
				break;
			}
		}
		if (ckAllZero == false)
			(*this) *= (1 / GetLength());	
	};
	template <class sT , int N>
		const sVector<sT,N> sVector<sT,N>::Unit() const
	{
		sVector<sT,N> res (*this);
		res.NormThis();
		return res;
	};

	// functions
	template <class T>
	T cross2d (const sVector<T,2> &v1 , const sVector<T,2> &v2)
	{
		return v1[0] * v2[1] - v1[1] * v2[0];
	};

	template <class T> 
	sVector<T,3> cross3d (const sVector<T,3> &v1 , const sVector<T,3> &v2)
	{
		T res[3] = { v1[1] * v2[2]  - v1[2] * v2[1] 
						, v1[2] * v2[0] - v1[0] * v2[2] 
						, v1[0] * v2[1] - v1[1] * v2[0] };
		return res;
	};

#endif
