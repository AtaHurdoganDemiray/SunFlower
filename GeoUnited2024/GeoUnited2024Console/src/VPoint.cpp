// VPoint.cpp: implementation of the VPoint class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "VPoint.h"

// VPoint.inl
#include "sVector.h"
#include "PolarCalc2d.h"

/*inline*/ const scalerType & VPoint::operator [] (int i)const
{
	return m_vector[i];
};
/*inline*/ scalerType & VPoint::operator [] (int i)
{
	return m_vector[i];
};
/*inline*/ const scalerType & VPoint::x()const
{
	return m_vector[0];
};
/*inline*/ scalerType & VPoint::x()
{
	return m_vector[0];
};
/*inline*/ const scalerType & VPoint::y()const
{
	return m_vector[1];
};
/*inline*/ scalerType & VPoint::y()
{
	return m_vector[1];
};
/*inline*/ const scalerType & VPoint::z()const
{
	return m_vector[2];
};
/*inline*/ scalerType & VPoint::z()
{
	return m_vector[2];
};
/*inline*/ void VPoint::x (const scalerType &new_x)
{
	m_vector[0] = new_x;
};
/*inline*/ void VPoint::y (const scalerType &new_y)
{
	m_vector[1] = new_y;
};
/*inline*/ void VPoint::z (const scalerType &new_z)
{
	m_vector[2] = new_z;
};
/*inline*/ const scalerType * VPoint::GetArray ()const
{
	return m_vector;
};
/*inline*/ scalerType * VPoint::GetArray()
{
	return m_vector;
};
/*inline*/ bool VPoint::operator == (const VPoint&comp)const
{
	if ( fabs(m_vector[0] - comp.m_vector[0]) < EPSILON
		&& fabs ( m_vector[1] - comp.m_vector[1] ) < EPSILON
		&& fabs ( m_vector[2] - comp.m_vector[2] ) < EPSILON )
		return true;

	return false;
};
/*inline*/ bool VPoint::operator != (const VPoint&comp)const
{
	return !(this->operator ==(comp));
};
/*inline*/ VPoint::VPoint()
:m_vector(0.0)
{

};
/*inline*/ VPoint::VPoint (const scalerType &defaultValue)
:m_vector(defaultValue)
{

};
/*inline*/ VPoint::VPoint (const scalerType pt[])
{
	m_vector[0] = pt[0];
	m_vector[1] = pt[1];
	m_vector[2] = pt[2];
};
/*inline*/ VPoint::VPoint (const scalerType &px, const scalerType &py, const scalerType &pz)
{
	m_vector[0] = px;
	m_vector[1] = py;
	m_vector[2] = pz;
};
/*inline*/ VPoint::VPoint (const VPoint &toCopy)
{
	m_vector[0]=toCopy.m_vector[0];
	m_vector[1]=toCopy.m_vector[1];
	m_vector[2]=toCopy.m_vector[2];
};
/*inline*/ VPoint::VPoint (const sVector<double,3> &v)
:m_vector(v)
{

};
//VPoint::~VPoint()
//{

