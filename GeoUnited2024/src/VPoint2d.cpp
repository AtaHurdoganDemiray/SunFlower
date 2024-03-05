// VPoint2d.cpp: implementation of the VPoint2d class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "VPoint2d.h"

// VPoint2d.inl

#include  "sVector.inl"


/*inline*/ scalerType & VPoint2d::operator [] (int i)
{
	return m_vector[i];
};
/*inline*/ const scalerType & VPoint2d::operator [] (int i)const
{
	return m_vector[i];
};
/*inline*/ scalerType & VPoint2d::x()
{
	return (m_vector[0]);
};
/*inline*/ scalerType & VPoint2d::y()
{
	return (m_vector[1]);
};
/*inline*/ const scalerType & VPoint2d::x()const
{
	return m_vector[0];
};
/*inline*/ const scalerType & VPoint2d::y()const
{
	return m_vector[1];
};
/*inline*/ void VPoint2d::x (const scalerType &new_x)
{
	m_vector[0] = new_x;
};
/*inline*/ void VPoint2d::y (const scalerType &new_y)
{
	m_vector[1] = new_y;
};
/*inline*/ scalerType * VPoint2d::GetArray ()
{
	return m_vector;
};
/*inline*/ const scalerType * VPoint2d::GetArray ()const
{
	return m_vector;
};
/*inline*/ VPoint2d::VPoint2d()
:m_vector( (scalerType)(0) )
{
};
/*inline*/ VPoint2d::VPoint2d (const scalerType &defaultValue)
:m_vector(defaultValue)
{

}
/*inline*/ VPoint2d::VPoint2d (const scalerType pt[])
{
	m_vector[0] = pt[0];
	m_vector[1] = pt[1];
};
/*inline*/ VPoint2d::VPoint2d (const scalerType &px, const scalerType &py)
{
	m_vector[0] = px;
	m_vector[1] = py;
};
/*inline*/ VPoint2d::VPoint2d (const VPoint2d &toCopy)
{
	m_vector[0]=toCopy.m_vector[0];
	m_vector[1]=toCopy.m_vector[1];
};
/*inline*/ VPoint2d::VPoint2d (const sVector<double,2> &v)
:m_vector(v)
{
	
};
//VPoint2d::~VPoint2d()
//{
//};
/*inline*/  VPoint2d & VPoint2d::operator =  (const VPoint2d& pt)
{
	m_vector[0] = pt.m_vector[0];
	m_vector[1] = pt.m_vector[1];
	return (*this);
};
/*inline*/ VPoint2d & VPoint2d::operator = (const scalerType pt[])
{
	m_vector[0] = pt[0];
	m_vector[1] = pt[1];
	return (*this);
};
/*inline*/ bool VPoint2d::operator == (const VPoint2d&comp)const
{
	if ( fabs(m_vector[0] - comp.m_vector[0]) < EPSILON
		&& fabs ( m_vector[1] - comp.m_vector[1] ) < EPSILON )
		return true;

	return false;
};
/*inline*/ bool VPoint2d::operator != (const VPoint2d&comp)const
{
	return !(this->operator ==(comp));
};
/*inline*/ const VPoint2d VPoint2d::operator - ()const 
{
	VPoint2d result(-m_vector[0],-m_vector[1]);
	return result;
};
///*inline*/ VPoint2d VPoint2d::operator + (const VPoint2d& pt)const
//{
//	VPoint2d pnt;
//	pnt.m_vector[0] = m_vector[0] + pt.m_vector[0];
//	pnt.m_vector[1] = m_vector[1] + pt.m_vector[1];
//	return pnt;
//};
/*inline*/ VPoint2d & VPoint2d::operator += (const VPoint2d& pt)
{
	m_vector[0] += pt.m_vector[0];
	m_vector[1] += pt.m_vector[1];
	return *this;
};
/*inline*/ const VPoint2d operator + (const VPoint2d &p1 , const VPoint2d &p2)
{
	VPoint2d result (p1.x()+p2.x() , p1.y()+p2.y());
	return result;
};
///*inline*/ VPoint2d VPoint2d::operator - (const VPoint2d& pt)const
//{
//	VPoint2d pnt;
//	pnt.m_vector[0] = m_vector[0] - pt.m_vector[0];
//	pnt.m_vector[1] = m_vector[1] - pt.m_vector[1];
//	return pnt;
//};
/*inline*/ VPoint2d & VPoint2d::operator -= (const VPoint2d& pt)
{
	m_vector[0] -= pt.m_vector[0];
	m_vector[1] -= pt.m_vector[1];
	return *this;
};
/*inline*/ const VPoint2d operator - (const VPoint2d &p1 , const VPoint2d &p2)
{
	VPoint2d result (p1.x() - p2.x() , p1.y() - p2.y());
	return result;
};

/*inline*/ VPoint2d & VPoint2d::operator *= (const scalerType &scalef)
{
	m_vector[0] *= scalef;
	m_vector[1] *= scalef;
	return *this;
};
/*inline*/ VPoint2d & VPoint2d::operator /= (const scalerType &dividef)
{
	m_vector[0] /= dividef;
	m_vector[1] /= dividef;
	return *this;
};
///*inline*/ scalerType VPoint2d::operator % (const VPoint2d& pt)const
//{
//	return 	( m_vector[0] * pt.m_vector[0] + m_vector[1] * pt.m_vector[1] );
//};
/*inline*/ const scalerType operator % (const VPoint2d &p1 , const VPoint2d &p2)
{
	vreal result = p1.x() * p2.x() + p1.y() * p2.y();
	return result;
};
///*inline*/ scalerType VPoint2d::operator ^ (const VPoint2d& pt)const
//{
//	return ( m_vector[0] * pt.m_vector[1] - m_vector[1] * pt.m_vector[0] );
//};
/*inline*/ const scalerType operator ^ (const VPoint2d &p1 , const VPoint2d &p2)
{
	scalerType result = p1.x() * p2.y() - p1.y() * p2.x();
	return result;
};

