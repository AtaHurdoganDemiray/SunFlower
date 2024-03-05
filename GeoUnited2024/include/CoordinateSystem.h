// CoordinateSystem.h

#ifndef __CoordinateSystem_h__
#define __CoordinateSystem_h__

#include "VPoint.h"
class Bofstream;
class Bifstream;
class TransformationMatrix;

struct CoordinateSystem
{
public:
	CoordinateSystem ();
	CoordinateSystem ( const VPoint & _origin );
	CoordinateSystem ( const VPoint & _origin 
						, const VPoint & _dirx
						, const VPoint & _diry
						, const VPoint & _dirz );
	
	CoordinateSystem ( const CoordinateSystem & Foo );
	
	const CoordinateSystem & operator = ( const CoordinateSystem &Foo );
	const VPoint & GetOrigin() const;
	const VPoint & GetDirx() const;
	const VPoint & GetDiry() const;
	const VPoint & GetDirz() const;
	VPoint AbsoluteCoordToRelativeCoord ( const VPoint & absolute ) const;
	VPoint RelativeCoordToAbsoluteCoord ( const VPoint & relative ) const;
	VPoint AbsoluteVectorToRelativeVector(const VPoint &vec)const;
	VPoint RelativeVectorToAbsoluteVector(const VPoint &vec)const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void Transform(const TransformationMatrix &tm);
	bool operator == (const CoordinateSystem &toCompare)const;
	bool operator != (const CoordinateSystem &toCompare)const;
	void SetOrigin (const VPoint &newo);
protected:
	VPoint origin;
	VPoint dirx;
	VPoint diry;
	VPoint dirz;
public:
	bool CheckAxis ()const;
	static void TransformFromAbsToLocalCs (std::vector<VPoint> &pvec,const CoordinateSystem &cs,bool makeZzero = false);
	static void TransformFromLocalCsToAbs (std::vector<VPoint> &pvec,const CoordinateSystem &cs);
	static CoordinateSystem ArbitraryAxisAlg (const VPoint &N);
	bool CompareDirectionVectors (const CoordinateSystem &cs,vreal compTol)const;// return true if directions are similar within tolerance

	// z directions must be parallel - may be reversed (opposite)  and dirX , dirY may not be same but X-Y plane is the same
	bool CoordinateSystem::IsWorkXYWorkPlanesSame(const CoordinateSystem &otherCs,vreal compTol)const;
	VStr ToString()const;
	static CoordinateSystem FromString(VStr &str);
	UnitVector ProjectVectorOnXYPlaneAndNormalize(const VPoint& absVec)const;
	static UnitVector GetNormalOfTwoAxisVectorPlane(const UnitVector& ax0, const UnitVector& ax1 , int& isAxisVecColinear);// if 2 vectors are colinear , ArbitraryAxisAlgorithm is used for N
	//,if the are not colinear isAxisVecColinear is made 0 , isAxisVecColinear is 1 if axis are colinear and same direction , it is -1 if colinear and opposite direction
	static bool IsAngularChangeSmallerThanLimit(const CoordinateSystem& cs1, const CoordinateSystem& cs2, vreal maxAngularChangeLimit
		,VPoint& angleChangeInfoPerDir , VPoint& rotationNVecX , VPoint& rotationNVecY , VPoint& rotationNVecZ);
	static vreal GetShortestAngleBetween2UnitVector(const VPoint& v1, const VPoint& v2 , VPoint& rotationNVec);
	static UnitVector BlendAxis(const UnitVector& ax0, const UnitVector& ax1 , const UnitVector& midRotationN , vreal t);

};


#endif //__CoordinateSystem_h__