//};
/*inline*/ VPoint & VPoint::operator = (const VPoint& pt)
{
	m_vector[0] = pt.m_vector[0];
	m_vector[1] = pt.m_vector[1];
	m_vector[2] = pt.m_vector[2];
	return (*this);
};
/*inline*/ VPoint & VPoint::operator = (const scalerType pt[])
{
	m_vector[0] = pt[0];
	m_vector[1] = pt[1];
	m_vector[2] = pt[2];
	return (*this);
};
/*inline*/ const VPoint VPoint::operator -()const
{
	VPoint result(-m_vector[0],-m_vector[1],-m_vector[2]);
	return result;
};
///*inline*/ VPoint VPoint::operator + (const VPoint& pt)const
//{
//	VPoint pnt;
//	pnt.m_vector[0] = m_vector[0] + pt.m_vector[0];
//	pnt.m_vector[1] = m_vector[1] + pt.m_vector[1];
//	pnt.m_vector[2] = m_vector[2] + pt.m_vector[2];
//	return pnt;
//};
/*inline*/ VPoint & VPoint::operator += (const VPoint& pt)
{
	m_vector[0] += pt.m_vector[0];
	m_vector[1] += pt.m_vector[1];
	m_vector[2] += pt.m_vector[2];
	return *this;
};
///*inline*/ VPoint VPoint::operator - (const VPoint& pt)const
//{
//	VPoint pnt;
//	pnt.m_vector[0] = m_vector[0] - pt.m_vector[0];
//	pnt.m_vector[1] = m_vector[1] - pt.m_vector[1];
//	pnt.m_vector[2] = m_vector[2] - pt.m_vector[2];
//	return pnt;
//};
/*inline*/ VPoint & VPoint::operator -= (const VPoint& pt)
{
	m_vector[0] -= pt.m_vector[0];
	m_vector[1] -= pt.m_vector[1];
	m_vector[2] -= pt.m_vector[2];
	return *this;
};
///*inline*/ const VPoint VPoint::operator * (const scalerType &scale_factor)const
//{
//	VPoint pnt;
//	pnt.m_vector[0] = m_vector[0] * scale_factor;
//	pnt.m_vector[1] = m_vector[1] * scale_factor;
//	pnt.m_vector[2] = m_vector[2] * scale_factor;
//	return pnt;
//};
/*inline*/ const VPoint operator ^ (const VPoint &p1 , const VPoint &p2)
{
	VPoint result ( ( p1.y() * p2.z()  - p1.z() * p2.y() )
					,( p1.z() * p2.x() - p1.x() * p2.z() )
					,( p1.x() * p2.y() - p1.y() * p2.x() ) );
	return result;
};
/*inline*/ const VPoint & VPoint::operator *= (const scalerType &scalef)
{
	m_vector[0] *= scalef;
	m_vector[1] *= scalef;
	m_vector[2] *= scalef;
	return *this;
};
/*inline*/ const VPoint & VPoint::operator /= (const scalerType &dividef)
{
	m_vector[0] /= dividef;
	m_vector[1] /= dividef;
	m_vector[2] /= dividef;
	return *this;
};
/*inline*/ const VPoint operator + (const VPoint &p1 , const VPoint &p2)
{
	VPoint result(p1);
	result += p2;
	return result;
};
/*inline*/ const VPoint operator - (const VPoint &p1 , const VPoint &p2)
{
	VPoint result(p1);
	result -= p2;
	return result;
};
///*inline*/ scalerType VPoint::operator % (const VPoint& pt)const
//{
//	return 	( m_vector[0] * pt.m_vector[0] + m_vector[1] * pt.m_vector[1] + m_vector[2] * pt.m_vector[2] );
//};
///*inline*/ VPoint VPoint::operator ^ (const VPoint& pt)const
//{
//	VPoint v;
//	v.m_vector[0] = m_vector[1] * pt.m_vector[2] - m_vector[2] * pt.m_vector[1];
//	v.m_vector[1] = m_vector[2] * pt.m_vector[0] - m_vector[0] * pt.m_vector[2];
//	v.m_vector[2] = m_vector[0] * pt.m_vector[1] - m_vector[1] * pt.m_vector[0];
//	return v;
//};
/*inline*/ const scalerType operator % (const VPoint &p1 , const VPoint &p2)
{
	scalerType result = p1.x() * p2.x()
						+ p1.y() * p2.y()
						+ p1.z() * p2.z();
	return result;
};


