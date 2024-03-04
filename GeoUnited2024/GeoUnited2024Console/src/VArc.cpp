// VArc.cpp

#include "stdafx.h"
#include "VArc.h"
#include "LinearEquations.h"
#include "TransformationMatrix.h"
#include "VLine.h"
#include "CoordinateSystem.h"
#include "PolarCalc.h"
#include "Arc.h"
#include "igesTransformationMatrix.h"
#include "ReorientMatrix.h"
#include "IgesEntitySet.h"
#include "PolarCalc2d.h"
#include "VLine2d.h"
#include "QuadraticEquation.h"
#include "TransformationMatrixAnalyzer.h"
#include "VInterval.h"

VArc::VArc()
: center (0.0,0.0,0.0),radius(1.0),startpoint(1.0,0.0,0.0),sweepangle(360.0),ax(0.0,0.0,1.0)
{
	//std::string s( IDS_Ssssss);
	
}

VArc::VArc ( const VPoint & _center , const vreal & _radius  , const VVector & _ax , const VPoint & _startpoint , const vreal & _sweepangle )
:center(_center),radius(_radius),startpoint(_startpoint),sweepangle(_sweepangle), ax(_ax)
{
	ax.NormThis();
}

VArc::VArc (const VPoint &_center , const vreal &_radius )
:center (_center),radius(_radius) 
, startpoint(_center.x() + _radius,_center.y(),_center.z())
,sweepangle(360.0)
,ax(0.0,0.0,1.0)
{

}

VArc::VArc ( const VArc & Foo )
{
	(*this) = Foo;
}
VArc::VArc (const VPoint &p1
			,const VPoint &p2
			, const VPoint &p3 
			, const VPoint &p4)
{
	VArc a(p1,p2,p3);
	vreal d1 = (a.GetCenter() - p1).GetLength2();
	VArc b(p2,p3,p4);
	vreal d2 = (b.GetCenter() - p2).GetLength2();
	VArc c(p3,p4,p1);
	vreal d3 = (c.GetCenter() - p3).GetLength2();
	if (d1<d3)
	{
		if (d2>d1)
		{
			*this=a;
		}
	}
	if (d1>d2)
	{
		if(d2<d3)
		{
			*this=b;
		}
	}
	if (d3<d1)
	{
		if(d3<d2)
		{
			*this=c;
		}
	}
}

VArc::VArc (const VPoint &p1, const VPoint &p2, const VPoint &unitArcAx, double bulge) : ax(unitArcAx)
{
	VPoint vec = p2 - p1;
	vec.NormThis();
	CoordinateSystem cs(p1, vec, unitArcAx  ^ vec, unitArcAx);
	
	VPoint mp = (p1 + p2)*0.5;
	VPoint midArcPoint = mp + bulge * cs.GetDiry();
	//MakeCircleFrom3Points(p1, midArcPoint, p2);
	//if ( this->GetAx () % unitArcAx < EPSILON ) 
	//	this->ReverseAxis();
	//double len = (mp - p1).GetLength();
	VArc temp ( p1 , midArcPoint , p2);
	if ( temp.GetAx () % unitArcAx < EPSILON ) 
		temp.ReverseAxis();
	if ( p1.Compare ( temp.GetStartPoint() , VGlobalValues::ONEMICRON_TO_VUNIT) == false)
		*this = VArc ( temp.GetCenter() , temp.GetRadius() , temp.GetAx() , temp.GetEndPoint() , -temp.GetSweepAngle());
	else
		*this = temp;
}
VArc::VArc(const VPoint& _center, vreal _radius , const VPoint& tn0 , const VPoint& tn1)// for r = 0 case , ax is UZ
:center(_center), radius(_radius), startpoint(_center), ax(0.0, 0.0, 1.0)
{
	// calculate sweep angle between 2 direction vectors : tn0 line and next line tn1 line
	//VPoint tn0U = tn0.Unit();
	VPoint yaxis(-tn0.y(), tn0.x(), 0.0);
	CoordinateSystem tmpcs(VPoint(0.0), tn0, yaxis, UZ);
	//VPoint tn1U = tn1.Unit();
	VPoint trv = tmpcs.AbsoluteCoordToRelativeCoord(tn1);
	sweepangle = PolarCalc2d::GetPolarAngle(trv.x(), trv.y());
	if (sweepangle > 180.0)
		sweepangle = sweepangle - 360.0;
}
VArc::~VArc()
{

}

VArc & VArc::operator = ( const VArc & Foo )
{
	center = Foo.center;
	radius = Foo.radius;
	startpoint = Foo.startpoint;
	sweepangle = Foo.sweepangle;
	ax = Foo.ax;
	return (*this);
}

VArc::VArc ( const VPoint & p1 , const VPoint & p2 , const VPoint & p3 )
{
	MakeCircleFrom3Points (p1,p2,p3);
}

const VPoint & VArc::GetCenter() const
{
	return center;
}

const vreal & VArc::GetRadius() const
{
	return radius;
}

const VPoint & VArc::GetStartPoint () const
{
	return startpoint;
}

const vreal & VArc::GetSweepAngle() const
{
	return sweepangle;
}

const VPoint & VArc::GetAx () const
{
	return ax;
}

bool VArc::operator == ( const VArc & comp ) const
{
	if ( center == comp.center
		&& radius == comp.radius
		&& startpoint == comp.startpoint
		&& sweepangle == comp.sweepangle 
		&& ax == comp.ax )
		return true;
	else
		return false;
}

bool VArc::operator != ( const VArc & comp ) const
{
	return !( (*this) == comp );
}

