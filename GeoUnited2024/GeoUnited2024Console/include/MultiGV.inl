// MultiGV.inl

#ifndef __MultiGV_inl__
#define __MultiGV_inl__
#include "MultiGV.h"

#include "Points.h"
#include "Lines.h"
#include "Triangles.h"
#include "VectorSerializer.h"

template<class G , class V>
	MultiGV<G,V>::MultiGV (const std::vector <const V*> & geoVecPointerVec)
{
	m_totalSize = 0;
	std::vector<const V*>::const_iterator i;
	for (i=geoVecPointerVec.begin(); i != geoVecPointerVec.end(); i++)
	{
		m_totalSize += (*i)->Size();
		m_geoVecPointerVec.push_back(*i);
	}
}
template<class G , class V>
	MultiGV<G,V>::MultiGV (int totalSize , const std::vector <const V*> & geoVecPointerVec)
	:m_geoVecPointerVec (geoVecPointerVec.size())
{
	m_totalSize = totalSize;
	copy (geoVecPointerVec.begin() , geoVecPointerVec.end() , m_geoVecPointerVec.begin());
}
template <class G , class V>
	MultiGV<G,V>::MultiGV (const MultiGV<G,V> &toCopy)
{
	m_totalSize = toCopy.m_totalSize;
	m_geoVecPointerVec = m_geoVecPointerVec;
}

template <class G , class V>
	MultiGV<G,V> & MultiGV<G,V>::operator = (const MultiGV<G,V> &toCopy)
{
	m_totalSize = toCopy.m_totalSize;
	m_geoVecPointerVec = toCopy.m_geoVecPointerVec;
	return *this;
}

template <class G , class V>
	int MultiGV<G,V>::Size ()const
{
	return m_totalSize;
}
template <class G , class V>
	const G MultiGV<G,V>::operator [] (int i)const
{
	int rangeBegin=0,rangeEnd=0;
	std::vector<const V*>::const_iterator j;
	for (j=m_geoVecPointerVec.begin();j != m_geoVecPointerVec.end();j++)
	{
		rangeEnd=rangeBegin+(*j)->Size();
		if(i>=rangeBegin && i<rangeEnd)
		{
			return (*j)->operator [](i-rangeBegin);
		}
		rangeBegin=rangeEnd;
	}
	throw VException ("MultiGV exception",1);
}
template<class G,class V>
const VPoint MultiGV<G,V>::GetMin()const
{
	VPoint overallmin = GETMAXPOINT();
	std::vector<const V*>::const_iterator i = m_geoVecPointerVec.begin();
	std::vector<const V*>::const_iterator e = m_geoVecPointerVec.end();
	
	for(i; i != e ; i++)
	{
		overallmin.ReplaceMin ( (*i)->GetMin() );
	}
	return overallmin;
}
template <class G,class V>
const VPoint MultiGV<G,V>::GetMax()const
{
	VPoint overallmax = GETMINPOINT();
	std::vector<const V*>::const_iterator i = m_geoVecPointerVec.begin();
	std::vector<const V*>::const_iterator e = m_geoVecPointerVec.end();
	
	for(i; i != e ; i++)
	{
		overallmax.ReplaceMax ( (*i)->GetMax() );
	}
	return overallmax;
}
template <class G,class V>
std::vector<const V*> & MultiGV<G,V>::GetGeoVecPointerVec()
{
	return m_geoVecPointerVec;
}
template <class G,class V>
void MultiGV<G,V>::Serialize (Bofstream &outfile)const
{
	int version = 1;
	outfile.write ((VChr *)(&version) , sizeof (int));
	if (version == 1)
	{
		outfile.write ((VChr *)(&m_totalSize) , sizeof(int));
		VectorSerializer::SerializePtrVec (outfile , m_geoVecPointerVec);	
	}
	else
	{
		throw VException ("MultiGV<G,V>::Serialize exception" , 1);
	}
}
template <class G,class V>
void MultiGV<G,V>::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr*)&version , sizeof (int));
	if (version == 1)
	{
		infile.read ((VChr *)(&m_totalSize),sizeof(int));
		VectorSerializer::SerializePtrVec (infile , m_geoVecPointerVec);
	}
	else
	{
		throw VException ("MultiGV<G,V>::Serialize exception" , 2);
	}
}

#endif
