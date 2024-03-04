// FMesh.h

#ifndef __FMesh_h__
#define __FMesh_h__

#include "FTri.h"
#include "Bofstream.h"
#include "Bifstream.h"
#include "TransformationMatrix.h"
#include "VPoint.h"

class TriangleMesh;
class TransformationMatrix;
class VRefTriangleMesh;
struct CoordinateSystem;
class SequentialLineSet;

class FMesh
{
public:
	friend class VTriangles;
	~FMesh();
	FMesh (const TriangleMesh &mesh);
	FMesh (const VRefTriangleMesh *mesh,const VPoint *offsetVec , bool isReversed);
	FMesh (const SequentialLineSet &sl , const VPoint &extrudeVec);
	void Transform (const TransformationMatrix &tm);
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	VTriangleE GetVTri (int i)const;
	int Size ()const;
	VPoint GetMin()const;
	VPoint GetMax()const;
	void CalcMinMax ();
	float * GetArray();
	const float *GetArray()const;
	void MakeArray (int asize);
	void DestroyArray ();
	int GetArraySize ()const;
	const VPoint GetMinPointInCs(const CoordinateSystem &cs) const;
	const VPoint GetMaxPointInCs(const CoordinateSystem &cs) const;
	void GetTriCorners (int i,VPoint &c1 , VPoint &c2 , VPoint &c3)const;
private:
	FMesh ();
	float * m_tri;
	int m_asize;
	TransformationMatrix m_tm;
	bool m_isTransformed;
	VPoint m_min;
	VPoint m_max;
public:
	void GetMinMaxAlongXInCs (const CoordinateSystem &cs , VPoint &absMn , VPoint &absMx)const;
	VPoint GetCenterOfGravity (vreal &totalArea)const;
};

#endif