/*inline*/ VPoint VPoint::Unit ()const
{
	scalerType norm = sqrt ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] + m_vector[2] * m_vector[2] );
	VPoint unit ( m_vector[0] / norm , m_vector[1] / norm , m_vector[2] / norm );
	return unit;
};
/*inline*/ const VPoint VPoint::operator / (const scalerType &f)const
{
	VPoint result(*this);
	result /= f;
	return result;
};
/*inline*/ bool VPoint::ReplaceMin (const VPoint &source)
{
	bool anyReplaced = false;

	if (source.IsCorrupted())
		return anyReplaced;

	if ( m_vector[0] > source.x() )
	{
		m_vector[0] = source.x();
		anyReplaced = true;
	}

	if ( m_vector[1] > source.y() )
	{
		m_vector[1] = source.y();
		anyReplaced = true;
	}

	if ( m_vector[2] > source.z() )
	{
		m_vector[2] = source.z();
		anyReplaced = true;
	}
	return anyReplaced;
};
/*inline*/ bool VPoint::ReplaceMax (const VPoint &source) 
{
	bool anyReplaced = false;
	if (source.IsCorrupted())
		return anyReplaced;

	if ( m_vector[0] < source.x() )
	{
		m_vector[0] = source.x();
		anyReplaced = true;
	}

	if ( m_vector[1] < source.y() )
	{
		m_vector[1] = source.y();
		anyReplaced = true;
	}

	if ( m_vector[2] < source.z() )
	{
		m_vector[2] = source.z();
		anyReplaced = true;
	}
	return anyReplaced;
};
/*inline*/ bool VPoint::UpdateMinMax (VPoint &minp,VPoint &maxp)const
{
	bool anyReplacedMn = minp.ReplaceMin (*this);
	bool anyReplacedMx = maxp.ReplaceMax (*this);
	return (anyReplacedMn || anyReplacedMx);
};
/*inline*/ scalerType VPoint::GetLength ()const
{
	return sqrt ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] + m_vector[2] * m_vector[2] );
};
/*inline*/ scalerType VPoint::GetLength2 ()const
{
	return ( m_vector[0] * m_vector[0] + m_vector[1] * m_vector[1] + m_vector[2] * m_vector[2] );
};
/*inline*/ bool VPoint::IsCoincident (const VPoint &comp)const
{
	if ( fabs	(m_vector[0]-comp.m_vector[0]) < EPSILON 
		&& fabs	(m_vector[1]-comp.m_vector[1]) < EPSILON 
		&& fabs	(m_vector[2]-comp.m_vector[2]) < EPSILON )		
		return true;

	return false;
};
/*inline*/ bool VPoint::Compare (const VPoint &comp , const scalerType &comptol)const
{
	if ( fabs(m_vector[0]-comp.m_vector[0]) < comptol
		&& fabs (m_vector[1]-comp.m_vector[1]) < comptol 
		&& fabs (m_vector[2]-comp.m_vector[2]) < comptol )
		return true;

	return false;
};
/*inline*/ void VPoint::NormThis ()
{
	(*this) /= GetLength();
};
/*inline*/ void VPoint::operator << (VIFStream & infile)
{
	infile>>m_vector[0];
	infile>>m_vector[1];
	infile>>m_vector[2];
};
/*inline*/ void VPoint::operator >> (VofStream & outfile)const
{
	outfile<<m_vector[0]<<( IDS_N);
	outfile<<m_vector[1]<<( IDS_N);
	outfile<<m_vector[2]<<( IDS_N);
};
/*inline*/ VPoint::VPoint (VIFStream & infile)
{
	(*this)<<infile;
};
/*inline*/ void VPoint::Serialize (Bofstream &binfile)const
{
	binfile.write ( (VChr *) &(m_vector[0]) , sizeof (scalerType) );
	binfile.write ( (VChr *) &(m_vector[1]) , sizeof (scalerType) );
	binfile.write ( (VChr *) &(m_vector[2]) , sizeof (scalerType) );
};
/*inline*/ void VPoint::Serialize (Bifstream &binfile) 
{
	binfile.read ( (VChr *) &(m_vector[0]) , sizeof (scalerType) );
	binfile.read ( (VChr *) &(m_vector[1]) , sizeof (scalerType) );
	binfile.read ( (VChr *) &(m_vector[2]) , sizeof (scalerType) );
};
/*inline*/ VPoint::VPoint (Bifstream &infile)
{
	Serialize (infile);
};
/*inline*/ bool VPoint::IsCorrupted ()const
{
	if ( m_vector[0] > MAX_NUMBER_LIMIT || m_vector[0] < MIN_NUMBER_LIMIT 
		|| m_vector[1] > MAX_NUMBER_LIMIT || m_vector[1] < MIN_NUMBER_LIMIT
		|| m_vector[2] > MAX_NUMBER_LIMIT || m_vector[2] < MIN_NUMBER_LIMIT )
	{
		return true;
	}
	else
	{
		return false;
	}
};
/*inline*/ void VPoint::MAKEMAXPOINT()
{
	(*this)=GETMAXPOINT();
};
/*inline*/ void VPoint::MAKEMINPOINT()
{
	(*this)=GETMINPOINT();
};
const VPoint operator * (const scalerType &scale_factor , const VPoint &pt)
{
	VPoint tmp ( pt.x()*scale_factor , pt.y()*scale_factor , pt.z()*scale_factor );
	return tmp;
};
const VPoint operator * (const VPoint &p , const scalerType &f)
{
	VPoint result(p);
	result *= f;
	return result;
};

