// CoordinateSystem.cpp

#include "stdafx.h"
#include "CoordinateSystem.h"
#include "TransformationMatrix.h"
#include "PolarCalc2d.h"
#include "RotationMatrix.h"

CoordinateSystem::CoordinateSystem ()
:origin(0.0,0.0,0.0),dirx(1.0,0.0,0.0),diry(0.0,1.0,0.0),dirz(0.0,0.0,1.0)
{

}

CoordinateSystem::CoordinateSystem ( const VPoint & _origin )
:origin(_origin),dirx(1.0,0.0,0.0),diry(0.0,1.0,0.0),dirz(0.0,0.0,1.0)
{

}

CoordinateSystem::CoordinateSystem ( const VPoint & _origin 
									, const VPoint & _dirx
									, const VPoint & _diry
									, const VPoint & _dirz )
{
	origin = _origin;
	dirx = _dirx;
	diry = _diry;
	dirz = _dirz;
	if ( ! CheckAxis())
		throw VException (( IDS_CooConExcAxiAre),1);
#if _DEBUG
	if(dirx%diry>1.e-6/*EPSILON*/ || dirx%dirz>1.e-6/*EPSILON*/ || diry%dirz>1.e-6/*EPSILON*/)
		throw VException(( IDS_CooConExc),1);
#endif
}

//#############################################################################

CoordinateSystem::CoordinateSystem ( const CoordinateSystem & Foo )
{
	(*this) = Foo;
}

//#############################################################################

const CoordinateSystem & CoordinateSystem::operator = ( const CoordinateSystem & Foo )
{
	origin = Foo.origin;
	dirx = Foo.dirx;
	diry = Foo.diry;
	dirz = Foo.dirz;
	return (*this);
}

//#############################################################################

const VPoint & CoordinateSystem::GetOrigin () const
{
	return origin;
}

//#############################################################################

const VPoint & CoordinateSystem::GetDirx () const
{
	return dirx;
}

//#############################################################################

const VPoint & CoordinateSystem::GetDiry () const
{
	return diry;
}

//#############################################################################

const VPoint & CoordinateSystem::GetDirz () const
{
	return dirz;
}

//#############################################################################

VPoint CoordinateSystem::AbsoluteCoordToRelativeCoord ( const VPoint & absolute ) const
{
	VPoint diff = absolute - origin;
	VPoint result ( diff%dirx , diff%diry , diff%dirz );
	return result;
}

//#############################################################################

VPoint CoordinateSystem::RelativeCoordToAbsoluteCoord ( const VPoint & relative ) const
{
	VPoint result (origin);
	result += (dirx*relative.x());
	result += (diry*relative.y());
	result += (dirz*relative.z());
	return result;
}
//#############################################################################

VPoint CoordinateSystem::AbsoluteVectorToRelativeVector(const VPoint &vec)const
{
	VPoint p1(0,0,0);
	VPoint transformed1=AbsoluteCoordToRelativeCoord(p1);
	VPoint transformed2=AbsoluteCoordToRelativeCoord(vec);
	VPoint tVec=transformed2-transformed1;
	return tVec;
}

//#############################################################################

VPoint CoordinateSystem::RelativeVectorToAbsoluteVector(const VPoint &vec)const
{
	//VPoint transformed1=RelativeCoordToAbsoluteCoord(origin);
	//VPoint transformed2=RelativeCoordToAbsoluteCoord(vec);
	//VPoint tVec=transformed2-transformed1;
	//return tVec;
	VPoint result (0.0,0.0,0.0);
	result += (dirx*vec.x());
	result += (diry*vec.y());
	result += (dirz*vec.z());
	return result;
}

//#############################################################################

void CoordinateSystem::Serialize(Bofstream &outfile)const
{
	origin.Serialize(outfile);
	dirx.Serialize(outfile);
	diry.Serialize(outfile);
	dirz.Serialize(outfile);
}

//#############################################################################

void CoordinateSystem::Serialize(Bifstream &infile)
{
	origin.Serialize(infile);
	dirx.Serialize(infile);
	diry.Serialize(infile);
	dirz.Serialize(infile);	
}

//#############################################################################

void CoordinateSystem::Transform(const TransformationMatrix &tm)
{
	origin=tm.Transform(origin);
	dirx=tm.TransformVec(dirx);
	diry=tm.TransformVec(diry);
	dirz=tm.TransformVec(dirz);
}

bool CoordinateSystem::operator == (const CoordinateSystem &toCompare)const
{
	if (this==&toCompare)
		return true;
	vreal tol = 0.1 * VGlobalValues::ONEMICRON_TO_VUNIT;
	if(origin.Compare(toCompare.origin,tol) == false)
		return false;
	if (dirx.Compare(toCompare.dirx,tol) == false)
		return false;
	if (diry.Compare(toCompare.diry,tol) == false)
		return false;
	if(dirz.Compare(toCompare.dirz,tol) == false)
		return false;

	return true;
}