/*inline*/ VPoint2d VPoint2d::Unit()const
{
	scalerType norm = sqrt ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] );
	VPoint2d unit ( m_vector[0] / norm , m_vector[1] / norm );
	return unit;
};
/*inline*/ const VPoint2d VPoint2d::operator / (const scalerType &f)const
{
	VPoint2d result(*this);
	result /= f;
	return result;
};
/*inline*/ void VPoint2d::ReplaceMin (const VPoint2d &source)
{
	if ( m_vector[0] > source.x() )
		m_vector[0] = source.x();

	if ( m_vector[1] > source.y() )
		m_vector[1] = source.y();
};
/*inline*/ void VPoint2d::ReplaceMax (const VPoint2d &source) 
{
	if ( m_vector[0] < source.x() )
		m_vector[0] = source.x();

	if ( m_vector[1] < source.y() )
		m_vector[1] = source.y();
};
/*inline*/ void VPoint2d::UpdateMinMax (VPoint2d &minp,VPoint2d &maxp)const
{
	minp.ReplaceMin (*this);
	maxp.ReplaceMax (*this);
};
/*inline*/ scalerType VPoint2d::GetLength ()const
{
	return sqrt ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] );
};
/*inline*/ scalerType VPoint2d::GetLength2 ()const
{
	return ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] );
};	
/*inline*/ bool VPoint2d::IsCoincident (const VPoint2d &comp)const
{
	if ( fabs	(m_vector[0]-comp.m_vector[0]) < EPSILON 
		&& fabs	(m_vector[1]-comp.m_vector[1]) < EPSILON )
		return true;

	return false;
};
/*inline*/ bool VPoint2d::Compare (const VPoint2d & comp , const scalerType & comptol)const
{
	if ( fabs(m_vector[0]-comp.m_vector[0]) < comptol
		&& fabs (m_vector[1]-comp.m_vector[1]) < comptol )
		return true;

	return false;
};
/*inline*/ void VPoint2d::NormThis()
{
	(*this) /= GetLength();
};
/*inline*/ void VPoint2d::operator << (VIFStream & infile)
{
	infile>>m_vector[0];
	infile>>m_vector[1];
};
/*inline*/ void VPoint2d::operator >> (VofStream & outfile)const
{
	outfile<<m_vector[0]<<( IDS_N);
	outfile<<m_vector[1]<<( IDS_N);
};
/*inline*/ VPoint2d::VPoint2d (VIFStream & infile)
{
	(*this)<<infile;
};
/*inline*/ void VPoint2d::Serialize (Bofstream &binfile)const
{
	binfile.write ( (VChr *) &(m_vector[0]) , sizeof (scalerType) );
	binfile.write ( (VChr *) &(m_vector[1]) , sizeof (scalerType) );
};
/*inline*/ void VPoint2d::Serialize (Bifstream &binfile) 
{
	binfile.read ( (VChr *) &(m_vector[0]) , sizeof (scalerType) );
	binfile.read ( (VChr *) &(m_vector[1]) , sizeof (scalerType) );
};
/*inline*/ VPoint2d::VPoint2d (Bifstream &infile)
{
	Serialize (infile);
};
/*inline*/ bool VPoint2d::IsCorrupted ()const
{
	if ( m_vector[0] > MAX_NUMBER_LIMIT || m_vector[0] < MIN_NUMBER_LIMIT 
		|| m_vector[1] > MAX_NUMBER_LIMIT || m_vector[1] < MIN_NUMBER_LIMIT )
	{
		return true;
	}
	else
	{
		return false;
	}
};
/*inline*/ void VPoint2d::MAKEMAXPOINT()
{
	(*this)=GETMAXPOINT2D();
};	
/*inline*/ void VPoint2d::MAKEMINPOINT()
{
	(*this)=GETMINPOINT2D();			
};

/*inline*/ const VPoint2d operator * (const scalerType &scale_factor , const VPoint2d &pt)
{
	VPoint2d tmp ( pt.x()*scale_factor , pt.y()*scale_factor );
	return tmp;
};
/*inline*/ const VPoint2d operator * (const VPoint2d &pt , const scalerType &scale_factor)
{
	VPoint2d tmp ( pt.x()*scale_factor , pt.y()*scale_factor );
	return tmp;
};

// VPoint2d.inl

bool isLessInX(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2)
{
	if(p1->x()<p2->x())
		return true;
	else
		return false;
}
bool isLessInY(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2)
{
	if(p1->y()<p2->y())
		return true;
	else
		return false;
}
bool isGreaterInX(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2)
{
	if(p1->x()>p2->x())
		return true;
	else
		return false;
}
bool isGreaterInY(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2)
{
	if(p1->y()>p2->y())
		return true;
	else
		return false;
}

const VPoint2d MINPOINT (MIN_NUMBER_LIMIT,MIN_NUMBER_LIMIT);
const VPoint2d MAXPOINT (MAX_NUMBER_LIMIT,MAX_NUMBER_LIMIT);

const VPoint2d & GETMINPOINT2D()
{
	return MINPOINT;
}

const VPoint2d & GETMAXPOINT2D()
{
	return MAXPOINT;
}
const sVector<double,2> & VPoint2d::GetMin()const
{
	return m_vector;
}
	
const sVector<double,2> & VPoint2d::GetMax()const
{
	return m_vector;
}

void VPoint2d::MakeTenDigits()
{
	float newx = (float)(m_vector.operator [](0));
	this->x (newx);
	float newy = (float)(m_vector.operator [](1));
	this->y (newy);
}