// VPoint.inl

bool isLessInX(const VPoint &p1,const VPoint &p2)
{	
	if(p1.x()<p2.x())
		return true;
	else
		return false;
}
bool isLessInY(const VPoint &p1,const VPoint &p2)
{
	if(p1.y()<p2.y())
		return true;
	else
		return false;
}
bool isLessInZ(const VPoint &p1,const VPoint &p2)
{
	if(p1.z()<p2.z())
		return true;
	else
		return false;
}
bool isGreaterInX(const VPoint &p1,const VPoint &p2)
{
	if(p1.x()>p2.x())
		return true;
	else
		return false;
}
bool isGreaterInY(const VPoint &p1,const VPoint &p2)
{
	if(p1.y()>p2.y())
		return true;
	else
		return false;
}
bool isGreaterInZ(const VPoint &p1,const VPoint &p2)
{
	if(p1.z()>p2.z())
		return true;
	else
		return false;
}

const VPoint MINPOINT (MIN_NUMBER_LIMIT,MIN_NUMBER_LIMIT,MIN_NUMBER_LIMIT);
const VPoint MAXPOINT (MAX_NUMBER_LIMIT,MAX_NUMBER_LIMIT,MAX_NUMBER_LIMIT);

const VPoint & GETMINPOINT()
{
	return MINPOINT;
}

const VPoint & GETMAXPOINT()
{
	return MAXPOINT;
}

VPoint::VectorDirections GetAlignTwoDirectionsParam (const VVector &v1,const VVector &v2,UnitVector &rotationAxisV1ToV2,vreal &thetaInDegrees)
{
	VPoint::VectorDirections state;
	vreal theta=0.0;
	VVector res;	
	//if ( fabs(fabs(v1%v2) - 1.0) > EPSILON)
	if (v1.Compare(v2,0.001) == false && v1.Compare(-1.0*v2,0.001) == false)
	{
		res=v1^v2;
		vreal magnitude=res.GetLength();
		state=VPoint::NOT_PARALLEL;
		theta=vp_asin(magnitude/v2.GetLength());//always positive
		if(v1%v2<0.0)
		{	
			//directions are opposite so theta should be greater than 90
			theta=PI-theta;
		}
		res.NormThis();
		rotationAxisV1ToV2=res;
		thetaInDegrees=theta*RAD_TO_DEGREE;	
		return state;
	}
	else
	//if(magnitude<EPSILON)
	{
		state=VPoint::PARALLEL_SAME;
		if(v1%v2<0.0)
		{
			state=VPoint::PARALLEL_OPPOSITE;
			theta=180.0;//PI
		}
		if(fabs(v1%UX) < fabs(v1%UY))
		{
			res=v1^UX;
		}
		else 
		{
			res=v1^UY;
		}
		res.NormThis();
		rotationAxisV1ToV2=res;
		thetaInDegrees=theta;//*RAD_TO_DEGREE;
		return state;
	}
}