bool CoordinateSystem::operator != (const CoordinateSystem &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void CoordinateSystem::SetOrigin (const VPoint &newo)
{
	origin = newo;
}

bool CoordinateSystem::CheckAxis ()const
{
	VPoint ck1 = dirz ^ dirx;
	if (fabs(1-ck1.GetLength2()) > 1.e-4)
		return false;
	if (ck1.Compare (diry,1.e-6) == false)
		return false;
	return true;
}

void CoordinateSystem::TransformFromAbsToLocalCs (std::vector<VPoint> &pvec,const CoordinateSystem &cs,bool makeZzero)
{
	std::vector<VPoint> tmp;
	tmp.reserve (pvec.size());
	size_t mysize = pvec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VPoint p = pvec.at(i);
		p = cs.AbsoluteCoordToRelativeCoord (p);
		if (makeZzero)
			p.z(0.0);
		tmp.push_back (p);
	}
	pvec = tmp;
}

void CoordinateSystem::TransformFromLocalCsToAbs (std::vector<VPoint> &pvec,const CoordinateSystem &cs)
{
	std::vector<VPoint> tmp;
	tmp.reserve (pvec.size());
	size_t mysize = pvec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VPoint p = pvec.at(i);
		p = cs.RelativeCoordToAbsoluteCoord (p);
		tmp.push_back (p);
	}
	pvec = tmp;
}
CoordinateSystem CoordinateSystem::ArbitraryAxisAlg (const VPoint &N)
{
	//Let the given normal vector be called N.
	//	Let the world Y axis be called Wy, which is always (0,1,0).

	//	Let the world Z axis be called Wz, which is always (0,0,1).

	//	Here we are looking for the arbitrary X and Y axes to go with the normal N. They will be called Ax and Ay. N could also be called Az (the arbitrary Z axis) as follows:
	//If (abs (Nx) < 1/64) and (abs (Ny) < 1/64)  then
	//	Ax = Wy X N  (where ("X") is the cross-product operator).
	//	Otherwise,
	//	Ax = Wz X N.

	//	Scale Ax to unit length.

	//	The method of getting the Ay vector is

	//	Ay = N X Ax.  Scale Ay to unit length.
	VPoint Ax;
	if (fabs (N.x()) < 0.015625 && fabs(N.y())< 0.015625 )
	{
		Ax = UY ^ N;
	}
	else
	{
		Ax = UZ ^ N;
	}
	Ax.NormThis();
	VPoint Ay = N ^ Ax;
	Ay.NormThis();
	CoordinateSystem cs (VPoint(0.0),Ax,Ay,N);
	return cs;
}

bool CoordinateSystem::CompareDirectionVectors (const CoordinateSystem &cs , vreal compTol)const
{
	if (GetDirx().Compare (cs.GetDirx() , compTol) == false)
		return false;
	if (GetDiry().Compare (cs.GetDiry() , compTol) == false)
		return false;
	if (GetDirz().Compare (cs.GetDirz() , compTol) == false)
		return false;
	return true;
}

bool CoordinateSystem::IsWorkXYWorkPlanesSame(const CoordinateSystem &otherCs , vreal compTol)const
{
	vreal dotPro = GetDirz() % otherCs.GetDirz();
	if (fabs(dotPro - 1.0) > compTol && fabs(dotPro + 1.0) > compTol)
		return false;

	vreal distz = (GetOrigin() - otherCs.GetOrigin()) % GetDirz();
	if (fabs(distz) > compTol)
		return false;
	return true;
}

VStr CoordinateSystem::ToString()const
{
	VStr mystr(origin.ToString());
	mystr += dirx.ToString();
	mystr += diry.ToString();
	mystr += dirz.ToString();
	return mystr;
}

CoordinateSystem CoordinateSystem::FromString(VStr &str)
{
	VPoint myorigin = VPoint::FromString(str);
	VPoint mydirx = VPoint::FromString(str);
	VPoint mydiry = VPoint::FromString(str);
	VPoint mydirz = VPoint::FromString(str);
	return CoordinateSystem(myorigin, mydirx, mydiry, mydirz);
}

UnitVector CoordinateSystem::ProjectVectorOnXYPlaneAndNormalize(const VPoint& absVec)const
{
	VPoint relp0 = AbsoluteCoordToRelativeCoord(VPoint(0.0));
	VPoint relp1 = AbsoluteCoordToRelativeCoord(absVec);
	relp0.z(0.0);
	relp1.z(0.0);
	VPoint absp0 = RelativeCoordToAbsoluteCoord(relp0);
	VPoint absp1 = RelativeCoordToAbsoluteCoord(relp1);

	VPoint projectedVec = (absp1 - absp0);
	if (projectedVec.Compare(VPoint(0.0), EPSILON) == true)
	{
		throw VException("CoordinateSystem::ProjectVectorOnXYPlaneAndNormalize exception , projected vector length is zero", 1);
	}
	return projectedVec.Unit();
}