bool VArc::CalcCircleFrom3Points ( const VPoint & p1 , const VPoint & p2 , const VPoint & p3 
											, VPoint & _center , VPoint & nor , vreal & _radius )
{
	// Function calculates the center point and radius of circle 
	// whose 3 point are given.
	// If points are colinear and no solution it returns false.

	// Calculating the plane normal of circle.
	VPoint v1 = p2 - p1;
	VPoint v2 = p3 - p1;
	
	nor = v1 ^ v2;

	if ( fabs(nor.x()) < EPSILON 
		&& fabs(nor.y()) < EPSILON
		&& fabs(nor.z()) < EPSILON )
	{
		// points are colinear 
		return false;
	}

	nor.NormThis();
	vreal x1 , y1 , x2 , y2 , x3 , y3;
	
	// defining a temp coord system for making calculations easier.
	VPoint origin = p1;
	VPoint dirz = nor;
	VPoint dirx = v1;
	dirx.NormThis();
	VPoint diry = dirz ^ dirx;
	
	x1 = 0.0; 
	y1 = 0.0;

	x2 =  v1 % dirx ;
	y2 = v1 % diry ;

	x3 = v2 % dirx ;
	y3 = v2 % diry ;

	// building 2 unknown linear eq. system(s)

	vreal a1 , b1 , c1 , a2 , b2 , c2;
	vreal solutionx , solutiony;

	a1 = 2 * ( x2 - x1 );
	b1 = 2 * ( y2 - y1 );
	c1 = x1 * x1 + y1 * y1 - ( x2 * x2 + y2 * y2 );

	a2 = 2 * ( x3 - x1 );
	b2 = 2 * ( y3 - y1 );
	c2 = x1 * x1 + y1 * y1 - ( x3 * x3 + y3 * y3 );
	if ( !LinearEquations::SolveLinear2Unknown ( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
	{
		a1 = 2 * ( x1 - x2 );
		b1 = 2 * ( y1 - y2 );
		c1 = x2 * x2 + y2 * y2 - ( x1 * x1 + y1 * y1 );

		a2 = 2 * ( x3 - x2 );
		b2 = 2 * ( y3 - y2 );
		c2 = x2 * x2 + y2 * y2 - ( x3 * x3 + y3 * y3 );

		if ( !LinearEquations::SolveLinear2Unknown 
			( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
		{
			a1 = 2 * ( x2 - x3 );
			b1 = 2 * ( y2 - y3 );
			c1 = x3 * x3 + y3 * y3 - ( x2 * x2 + y2 * y2 );

			a2 = 2 * ( x1 - x3 );
			b2 = 2 * ( y1 - y3 );
			c2 = x3 * x3 + y3 * y3 - ( x1 * x1 + y1 * y1 );
		
			if ( !LinearEquations::SolveLinear2Unknown 
			( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
				return false;
		}
	}

	// for debug
	//vreal tmpx,tmpy;
	//bool cknnnn = LinearEquations::SolveLinear2UnknownByGauss (a1,b1,c1,a2,b2,c2,tmpx,tmpy);
	// for debug

	//Calculating the radius.
	vreal dx = solutionx - x1;
	vreal dy = solutiony - y1;
	_radius = sqrt(dx * dx + dy * dy);

	// Transform back to absolute coordinates for center point.
	VPoint tvx = dirx * solutionx; 
	VPoint tvy = diry * solutiony;
	_center = origin + tvx + tvy;	
	return true;
}		

void VArc::Serialize (Bofstream &outfile)const
{
	outfile.write((VChr*)&radius,sizeof(vreal));
	center.Serialize(outfile);
	outfile.write((VChr*)&sweepangle,sizeof(vreal));
	startpoint.Serialize(outfile);
	ax.Serialize(outfile);
}
	
void VArc::Serialize (Bifstream &infile)
{
	infile.read((VChr*)&radius,sizeof(vreal));
	center.Serialize(infile);
	infile.read((VChr*)&sweepangle,sizeof(vreal));
	startpoint.Serialize(infile);
	ax.Serialize(infile);
}

void VArc::Transform(const TransformationMatrix &tm)
{
	center=tm.Transform(center);
	startpoint=tm.Transform(startpoint);
	ax=tm.TransformVec(ax);
	ax.NormThis();//if arc is scaled this is needed
	TransformationMatrixAnalyzer at;
	if (at.IsMirror(tm))
		sweepangle *= -1.;
	double sx,sy,sz;
	at.GetScaleFactors(tm,sx,sy,sz);
	// check if arc axis is along UX , UY or UZ
	bool isAlongX =false;
	if ( fabs (1. - fabs (GetAx() % UX)) < 1.e-8)
		isAlongX = true;
	bool isAlongY =false;
	if ( fabs (1. - fabs (GetAx() % UY)) < 1.e-8)
		isAlongY = true;
	bool isAlongZ = false;
	if ( fabs (1. - fabs (GetAx() % UZ)) < 1.e-8)
		isAlongZ = true;
	if (isAlongX == false && isAlongY == false && isAlongZ == false)
	{
		if (fabs(sx-sy) > 1.e-8 || fabs (sx-sz) > 1.e-8)
			throw VException (( IDS_VArTraExcNonUni),1);
		radius *= sx;
	}
	else if (isAlongX)
	{
		if (fabs (sy-sz) > 1.e-8)
			throw VException (( IDS_VArTraExcNonUni),1);
		radius *= sy;
	}
	else if (isAlongY)
	{
		if (fabs (sx-sz) > 1.e-8)
			throw VException (( IDS_VArTraExcNonUni),1);
		radius *= sx;
	}
	else if (isAlongZ)
	{
		if (fabs(sx-sy) > 1.e-8)
			throw VException (( IDS_VArTraExcNonUni),1);
		radius *= sx;
	}
	else
		throw VException (( IDS_VArTraExcArcAxi),1);
}

void VArc::MakeFullCircle()
{
	sweepangle=360.;
}

CoordinateSystem VArc::GetCoordinateSystem()const
{
	VPoint dirx=GetStartPoint()-GetCenter();
	dirx.NormThis();
	VPoint dirz=GetAx();
	dirz.NormThis();
	VPoint diry=dirz^dirx;
	diry.NormThis();
	VPoint myorigin=GetCenter();
	CoordinateSystem cs(myorigin,dirx,diry,dirz);
	return cs;
}

VArc::VArc ( const VPoint & _center , const vreal & _radius , const VPoint & _ax , const VPoint & _startpoint , const VPoint & _endpoint )
{
	//VVector v = _endpoint-_startpoint;
	//VPoint pm = _startpoint + 0.5*v;
	//VVector rvec = pm - _center;
	//rvec.NormThis();
	//rvec*=_radius;
	//VPoint _midpoint = _center + rvec;
	//MakeCircleFrom3Points (_startpoint , _midpoint , _endpoint);
	VArc tmparc (_center , _radius , _ax , _startpoint , 360.);
	CoordinateSystem cs = tmparc.GetCoordinateSystem();
	//VPoint deltas = _startpoint-_center;
	//double xc0 = deltas % cs.GetDirx();
	//double yc0 = deltas % cs.GetDiry();
	//double _sweepangle1 = PolarCalc2d::GetPolarAngle(xc0,yc0);
	VPoint deltae = _endpoint-_center;
	double xc = deltae % cs.GetDirx();
	double yc = deltae % cs.GetDiry();
	double _sweepangle = PolarCalc2d::GetPolarAngle(xc,yc);
	
	//double _sweepangle = _sweepangle2 - _sweepangle1;
	if (_sweepangle < 0.0)
		_sweepangle = 360 + _sweepangle;	// this is used mostly for iges arcs and they are always CCW from start to end so make angle + by complementing to 360

	if (fabs(_sweepangle) < EPSILON)
		_sweepangle = 360.0;

	(*this) = VArc (_center , _radius , _ax , _startpoint , _sweepangle); 
}

void VArc::MakeCircleFrom3Points (const VPoint &p1 , const VPoint &p2 , const VPoint &p3)
{
	VPoint acenter , anor;
	vreal aradius;
	bool ck=this->CalcCircleFrom3Points ( p1 , p2 , p3 , acenter , anor , aradius );
	if(ck==false)
		throw VException(( IDS_ArcFroPoiExc),1);
	radius = aradius;
	center = acenter;
	startpoint = p1;
	ax = anor;
	
	//PolarCalc angleCalc;
	vreal angle2= PolarCalc::GetAngleBetween(center,p1,p2,ax);
	vreal angle3= PolarCalc::GetAngleBetween(center,p1,p3,ax);
	// Circle is made such that arc starts from p1 and ends at p3 and p2 is in the middle	
	if(angle3>angle2)
		sweepangle=angle3;
	else
		sweepangle=angle2-360.0;
	// Circle is made such that arc starts from p1 and ends at p3 and p2 is in the middle
}

bool VArc::IsFull()const
{
	if (fabs(fabs(sweepangle) - 360.0) < 1.e-8)
		return true;
	else
		return false;
}

void VArc::GetIgesArc (igesArc &a , igesTransformationMatrix &tm , bool &isTransformed)const
{
	//if (fabs(ax.x()) < EPSILON && fabs(ax.y())< EPSILON && fabs (ax.z() - 1)<EPSILON)
	//{
	//	// there is no need to translate 
	//	isTransformed = false;
	//	a.mZT = center.z();
	//	a.mX1 = center.x();
	//	a.mY1 = center.y();
	//	a.mX2 = GetStartPoint().x();
	//	a.mY2 = GetStartPoint().y();
	//	a.mX3 = GetEndPoint().x();
	//	a.mY3 = GetEndPoint().y();
	//	return;
	//}

	CoordinateSystem absolutecs;
	//CoordinateSystem cs = GetCoordinateSystem();
	//if (sweepangle < EPSILON)
	//{
	//	CoordinateSystem tmp (cs.GetOrigin(),cs.GetDirx(),-cs.GetDiry(),-cs.GetDirz());
	//	cs = tmp;
	//}
	VArc tmp = sweepangle < EPSILON ? VArc (center , radius , -ax , startpoint , -sweepangle):(*this);
	if (fabs ( (tmp.GetAx() % UZ) - 1) < 1.e-8)
	{
		a.mZT = tmp.GetCenter().z();
		a.mX1 = tmp.GetCenter().x();
		a.mY1 = tmp.GetCenter().y();
		a.mX2 = tmp.GetStartPoint().x();
		a.mY2 = tmp.GetStartPoint().y();
		VPoint endp = tmp.GetEndPoint();
		a.mX3 = endp.x();
		a.mY3 = endp.y();
		isTransformed = false;
	}
	else
	{
		CoordinateSystem cs = tmp.GetCoordinateSystem();
		ReorientMatrix to2d (cs /*tmp.GetCoordinateSystem()*/ , absolutecs);
		tmp.Transform(to2d);
		a.mZT = 0.0;
		a.mX1 = tmp.GetCenter().x();
		a.mY1 = tmp.GetCenter().y();
		a.mX2 = tmp.GetStartPoint().x();
		a.mY2 = tmp.GetStartPoint().y();
		VPoint endp = tmp.GetEndPoint();
		a.mX3 = endp.x();
		a.mY3 = endp.y();
		ReorientMatrix to3d (absolutecs , cs);
		to3d.SetIgesTransformation(tm );
		isTransformed = true;
	}
}

VPoint VArc::GetEndPoint ()const
{
	if (IsFull())
		return startpoint;
	CoordinateSystem cs = GetCoordinateSystem();
	vreal radangle = sweepangle * DEGREE_TO_RAD;
	vreal localx = radius * cos(radangle);
	vreal localy = radius * sin(radangle);
	VPoint localp (localx,localy,0.0);
	VPoint endp = cs.RelativeCoordToAbsoluteCoord (localp);
	return endp;
}

int VArc::MakeAndAddIgesArc(IgesEntitySet &entset)
{
	igesArc * a = new igesArc;
	igesTransformationMatrix * tm = new igesTransformationMatrix;
	bool istm;
	GetIgesArc (*a,*tm,istm);
	if (istm)
	{
		int tno = entset.InsertEntity (tm);
		a->DE.pXform = tno;
	}
	int ano = entset.InsertEntity (a);
	return ano;
}

double VArc::GetMinDist(const VPoint &p, VPoint &minponarc, double &angle)const
{
	// Project point on circle plane
	VPoint pp = ProjectPointOnArcPlane(p);

	// crossing point and angle
	VPoint dif = pp - center;
	if (dif.GetLength2() < EPSILON)
	{
		// this is point is on center
		minponarc = GetStartPoint();
		angle = 0.0;
		return radius;
	}
	CoordinateSystem cs = GetCoordinateSystem();
	double xc = dif % cs.GetDirx();
	double yc = dif % cs.GetDiry();
	double minangle = PolarCalc2d::GetPolarAngle(xc,yc);
	if (sweepangle > 0)
	{
		if (minangle <= sweepangle)
		{
			// minpoint is between sweep of arc so return it
			minponarc = center + (radius * dif.Unit());
			angle = minangle;
			return (p-minponarc).GetLength();
		}
		else
		{
			// minpoint is not between sweep of arc , one of the limit points is the minp on circle , either start or end point
			if (minangle - sweepangle < 360 - minangle)
			{
				// minpoint is closer to end point of circle so
				minponarc = GetEndPoint();
				angle = sweepangle;
				return (p-minponarc).GetLength();
			}
			else
			{
				// minpoint is closer to start point of circle
				minponarc = startpoint;
				angle = 0.0;
				return (p-minponarc).GetLength();
			}
		}
	}
	else
	{
		double inverseangle = minangle - 360.0;
		if (fabs(sweepangle)>=fabs(inverseangle))
		{
			// minpoint is between sweep of arc so return it
			minponarc = center + (radius * dif.Unit());
			angle = inverseangle;
			return (p-minponarc).GetLength();
		}
		else
		{
			// min point is not between sweep of arc , one of the limit points is the minp on circle , either start or end point
			if (fabs(inverseangle)-fabs(sweepangle) < 360 - fabs(inverseangle))
			{
				// minpoint is closer to end point of circle
				minponarc = GetEndPoint();
				angle = sweepangle;
				return (p-minponarc).GetLength();
			}
			else
			{
				// min point is closer to start point
				minponarc = startpoint;
				angle = 0.0;
				return (p-minponarc).GetLength();
			}
		}
	}

	return 1.;
}

std::vector<VArc> VArc::Split (const double angle , const double lengththreshold)const
{
	// use positive equavalent of angle if it is negative
	double myangle = angle;
	if (angle < 0)
		myangle += 360.;
	std::vector<VArc> res;
	if (sweepangle < 0)
	{
		// sweepangle is negative so use negative equavalent of angle
		myangle -= 360.;
		if (fabs(myangle) > fabs(sweepangle) || fabs(myangle) < EPSILON || fabs(myangle - sweepangle) < EPSILON)
		{
			res.push_back(*this);
		}
		else
		{
			// Split into 2 
			VArc a1 (*this);
			a1.sweepangle = myangle;
			VArc a2 (a1.center , a1.radius , a1.ax , a1.GetEndPoint() , sweepangle - myangle);
			double al1 = fabs(a1.GetSweepAngle() * 2.0 * a1.GetRadius() * DEGREE_TO_RAD);
			double al2 = fabs(a2.GetSweepAngle() * 2.0 * a2.GetRadius() * DEGREE_TO_RAD);
			if (al1 > lengththreshold && al2 > lengththreshold)
			{
				res.push_back(a1);
				res.push_back(a2);
			}
			else
				res.push_back (*this);
		}
	}
	else
	{
		if (myangle > sweepangle || fabs(myangle) < EPSILON || fabs (myangle - sweepangle) < EPSILON)
		{
			// one result the arc itself
			res.push_back(*this);
		}
		else
		{
			// Split into 2
			VArc a1 (*this);
			a1.sweepangle = myangle;
			VArc a2 (a1.center , a1.radius , a1.ax , a1.GetEndPoint() , sweepangle - myangle );
			double al1 = fabs(a1.GetSweepAngle() * 2.0 * a1.GetRadius() * DEGREE_TO_RAD);
			double al2 = fabs(a2.GetSweepAngle() * 2.0 * a2.GetRadius() * DEGREE_TO_RAD);
			if (al1 > lengththreshold && al2 > lengththreshold)
			{
				res.push_back (a1);
				res.push_back (a2);
			}
			else
				res.push_back(*this);
		}
	}

	return res;
}

VPoint VArc::ProjectPointOnArcPlane(const VPoint &p) const
{
	// Project point on circle plane
	VPoint pp;
	double val1 = center % ax;
	double val2 = p % ax;
	double t = val1 - val2;
	pp = p + (ax * t);
	return pp;
}

std::vector<VPoint> VArc::GetInt(const VLine &l) const
{
	CoordinateSystem cs = GetCoordinateSystem();
	VPoint pp1 = ProjectPointOnArcPlane(l.GetP1());
	VPoint pp2 = ProjectPointOnArcPlane(l.GetP2());
	VPoint rpp1 = pp1 - center;
	VPoint rpp2 = pp2 - center;
	VPoint2d rl1 (rpp1 % cs.GetDirx(),rpp1 % cs.GetDiry());
	VPoint2d rl2 (rpp2 % cs.GetDirx(),rpp2 % cs.GetDiry());
	VLine2d rl (rl1,rl2);

	std::vector<VPoint> res;
	VPoint2d D = rl.GetP1(); 
	double ca = rl.GetV() % rl.GetV();
	double cb = 2.0 * (D % rl.GetV());
	double cc = (D%D) - radius * radius;

	QuadraticEquation q (ca,cb,cc);
	std::vector<double> roots;
	q.GetRoots(roots);
	if (roots.size() == 1 || roots.size() == 2)
	{
		// find roots which are between 0-1 range
		std::vector<double> online;
		if (roots[0] <= 1.0 && roots[0] >= 0.0)
			online.push_back(roots[0]);
		if (roots.size()== 2)
			if (roots[1] <= 1.0 && roots[1] >= 0.0)
				online.push_back (roots[1]);
		std::vector<VPoint2d> onlinep;
		size_t mysize = online.size();
		for (size_t j=0;j<mysize;++j)
		{
			onlinep.push_back((rl.GetP(online[j])));
		}

		// Check if intersections is inside arc angle range
		for (size_t j=0;j<mysize; ++j)
		{
			VPoint2d curp = onlinep[j];
			double angle = PolarCalc2d::GetPolarAngle(curp.x(),curp.y());
			bool isonarc = IsAngleOnArc(angle);
			if (isonarc)
			{
				VPoint pthreed (curp.x(),curp.y(),0.0);
				VPoint rp=cs.RelativeCoordToAbsoluteCoord(pthreed);
				res.push_back(rp);
			}
		}
	}

	return res;
}
std::vector<VPoint> VArc::GetIntForInfiniteLineAndCircleOfArc (const VLine &l) const
{
	CoordinateSystem cs = GetCoordinateSystem();
	VPoint pp1 = ProjectPointOnArcPlane(l.GetP1());
	VPoint pp2 = ProjectPointOnArcPlane(l.GetP2());
	VPoint rpp1 = pp1 - center;
	VPoint rpp2 = pp2 - center;
	VPoint2d rl1 (rpp1 % cs.GetDirx(),rpp1 % cs.GetDiry());
	VPoint2d rl2 (rpp2 % cs.GetDirx(),rpp2 % cs.GetDiry());
	VLine2d rl (rl1,rl2);

	std::vector<VPoint> res;
	VPoint2d D = rl.GetP1(); 
	double ca = rl.GetV() % rl.GetV();
	double cb = 2.0 * (D % rl.GetV());
	double cc = (D%D) - radius * radius;

	QuadraticEquation q (ca,cb,cc);
	std::vector<double> roots;
	q.GetRoots(roots);
	if (roots.size() == 1 || roots.size() == 2)
	{
		// find roots which are between 0-1 range
		std::vector<double> online;
		//if (roots[0] <= 1.0 && roots[0] >= 0.0)
			online.push_back(roots[0]);
		if (roots.size()== 2)
			//if (roots[1] <= 1.0 && roots[1] >= 0.0)
				online.push_back (roots[1]);
		std::vector<VPoint2d> onlinep;
		size_t mysize = online.size();
		for (size_t j=0;j<mysize;++j)
		{
			onlinep.push_back((rl.GetP(online[j])));
		}

		// Check if intersections is inside arc angle range
		for (size_t j=0;j<mysize; ++j)
		{
			VPoint2d curp = onlinep[j];
			double angle = PolarCalc2d::GetPolarAngle(curp.x(),curp.y());
			//bool isonarc = IsAngleOnArc(angle);
			//if (isonarc)
			{
				VPoint pthreed (curp.x(),curp.y(),0.0);
				VPoint rp=cs.RelativeCoordToAbsoluteCoord(pthreed);
				res.push_back(rp);
			}
		}
	}

	return res;
}
std::vector<VPoint> VArc::GetInt(const VArc &a , vreal overlapTol) const
{
	std::vector<VPoint> res;
	CoordinateSystem cs = GetCoordinateSystem();
	// Project center of a to our coordinate system
	VPoint pca = ProjectPointOnArcPlane(a.GetCenter());
	VPoint dif = pca - center;
	double C1x = dif % cs.GetDirx();
	double C1y = dif % cs.GetDiry();
	double K = radius * radius + C1x * C1x + C1y * C1y - a.radius * a.radius;
	if (fabs(C1x) < overlapTol /*EPSILON*/ && fabs(C1y)<overlapTol/*EPSILON*/)
	{
		// Centers are coincident
		// This function don t handle overlapping circles
		// if circles do not overlap the intersection is empty set anyway 
		return res;
	}
	
	double ca , cb , cc;
	bool xiscalc;
	double L,M,N,O;
	if (fabs(C1x)>fabs(C1y))
	{
		double df = 1./ C1x;
		N = K * 0.5 * df;
		O = -C1y * df;
		ca = O*O + 1;
		cb = 2 * O * N;
		cc = N * N - radius * radius;
		xiscalc = false;
	}
	else
	{
		double df = 1. / C1y;
		L = K * 0.5 * df;
		M = -C1x * df;
		ca = M * M + 1.;
		cb = 2. * L * M;
		cc = L * L - radius * radius;
		xiscalc = true;
	}
	QuadraticEquation q (ca,cb,cc);
	std::vector<double> roots;
	int rtno = q.GetRoots(roots);
	for (int i=0;i<rtno;++i)
	{
		VPoint rp (0.0);
		double cr = roots[i];
		if (xiscalc)
		{
			rp.x (cr);
			rp.y (L + M * cr);
		}
		else
		{
			rp.y (cr);
			rp.x (N + O * cr);
		}
		double myangle = PolarCalc2d::GetPolarAngle(rp.x(),rp.y());
		if (IsAngleOnArc(myangle))
		{
			VPoint ap = cs.RelativeCoordToAbsoluteCoord(rp);
			double myangle2 = a.GetAngle(ap);
			if (a.IsAngleOnArc(myangle2))
				res.push_back(ap);
		}
	}
	return res;
}
std::vector<VPoint> VArc::GetIntForFullArcs (const VArc &a , vreal overlapTol) const
{
	std::vector<VPoint> res;
	CoordinateSystem cs = GetCoordinateSystem();
	// Project center of a to our coordinate system
	VPoint pca = ProjectPointOnArcPlane(a.GetCenter());
	VPoint dif = pca - center;
	double C1x = dif % cs.GetDirx();
	double C1y = dif % cs.GetDiry();
	double K = radius * radius + C1x * C1x + C1y * C1y - a.radius * a.radius;
	if (fabs(C1x) <overlapTol /*EPSILON*/ && fabs(C1y)<overlapTol/*EPSILON*/)
	{
		// Centers are coincident
		// This function don t handle overlapping circles
		// if circles do not overlap the intersection is empty set anyway 
		return res;
	}
	
	double ca , cb , cc;
	bool xiscalc;
	double L,M,N,O;
	if (fabs(C1x)>fabs(C1y))
	{
		double df = 1./ C1x;
		N = K * 0.5 * df;
		O = -C1y * df;
		ca = O*O + 1;
		cb = 2 * O * N;
		cc = N * N - radius * radius;
		xiscalc = false;
	}
	else
	{
		double df = 1. / C1y;
		L = K * 0.5 * df;
		M = -C1x * df;
		ca = M * M + 1.;
		cb = 2. * L * M;
		cc = L * L - radius * radius;
		xiscalc = true;
	}
	QuadraticEquation q (ca,cb,cc);
	std::vector<double> roots;
	int rtno = q.GetRoots(roots);
	for (int i=0;i<rtno;++i)
	{
		VPoint rp (0.0);
		double cr = roots[i];
		if (xiscalc)
		{
			rp.x (cr);
			rp.y (L + M * cr);
		}
		else
		{
			rp.y (cr);
			rp.x (N + O * cr);
		}
		VPoint ap = cs.RelativeCoordToAbsoluteCoord(rp);
		res.push_back(ap);
	}
	return res;
}

VArc VArc::GetReversed() const
{
	VArc r (center,radius,ax,GetEndPoint(),-sweepangle);
	return r;
}

VPoint VArc::GetP (double angle)const// angle is in arc cs
{
	angle*=DEGREE_TO_RAD;
	VPoint lp ( GetRadius() * cos(angle) , GetRadius() * sin(angle) , 0.0 );
	VPoint result = GetCoordinateSystem().RelativeCoordToAbsoluteCoord ( lp );
	return result;
}

bool VArc::IsCCW ()const
{
	VVector r = GetStartPoint() - GetCenter();
	double da = 1.;
	if (sweepangle < 0.0)
		da = -1.;
	VVector t = GetP (da) - GetStartPoint();
	VVector crs = r ^ t;
	if (crs % GetAx() > 0.)
		return true;
	else
		return false;
}

bool VArc::IsAngleOnArc(double positiveAngle) const
{
	if (sweepangle < 0.0)
	{
		double negativeAngle = positiveAngle - 360.0;
		if (negativeAngle - sweepangle > -1.e-8)//(negativeAngle >= sweepangle)
		{
			return true;
		}
	}
	else
	{
		if (positiveAngle - sweepangle < 1.e-8)//(positiveAngle <= sweepangle)
		{
			return true;
		}
	}
	return false;
}

double VArc::GetAngle(const VPoint &p)const
{
	CoordinateSystem cs = GetCoordinateSystem();
	VPoint rp = cs.AbsoluteCoordToRelativeCoord(p);
	double angle = PolarCalc2d::GetPolarAngle(rp.x(),rp.y());
	if (sweepangle < 0.0)
		angle -= 360.0;
	return angle;
}

void VArc::ReverseAxis()
{
	ax *= -1.;
	sweepangle *= -1.;
}

void VArc::ReplaceMinMax(VPoint &minpoint , VPoint &maxpoint ,int dividecount)const
{
	double mysweep = sweepangle;
	if (fabs(mysweep) < EPSILON)
		mysweep = 360.0;
	double deltaangle = mysweep / dividecount;
	if (fabs(deltaangle) < EPSILON)
		throw VException (( IDS_VArRepExcDelIs),1);
	double curangle = 0;
	for (int i=0;i<=dividecount;++i)
	{
		VPoint cp = GetP(curangle);
		minpoint.ReplaceMin(cp);
		maxpoint.ReplaceMax(cp);
		curangle += deltaangle;
	}
}
void VArc::ReplaceMinMaxInCs(const CoordinateSystem &cs , VPoint &minpoint , VPoint &maxpoint ,int dividecount)const
{
	double mysweep = sweepangle;
	if (fabs(mysweep) < EPSILON)
		mysweep = 360.0;
	double deltaangle = mysweep / dividecount;
	if (fabs(deltaangle) < EPSILON)
		throw VException (( IDS_VArRepExcDelIs),1);
	double curangle = 0;
	for (int i=0;i<=dividecount;++i)
	{
		VPoint cp = GetP(curangle);
		VPoint cpcs = cs.AbsoluteCoordToRelativeCoord (cp);
		minpoint.ReplaceMin(cpcs);
		maxpoint.ReplaceMax(cpcs);
		curangle += deltaangle;
	}
}
double VArc::GetArcLength()const
{
	double arclength = sweepangle * DEGREE_TO_RAD * radius;
	return fabs(arclength);
}

bool VArc::IsDegenerate (bool ckMinRadius
						, double minRadiusTolerance
						, bool ckCenterStartPointConformsRadius
						, double radiusConformanceTolerance
						, bool ckIsAxVectorUnit
						, double axVectorCkTolerance
						, bool ckIsAxPerpToStartVec
						, double axPerpToStartVecTolerance
						, bool ckSweepAngle
						, double sweepAngleTolerance
						, bool ckMinPerimeter
						, double minPerimeterAllowed 
						, bool ckMaxPerimeter
						, double maxPerimeterAllowed)const
{
	if (ckMinRadius)
	{
		if (GetRadius()<minRadiusTolerance)
			return true;
	}
	VPoint startVector = GetStartPoint()-GetCenter();
	vreal startVecLength = startVector.GetLength(); 
	if (ckCenterStartPointConformsRadius)
	{
		if (fabs(startVecLength - GetRadius()) > radiusConformanceTolerance)
			return true;
	}
	startVector /= startVecLength;
	if (ckIsAxVectorUnit)
	{
		if ((GetAx().GetLength() - 1.0) > axVectorCkTolerance)
			return true;
	}
	if (ckIsAxPerpToStartVec)
	{
		if ( fabs ( startVector % GetAx()) > axPerpToStartVecTolerance)
			return true;
	}
	if (ckSweepAngle)
	{
		if (fabs(GetSweepAngle ()) < sweepAngleTolerance)
			return true;
	}
	if (ckMinPerimeter || ckMaxPerimeter)
	{
		double perimeter = GetArcLength();
		if (ckMinPerimeter)
		{
			if (perimeter < minPerimeterAllowed)
				return true;
		}
		if (ckMaxPerimeter)
		{
			if (perimeter > maxPerimeterAllowed)
				return true;
		}
	}

	return false;
}

bool VArc::Compare (const VArc &a2 , double tol)const
{
	if (fabs (GetRadius()-a2.GetRadius()) > tol)
		return false;
	if (GetAx().Compare (a2.GetAx() , tol) == false)
		return false;
	if (fabs (GetSweepAngle() - a2.GetSweepAngle()) > tol)
		return false;
	if (GetCenter().Compare (a2.GetCenter(),tol) == false)
		return false;
	if (fabs (fabs(GetSweepAngle())-360) > tol)
	{
		if (GetStartPoint().Compare (a2.GetStartPoint(),tol) == false)
			return false;
	}
	return true;
}

double VArc::GetBulge() const
{
	/*CoordinateSystem cs = GetCoordinateSystem();*/
	VPoint p1 = /*cs.AbsoluteCoordToRelativeCoord*/(startpoint);
	VPoint p2 = /*cs.AbsoluteCoordToRelativeCoord*/(GetEndPoint());

	if (IsFull())
		throw VException( IDS_VArGetArcFul, 1);

	VPoint kirisMidP = (p2 + p1) * 0.5;
	VPoint arcMidP = /*cs.AbsoluteCoordToRelativeCoord*/(GetP(sweepangle * 0.5));

	VPoint vec = (p2 - p1).Unit();
	VPoint axVec(-vec.y(), vec.x(), 0.);
	axVec.NormThis();
	VPoint vec2 = arcMidP - kirisMidP;
	
	vreal bulge = (kirisMidP - arcMidP).GetLength();

	if (axVec % vec2 < 0)
		return -bulge;
	
	return bulge;
}
bool VArc::IsOnArc(const VPoint &p, vreal tol) const
{
	VPoint diff = center - p;
	vreal x = fabs(diff.GetLength2() - radius * radius);
	if (x > tol)
		return false;

	vreal ang = GetAngle(p);
	if (sweepangle < 0)
	{
		ang -= 360;
		if (ang < sweepangle)
			return false;
	}
	else
		if (ang > sweepangle)
			return false;

	return true;
}
bool VArc::Split (int n, std::vector<VArc> &res)const
{
	if (n <= 1)
	{
		res.push_back(*this);
		return false;
	}

	vreal myangle = sweepangle / n;
//	if (angle < 0)
	//	myangle += 360.;
	if (sweepangle < 0)
	{
		// sweepangle is negative so use negative equavalent of angle
		//myangle -= 360.;
	/*	VPoint sp = startpoint;
		for (int i = 0; i < n; ++i)
		{
			VArc splitArc(center , radius , ax , sp, myangle);
			res.push_back(splitArc);
			sp = splitArc.GetEndPoint();
		}*/
	}
	else
	{
		/*VPoint sp = startpoint;
		for (int i = 0; i < n; ++i)
		{
			VArc splitArc(center , radius , ax , sp, myangle);
			res.push_back(splitArc);
			sp = splitArc.GetEndPoint();
		}*/
	}

	VPoint sp = startpoint;
	for (int i = 0; i < n; ++i)
	{
		VArc splitArc(center , radius , ax , sp, myangle);
		res.push_back(splitArc);
		sp = splitArc.GetEndPoint();
	}

	return true;
}

vreal VArc::GetChordWidthAtH (vreal h)const// h is assumed to be between 0 and radius (0 means center chord width is diam, r means tip of circle chord width is 0)
{
	if (h < 0.0)
		return radius * 2.0;
	if (h > radius)
		return 0.0;
	return (sqrt (radius * radius - h*h) * 2.0);
}

vreal VArc::GetChordalDistanceForChordWidth (vreal chordWidth)const // chordal distance is distance between center of chord and circle tip
{
	if (chordWidth < 0.0)
		return radius;
	vreal halfwidth = chordWidth * 0.5;
	if (halfwidth > radius)
		return 0.0;

	vreal dist1 = sqrt (radius * radius - halfwidth*halfwidth);
	return radius-dist1;
}

vreal VArc::GetChordHForWidth (vreal chordWidth)const
{
	vreal halfWidth = chordWidth * 0.5;
	if (halfWidth > radius)
		return 0.0;
	vreal h = sqrt (radius*radius - halfWidth*halfWidth);
	return h;
}

vreal VArc::GetChordAngle(vreal chordWidth)const
{
	vreal h = GetChordHForWidth (chordWidth);
	vreal halfw = chordWidth * 0.5;
	vreal angleInRad = atan2 (halfw,h);
	return angleInRad * RAD_TO_DEGREE * 2.0;
}

void VArc::GetStartEndTangentUnitVectors (VPoint &uStartTangentVec , VPoint &uEndTangentVec)const
{
	VVector v = GetStartPoint()-GetCenter();
	v.NormThis();
	uStartTangentVec = GetAx()^v;
	if (GetSweepAngle() < 0.0)
		uStartTangentVec *= -1.;
	if (IsFull())
	{
		uEndTangentVec = uStartTangentVec;
	}
	else
	{
		VVector v2 = GetEndPoint()-GetCenter();
		v2.NormThis();
		uEndTangentVec = GetAx() ^ v2;
		if (GetSweepAngle() < 0.0)
			uEndTangentVec *= -1.;
	}
}

VPoint VArc::GetUnitStartTangentVec()const
{
	VVector v = GetStartPoint()-GetCenter();
	v.NormThis();
	VPoint uStartTangentVec = GetAx()^v;
	if (GetSweepAngle() < 0.0)
		uStartTangentVec *= -1.;
	return uStartTangentVec;
}

VPoint VArc::GetUnitEndTangentVec()const
{
	VVector v2 = GetEndPoint()-GetCenter();
	v2.NormThis();
	VPoint uEndTangentVec = GetAx() ^ v2;
	if (GetSweepAngle() < 0.0)
		uEndTangentVec *= -1.;
	return uEndTangentVec;
}

VPoint VArc::GetTangAt (const VPoint &pointOnArc)const
{
	VPoint vec = pointOnArc - GetCenter();
	vec.NormThis();
	VPoint tang = this->GetAx() ^ vec;
	if (IsCCW() == false)
		tang *= -1.;
	return tang;
}
VPoint VArc::GetTan (vreal par)const
{
	VPoint p2 = GetP(par);	
	VPoint rVec = (p2 - GetCenter()).Unit();
	vreal fac = this->sweepangle < 0.0 ? 1. : -1.;
	return rVec ^ GetAx() *fac; 
}
void VArc::SplitTo4Quadrants (std::vector<VSignedInterval> &fromStartToEnd , int quadi[4])const// quadi[i] = -1 if no arc portion lies on that angle quadrant
{
	VPoint tanVecAtStart = GetTan (0.0);
	VPoint tanVecAtMid = GetTan (0.5);
	VPoint tanVecAtEnd = GetTan (1.);
	vreal tAstart = PolarCalc2d::GetPolarAngle (tanVecAtStart.x(),tanVecAtStart.y());
	vreal tAmid = PolarCalc2d::GetPolarAngle (tanVecAtMid.x(),tanVecAtMid.y());
	vreal tAend = PolarCalc2d::GetPolarAngle (tanVecAtEnd.x(),tanVecAtEnd.y());
	VInterval ai (tAstart,tAend);
	VInterval tAmidi (tAmid,tAmid);
	bool isNonReversed = true;
	std::list<vreal> mp;
	if (ai.IsContaining(tAmidi) == false)
	{
		isNonReversed = false;
	}
	if (tAstart > tAend)
		isNonReversed = !isNonReversed;

	int initialqno = PolarCalc2d::GetQuadrantFor (tAstart);
	mp.push_back (tAstart);
	if (IsCCW())
	{
		for (int j=0;j<4;++j)
		{
			vreal curqa = PolarCalc2d::GetQuadrantAngles((initialqno+j) % 4).GetT1();
			VInterval tmp(curqa,curqa);
			if (ai.IsContaining(tmp) == isNonReversed)
			{
				mp.push_back(curqa);
			}
			else
				break;
		}
	}
	else
	{
		initialqno += 4;
		for (int j=0; j<4;++j)
		{
			vreal curqa = PolarCalc2d::GetQuadrantAngles((initialqno - j)%4).GetT0();
			VInterval tmp (curqa,curqa);
			if (ai.IsContaining(tmp) == isNonReversed)
			{
				mp.push_back(curqa);
			}
			else
				break;
		}
	}
	mp.push_back(tAend);
	std::list<vreal>::const_iterator ii = mp.begin();
	std::list<vreal>::const_iterator ee = mp.end();
	for (ii;ii!=ee;++ii)
	{
		std::list<vreal>::const_iterator ni = ii;
		++ni;
		if (ni!=ee)
		{
			VInterval curinterval (*ii,*ni);
			int signf = 1;
			if (*ii>*ni)
				signf = -1;
			fromStartToEnd.push_back (VSignedInterval (curinterval,signf));
		}
	}
	// angles are absolute we need transform to parameters of VArc
}	

//void VArc::SplitTo4Quadrants (std::vector<VArc> &fromStartToEnd , int quadi[4])const// quadi[i] = -1 if no arc portion lies on that angle quadrant
//{
//
//}

VArc VArc::GetArcOnXY()const
{
	vreal comptol = VGlobalValues::ONEMICRON_TO_VUNIT;
	if (GetAx().Compare(UZ, comptol) == false)
	{
		throw VException("VArc::GetArcOnXY exception , arc plane not parallel to XY",1);
	}
	VArc myarc(this->GetCenter().GetPointOnXY(), this->GetRadius(), this->GetAx(), this->GetStartPoint().GetPointOnXY(), this->GetSweepAngle());
	return myarc;
}

vreal VArc::GetArcStartAngleBetween0_360()const
{
	CoordinateSystem cs = this->GetCoordinateSystem();
	VPoint relstart = cs.AbsoluteCoordToRelativeCoord(this->GetStartPoint());
	vreal startAngle = PolarCalc2d::GetPolarAngle(relstart.x(), relstart.y());
	return startAngle;
}

vreal VArc::GetArcEndAngleBetween0_360()const
{
	VPoint endpoint = this->GetEndPoint();
	CoordinateSystem cs = this->GetCoordinateSystem();
	VPoint relend = cs.AbsoluteCoordToRelativeCoord(endpoint);
	vreal endAngle = PolarCalc2d::GetPolarAngle(relend.x(), relend.y());
	return endAngle;
}

bool VArc::IsPointOnArc(const VPoint& p, vreal tol)const
{
	VPoint localp = GetCoordinateSystem().AbsoluteCoordToRelativeCoord(p);
	if (fabs((localp.x() * localp.x() + localp.y() * localp.y()) - GetRadius() * GetRadius()) > tol * tol * 4.0)
		return false;
	vreal ang = PolarCalc2d::GetPolarAngle(localp.x(), localp.y());
	vreal ang2 = ang - 360.0;
	VInterval i(0.0, GetSweepAngle());
	if (i.IsContaining(ang))
		return true;
	if (i.IsContaining(ang2))
		return true;
	return false;
}

VArc VArc::MakeArcWith2PointsAndRadius(const VPoint& p1, const VPoint& p2, vreal radius , const VPoint& arcPlaneNormal)// negative radius allowed
{
	if (p1.Compare(p2, 1.e-10))
	{
		//VpAfxMessageBox((IDS_CirPoiAndCenCan));
		throw VException("VArc::MakeArcWith2PointsAndRadius exception. Given 2 points are coincident.",1);// ((IDS_CirPoiAndCenCan), 1);
	}
	//EndDynamicArcDraw();

	if (VerifyCenterPointOn(p1, p2, arcPlaneNormal) == false)
	{
		//VpAfxMessageBox((LPCSTR)MsgStrings::M15());
		//return;
		throw VException("VArc::MakeArcWith2PointsAndRadius exception. There is a given plane normal. p1 or p2 is not on this plane.",1);
	}
	vreal len = (p1 - p2).GetLength();
	//if (len > radius)
	//	VException( IDS_ActCalTwoPoiAnd, 1);
	vreal len2 = len * 0.5;

	vreal a = sqrt((radius * radius) - (len2 * len2));
	vreal bulge = radius - a;
	VArc myarc(p1, p2, arcPlaneNormal , bulge);
	return myarc;
}

bool VArc::VerifyCenterPointOn(const VPoint& c
	, const VPoint& p
	, const VVector& ax)
{
	if (fabs((c - p) % ax) > 1.e-4)
		return false;// center point and circle point are not coPlanar
	else
		return true;
}

OffsetHelperArc::OffsetHelperArc(const VPoint& tn1, const VPoint& corner, const VPoint& tn2, const VPoint& planeNormal)
	:m_corner(corner),m_tn1(tn1)
{
	m_n1 = tn1 ^ planeNormal;
	m_n2 = tn2 ^ planeNormal;
	VPoint tnMid = ((tn1 + tn2) * 0.5).Unit();
	m_nMid = tnMid ^ planeNormal;
}
VArc OffsetHelperArc::GetArc(vreal signedRadius)const
{
	vreal smartSignFactor = 1.0;

	VArc a1(m_corner+m_n1*signedRadius,m_corner+m_nMid*signedRadius,m_corner+m_n2*signedRadius);
	VArc a2(m_corner + m_n1 * signedRadius, m_corner + m_nMid * ( -1.0* signedRadius), m_corner + m_n2 * signedRadius);
	if (fabs((a1.GetUnitStartTangentVec() % m_tn1) - 1.0) < 1.e-8)
		return a1;
	else
		return a2;
}