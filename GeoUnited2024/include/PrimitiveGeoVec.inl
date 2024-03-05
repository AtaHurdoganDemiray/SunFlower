// PrimitiveGeoVec.inl

#ifndef __PrimitiveGeoVec_inl__
#define __PrimitiveGeoVec_inl__
#include "PrimitiveGeoVec.h"
#include "VectorSerializer.h"

#include "TransformationMatrix.h"
#include <iostream>


template <class T,int N>
PrimitiveGeoVec<T,N>::PrimitiveGeoVec()
:m_aabb()
{
	//m_vec.reserve(DEFAULT_CAPACITY);
};
template <class T,int N>
PrimitiveGeoVec<T,N>::PrimitiveGeoVec (int initialcapacity)
:m_aabb()
{
	m_vec.reserve(initialcapacity);
};
template <class T,int N>
PrimitiveGeoVec<T,N>::PrimitiveGeoVec (const std::vector<T> &std_vec)
:m_aabb()
{
	int vsize = (int)(std_vec.size());
	m_vec.reserve (vsize);
	for (int i=0;i<vsize;i++)
	{
		this->PushBack (std_vec.at(i));
	}
};
template <class T,int N>
PrimitiveGeoVec<T,N>::PrimitiveGeoVec (const PrimitiveGeoVec<T,N> &toCopy)
:m_vec(toCopy.m_vec),m_aabb(toCopy.m_aabb)
{
	
};
template <class T,int N>
PrimitiveGeoVec<T,N> & PrimitiveGeoVec<T,N>::operator = (const PrimitiveGeoVec<T,N> &toCopy)
{
	m_vec=toCopy.m_vec;
	m_aabb = toCopy.m_aabb;
	return *this;
};
template <class T,int N>
PrimitiveGeoVec<T,N> & PrimitiveGeoVec<T,N>::operator = (const std::vector<T> &std_vec)
{
	Clear();
	int vsize = (int)(std_vec.size());
	m_vec.reserve (vsize);

	for (int i=0;i<vsize;i++)
	{
		PushBack (std_vec.at(i));
	}
	return *this;
};
template <class T,int N>
PrimitiveGeoVec<T,N>::~PrimitiveGeoVec()
{
	
};
template <class T,int N>
int PrimitiveGeoVec<T,N>::Size()const
{
	return (int)(m_vec.size());
};
template <class T,int N>
const T & PrimitiveGeoVec<T,N>::At(int i)const
{
	return m_vec.at(i);
};
template <class T,int N>
T & PrimitiveGeoVec<T,N>::At(int i)
{
	return m_vec.at(i);
};
template <class T,int N>
const sVector<double,N> & PrimitiveGeoVec<T,N>::GetMin()const
{
	return m_aabb.GetMin();
};
template <class T,int N>
const sVector<double,N> & PrimitiveGeoVec<T,N>::GetMax()const
{
	return m_aabb.GetMax();
};
template <class T,int N>
void PrimitiveGeoVec<T,N>::PushBack(const T &newitem)
{
	m_vec.push_back(newitem);
	UpdateAABB(newitem,m_aabb);
};
template <class T,int N>
void PrimitiveGeoVec<T,N>::Clear()
{
	m_vec.clear();
	m_aabb.Clear();
};

template <class T,int N>
void PrimitiveGeoVec<T,N>::Serialize(Bofstream &outfile)const
{
	int currversion=1;
	outfile.write((VChr*)&currversion,sizeof(int));
	if ( currversion==1)
	{
		VectorSerializer::SerializeTVec(outfile,m_vec);
		m_aabb.Serialize(outfile);	
	}
	else
	{
		throw VException (("wrong version number"),2);
	}
};
template <class T,int N>
void PrimitiveGeoVec<T,N>::Serialize(Bifstream &infile)
{
	int currversion;
	infile.read((VChr*)&currversion,sizeof(int));	
	
	if (currversion==1)
	{
		VectorSerializer::SerializeTVec(infile,m_vec);
		m_aabb.Serialize(infile);
	}
	else
	{
		throw VException (("wrong version number"),3);
	}
};
template <class T,int N>
bool PrimitiveGeoVec<T,N>::operator == (const PrimitiveGeoVec<T,N> &toCompare)const
{
	if(this==&toCompare)
		return true;
	if(m_vec!=toCompare.m_vec)
		return false;
	if(m_aabb!=toCompare.m_aabb)
		return false;

	return true;
};
template <class T,int N>
bool PrimitiveGeoVec<T,N>::operator != (const PrimitiveGeoVec<T,N> &toCompare)const
{
	return !(this->operator ==(toCompare));
};
template <class T,int N>
void PrimitiveGeoVec<T,N>::Transform (const TransformationMatrix &tm)
{
	m_aabb.Clear();
    std::vector<T>::iterator i;
	for(i=m_vec.begin();i != m_vec.end(); i++)
	{
		(*i) = tm.Transform ((*i));
		m_aabb.ReplaceMin ((*i).GetMin());
		m_aabb.ReplaceMax ((*i).GetMax());
	}
};
template <class T,int N>
void PrimitiveGeoVec<T,N>::AddBack (const PrimitiveGeoVec<T,N> &toAdd)
{
	UpdateAABB (toAdd.GetMin(),m_aabb);
	UpdateAABB (toAdd.GetMax(),m_aabb);
	std::vector<T>::const_iterator i = toAdd.m_vec.begin();
	std::vector<T>::const_iterator e = toAdd.m_vec.end();
	for(i ; i!=e; i++)
	{
		m_vec.push_back (*i);
	}
};
template<class T,int N>
void PrimitiveGeoVec<T,N>::Reserve (int newcapacity)
{
	m_vec.reserve (newcapacity);
};

template <class T,int N>
const std::vector<T> & PrimitiveGeoVec<T,N>::GetVec()const
{
	return m_vec;
};

template <class T,int N>
std::vector<T> & PrimitiveGeoVec<T,N>::GetVec()
{
	return m_vec;
};

template <class T,int N>
void PrimitiveGeoVec<T,N>::UpdateMinMax ()
{
	for (int i=0;i<N;++i)
	{
		m_aabb.GetMin ()[i]=MAX_NUMBER_LIMIT;
		m_aabb.GetMax ()[i]=MIN_NUMBER_LIMIT;
	}

	int mysize = (int)m_vec.size();
	for (int i=0;i<mysize;++i)
	{
		UpdateAABB ( m_vec[i] , m_aabb);
	}
};

#endif