UnitVector CoordinateSystem::GetNormalOfTwoAxisVectorPlane(const UnitVector& ax0, const UnitVector& ax1 , int& isAxisVecColinear)// if 2 vectors are colinear , ArbitraryAxisAlgorithm is used for N
{
	vreal dotpro = ax0 % ax1;
	if (fabs(dotpro - 1.0) < EPSILON)
	{
		isAxisVecColinear = 1;
		CoordinateSystem cstemp = ArbitraryAxisAlg(ax0);
		return cstemp.GetDirx();
	}
	if (fabs(dotpro + 1.0) < EPSILON)
	{
		isAxisVecColinear = -1;
		CoordinateSystem cstemp = ArbitraryAxisAlg(ax1);
		return cstemp.GetDirx();
	}
	isAxisVecColinear = 0;
	VPoint csZ = ax0 ^ ax1;
	csZ.NormThis();
	return csZ;
}

vreal CoordinateSystem::GetShortestAngleBetween2UnitVector(const VPoint& v1, const VPoint& v2 , VPoint& rotationNVec)
{
	int isAxisVecColinear;
	rotationNVec = GetNormalOfTwoAxisVectorPlane(v1, v2, isAxisVecColinear);
	if (isAxisVecColinear != 0)
	{
		if (isAxisVecColinear == 1)
			return 0;
		else
			return 180.0;
	}
	VPoint csY = rotationNVec ^ v1;
	CoordinateSystem workCs(VPoint(0.0), v1, csY, rotationNVec);
	VPoint vec = workCs.AbsoluteVectorToRelativeVector(v2);
	vreal angle = PolarCalc2d::GetPolarAngle(vec.x(), vec.y());
	vreal angleNeg = angle - 360.0;
	if (fabs(angleNeg) < fabs(angle))
		return angleNeg;
	else
		return angle;
}

bool CoordinateSystem::IsAngularChangeSmallerThanLimit(const CoordinateSystem& cs1, const CoordinateSystem& cs2, vreal maxAngularChangeLimit
	,VPoint& angleChangeInfoPerDir,VPoint& rotationNVecX , VPoint& rotationNVecY , VPoint& rotationNVecZ)
{
	vreal deltaAxisAngleX = GetShortestAngleBetween2UnitVector(cs1.GetDirx(), cs2.GetDirx(),rotationNVecX);
	vreal deltaAxisAngleY = GetShortestAngleBetween2UnitVector(cs1.GetDiry(), cs2.GetDiry(),rotationNVecY);
	vreal deltaAxisAngleZ = GetShortestAngleBetween2UnitVector(cs1.GetDirz(), cs2.GetDirz(),rotationNVecZ);
	angleChangeInfoPerDir = VPoint(deltaAxisAngleX, deltaAxisAngleY, deltaAxisAngleZ);
	vreal maxDeltaAxisAngle = deltaAxisAngleX;
	if (maxDeltaAxisAngle < deltaAxisAngleY)
		maxDeltaAxisAngle = deltaAxisAngleY;
	if (maxDeltaAxisAngle < deltaAxisAngleZ)
		maxDeltaAxisAngle = deltaAxisAngleZ;
	if (maxDeltaAxisAngle > maxAngularChangeLimit)
	{
		return false;
	}
	return true;
}

UnitVector CoordinateSystem::BlendAxis(const UnitVector& ax0, const UnitVector& ax1, const UnitVector& midRotationN , vreal t)
{	
	int isAxisVecColinear;
	VPoint rotationNVec = GetNormalOfTwoAxisVectorPlane(ax0, ax1, isAxisVecColinear);
	if (isAxisVecColinear != 0)
	{
		if (isAxisVecColinear == -1)
		{
			// they are collinear but opposite directions - 180 degrees
			rotationNVec = midRotationN;
			vreal myAng = 180.0 * t;
			RotationMatrix R(VPoint(0.0), rotationNVec, myAng);
			VPoint curAx = R.Transform(ax0);
			return curAx;
		}
		else
			return ax0;// they are colinear and same direction - same vectors
	}
	// normal case , rotate around rotationNVec
	if (midRotationN % rotationNVec < 0.0)
		rotationNVec *= -1.0;
	vreal shortestAngle = GetShortestAngleBetween2UnitVector(ax0, ax1, rotationNVec);
	vreal curDeltaAngle = shortestAngle * t;
	RotationMatrix R(VPoint(0.0), rotationNVec, curDeltaAngle);
	VPoint curAx = R.Transform(ax0);
	return curAx;
}