bool GetAlignTwoDirectionsAround (const VVector &v1,const VVector &v2,const UnitVector &rotationAxisV1ToV2,vreal &thetaInDegrees)
{
	//vreal theta=0.0;
	//VVector res=v1^v2;
	//vreal magnitude=res.GetLength();
	//if(magnitude<EPSILON)
	if (v1.Compare(v2,0.001) == true || v1.Compare(-1.0*v2,0.001) == true)
	{
		vreal theta = 0.0;
		//VVector res = v1 ^ v2;
		//vreal magnitude = res.GetLength();
		//state=VPoint::PARALLEL_SAME;
		if(v1%v2<0.0)
		{
			theta=180.0;//PI
		}
		if (fabs(v1 % rotationAxisV1ToV2) > EPSILON)
		{
			//return false; // Around vector is not correct
			LogFileWriter::WriteUnknownExceptionLog("VPoint::GetAlignTwoDirectionsAround exception , around vector is not correct");
		}
		thetaInDegrees=theta;//*RAD_TO_DEGREE;
		return true;
	}
	else
	{
		vreal theta = 0.0;
		VVector res = v1 ^ v2;
		vreal magnitude = res.GetLength();
		res /= magnitude;
		//state=VPoint::NOT_PARALLEL;
		if (fabs (fabs (res % rotationAxisV1ToV2) - 1.0) > 1.e-6/*EPSILON*/)
		{
			//return false;  // Around vector is not correct
			LogFileWriter::WriteUnknownExceptionLog("VPoint::GetAlignTwoDirectionsAround exception , around vector is not correct , ck2");
		}

		theta=vp_asin(magnitude/v2.GetLength());//always positive
		if(v1%v2<0.0)
		{
			//directions are opposite so theta should be greater than 90
			theta=PI-theta;
		}
		thetaInDegrees=theta*RAD_TO_DEGREE;	
		if (res % rotationAxisV1ToV2 < 0.0)
		{
			// Around vector is opposite in direction so angle should be reversed
			thetaInDegrees *= -1.;
		}
	
		return true;
	}
}

const sVector<double,3> & VPoint::GetMin()const
{
	return m_vector;
}	
const sVector<double,3> & VPoint::GetMax()const
{
	return m_vector;
}

bool VPoint::IsColinear (const VPoint &p1,const VPoint &p2,const VPoint &p3,double tol)
{
	VVector v1 = p2 -p1;
	VVector v2 = p3 - p2;
	VVector crsvec = v1 ^ v2;
	if (fabs(crsvec.x()) < tol && fabs(crsvec.y())< tol && fabs(crsvec.z()) < tol)
		return true;
	else
		return false;
}

bool VPoint::IsCoordsExceed(const VPoint &p, double coordLimit)
{
	if (fabs (p.x()) > coordLimit || fabs (p.y())> coordLimit || fabs (p.z()) > coordLimit)
		return true;
	else
		return false;
}

bool VPoint::IsSharpCorner (const VPoint &c0 , const VPoint &c1 , const VPoint &c2 , double cosAngleLimit)//c0 , c1 and c2 are sequential positions on a travel
{
	VVector v0 = c1 - c0;
	VVector v1 = c2 - c1;
	return IsSharpCornerU (v0.Unit() , v1.Unit() , cosAngleLimit);
}

bool VPoint::IsSharpCornerU(const UnitVector &v0, const UnitVector &v1 , double cosAngleLimit)
{
	if (v0%v1 < cosAngleLimit)
		return true;
	else
		return false;	
}

bool VPoint::IsSharpCorner (const VVector &v0 , const VVector &v1 , double cosAngleLimit)
{
	return IsSharpCornerU (v0.Unit(),v1.Unit(),cosAngleLimit);	
}

VPoint VPoint::GetPointOnXY()const
{
	return VPoint(x(),y(),0.0);
}

VStr VPoint::ToString()const
{
	VStrStream mystr;
	mystr << x() << VGlobalValues::m_numberSeperator << y() << VGlobalValues::m_numberSeperator << z() << VGlobalValues::m_numberSeperator;
	VStr r(mystr.str());
	return r;
}

VPoint VPoint::FromString(VStr &str)
{
	//VStr singleLine = VGlobalFunctions::EatOneLine(str);
	std::vector<VStr> valVec;
	TypeConverters::SplitString(str, VGlobalValues::m_numberSeperator, valVec);
	int valVecSize = (int)valVec.size();
	if (valVecSize < 3)
		throw VException("VPoint::FromString exception vector size is smaller than 3",1);
	VPoint result(TypeConverters::ConvertStringTovreal(valVec[0]), TypeConverters::ConvertStringTovreal(valVec[1]), TypeConverters::ConvertStringTovreal(valVec[2]));
	VStr newStr;
	for (int i = 3; i < valVecSize; ++i)
	{
		newStr += valVec.at(i);
		newStr += VGlobalValues::m_numberSeperator;
	}
	str = newStr;
	return result;
}