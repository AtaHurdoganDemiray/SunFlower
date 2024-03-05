// FMesh.cpp

#include "stdafx.h"
#include "FMesh.h"

#include "TriangleMesh.h"
#include "TransformationMatrix.h"
#include "VRefTriangleMesh.h"
#include "CoordinateSystem.h"
#include "SequentialLineSet.h"

FMesh::FMesh()
:m_tri (0) , m_isTransformed (false)
{

}
//FMesh::FMesh(std::list<float> *mystllist)
//{
//	m_tri = mystllist;
//	int mysize = (int)m_tri.size();
//	if ((mysize%12) != 0)
//		throw VException (("FMesh constructor exception , wrong size!"),1);
//}
FMesh::FMesh(const TriangleMesh &mesh)
: m_tri(0),m_isTransformed (false)
{
	int trisize = mesh.Size();
	MakeArray (trisize * 12);

	for (int i=0;i<trisize;++i)
	{
		const VTriangleE &curt = mesh.At (i);
		int bi = i *12;
		const VPoint &n = curt.GetNormal();
		const VPoint &c1 = curt.GetCorner1();
		const VPoint &c2 = curt.GetCorner2();
		const VPoint &c3 = curt.GetCorner3();
		m_tri[bi] = (float)n.x();
		m_tri[bi+1] = (float)n.y();
		m_tri[bi+2] = (float)n.z();
		m_tri[bi+3] = (float)c1.x();
		m_tri[bi+4] = (float)c1.y();
		m_tri[bi+5] = (float)c1.z();
		m_tri[bi+6] = (float)c2.x();
		m_tri[bi+7] = (float)c2.y();
		m_tri[bi+8] = (float)c2.z();
		m_tri[bi+9] = (float)c3.x();
		m_tri[bi+10] = (float)c3.y();
		m_tri[bi+11] = (float)c3.z();
	}
	CalcMinMax();
}

FMesh::FMesh(const VRefTriangleMesh *mesh,const VPoint *offsetVec,bool isReversed)
: m_tri(0),m_isTransformed (false)
{
	if (offsetVec == 0 && isReversed == false)
	{
		int trisize = mesh->Size();
		MakeArray (trisize * 12);

		for (int i=0;i<trisize;++i)
		{
			VTriangleE curt = mesh->operator [] (i);
			int bi = i *12;
			const VPoint &n = curt.GetNormal();
			const VPoint &c1 = curt.GetCorner1();
			const VPoint &c2 = curt.GetCorner2();
			const VPoint &c3 = curt.GetCorner3();
			m_tri[bi] = (float)n.x();
			m_tri[bi+1] = (float)n.y();
			m_tri[bi+2] = (float)n.z();
			m_tri[bi+3] = (float)c1.x();
			m_tri[bi+4] = (float)c1.y();
			m_tri[bi+5] = (float)c1.z();
			m_tri[bi+6] = (float)c2.x();
			m_tri[bi+7] = (float)c2.y();
			m_tri[bi+8] = (float)c2.z();
			m_tri[bi+9] = (float)c3.x();
			m_tri[bi+10] = (float)c3.y();
			m_tri[bi+11] = (float)c3.z();
		}
	}
	else if (offsetVec == 0 && isReversed == true)
	{
		int trisize = mesh->Size();
		MakeArray (trisize * 12);

		for (int i=0;i<trisize;++i)
		{
			VTriangleE curt = mesh->operator [] (i);
			int bi = i *12;
			const VPoint &n = curt.GetNormal();
			const VPoint &c1 = curt.GetCorner1();
			const VPoint &c2 = curt.GetCorner2();
			const VPoint &c3 = curt.GetCorner3();
			m_tri[bi] = (float)(n.x() *-1.0);
			m_tri[bi+1] = (float)(n.y() *-1.0);
			m_tri[bi+2] = (float)(n.z() * -1.0);
			m_tri[bi+3] = (float)c3.x();
			m_tri[bi+4] = (float)c3.y();
			m_tri[bi+5] = (float)c3.z();
			m_tri[bi+6] = (float)c2.x();
			m_tri[bi+7] = (float)c2.y();
			m_tri[bi+8] = (float)c2.z();
			m_tri[bi+9] = (float)c1.x();
			m_tri[bi+10] = (float)c1.y();
			m_tri[bi+11] = (float)c1.z();
		}
	}
	else if (offsetVec != 0 && isReversed == false)
	{
		int trisize = mesh->Size();
		MakeArray (trisize * 12);

		for (int i=0;i<trisize;++i)
		{
			VTriangleE curt = mesh->operator [] (i);
			int bi = i *12;
			const VPoint &n = curt.GetNormal();
			const VPoint &c1 = curt.GetCorner1();
			const VPoint &c2 = curt.GetCorner2();
			const VPoint &c3 = curt.GetCorner3();
			m_tri[bi] = (float)n.x();
			m_tri[bi+1] = (float)n.y();
			m_tri[bi+2] = (float)n.z();
			m_tri[bi+3] = (float)(c1.x()+offsetVec->x());
			m_tri[bi+4] = (float)(c1.y()+offsetVec->y());
			m_tri[bi+5] = (float)(c1.z()+offsetVec->z());
			m_tri[bi+6] = (float)(c2.x()+offsetVec->x());
			m_tri[bi+7] = (float)(c2.y()+offsetVec->y());
			m_tri[bi+8] = (float)(c2.z()+offsetVec->z());
			m_tri[bi+9] = (float)(c3.x()+offsetVec->x());
			m_tri[bi+10] = (float)(c3.y()+offsetVec->y());
			m_tri[bi+11] = (float)(c3.z()+offsetVec->z());
		}
	}
	else if (offsetVec != 0 && isReversed == true)
	{
		int trisize = mesh->Size();
		MakeArray (trisize * 12);

		for (int i=0;i<trisize;++i)
		{
			VTriangleE curt = mesh->operator [] (i);
			int bi = i *12;
			const VPoint &n = curt.GetNormal();
			const VPoint &c1 = curt.GetCorner1();
			const VPoint &c2 = curt.GetCorner2();
			const VPoint &c3 = curt.GetCorner3();
			m_tri[bi] = (float)(n.x()*-1.0);
			m_tri[bi+1] = (float)(n.y()*-1.0);
			m_tri[bi+2] = (float)(n.z()*-1.0);
			m_tri[bi+3] = (float)(c3.x()+offsetVec->x());
			m_tri[bi+4] = (float)(c3.y()+offsetVec->y());
			m_tri[bi+5] = (float)(c3.z()+offsetVec->z());
			m_tri[bi+6] = (float)(c2.x()+offsetVec->x());
			m_tri[bi+7] = (float)(c2.y()+offsetVec->y());
			m_tri[bi+8] = (float)(c2.z()+offsetVec->z());
			m_tri[bi+9] = (float)(c1.x()+offsetVec->x());
			m_tri[bi+10] = (float)(c1.y()+offsetVec->y());
			m_tri[bi+11] = (float)(c1.z()+offsetVec->z());
		}
	}
	CalcMinMax();
}
FMesh::FMesh(const SequentialLineSet &baseSl ,const VPoint &extrudeVec)
: m_tri(0),m_isTransformed (false)
{
	int linesize = baseSl.Size();
	MakeArray (linesize*2 *12);

	for (int i=0;i<linesize;++i)
	{
		VLine curl = baseSl.operator[] (i);
		//VTriangleE curt = mesh->operator [] (i);
		int bi = i *24;
		const VPoint &n = (curl.GetV() ^ extrudeVec).Unit();//curt.GetNormal();
		const VPoint &c1 = curl.GetP1();//curt.GetCorner1();
		const VPoint &c2 = curl.GetP2();
		VPoint c3 = c2 + extrudeVec;
		VPoint c4 = c1 + extrudeVec;
		m_tri[bi] = (float)n.x();
		m_tri[bi+1] = (float)n.y();
		m_tri[bi+2] = (float)n.z();
		m_tri[bi+3] = (float)c1.x();
		m_tri[bi+4] = (float)c1.y();
		m_tri[bi+5] = (float)c1.z();
		m_tri[bi+6] = (float)c2.x();
		m_tri[bi+7] = (float)c2.y();
		m_tri[bi+8] = (float)c2.z();
		m_tri[bi+9] = (float)c3.x();
		m_tri[bi+10] = (float)c3.y();
		m_tri[bi+11] = (float)c3.z();

		m_tri[bi+12] = (float)n.x();
		m_tri[bi+13] = (float)n.y();
		m_tri[bi+14] = (float)n.z();
		m_tri[bi+15] = (float)c3.x();
		m_tri[bi+16] = (float)c3.y();
		m_tri[bi+17] = (float)c3.z();
		m_tri[bi+18] = (float)c4.x();
		m_tri[bi+19] = (float)c4.y();
		m_tri[bi+20] = (float)c4.z();
		m_tri[bi+21] = (float)c1.x();
		m_tri[bi+22] = (float)c1.y();
		m_tri[bi+23] = (float)c1.z();
	}
	CalcMinMax();
}
FMesh::~FMesh ()
{
	delete [] m_tri;
}

void FMesh::Transform (const TransformationMatrix &tm)
{
	m_isTransformed = true;
	m_tm = tm * m_tm;
	int i = 0;
	float *a = GetArray();
	int asize = GetArraySize();
	if ((asize % 12) != 0)
		throw VException (( IDS_FacOnlExcArrSiz),1);
	
	while (i<asize)
	{
		VPoint n (a[i],a[i+1],a[i+2]);
		n = tm.TransformVec(n);
		n.NormThis();
		a[i] = (float)n[0];
		a[i+1] = (float)n[1];
		a[i+2] = (float)n[2];

		VPoint v1 (a[i+3],a[i+4],a[i+5]);
		v1 = tm.Transform (v1);
		a[i+3] = (float)v1[0];
		a[i+4] = (float)v1[1];
		a[i+5] = (float)v1[2];

		VPoint v2 (a[i+6],a[i+7],a[i+8]);
		v2 = tm.Transform (v2);
		a[i+6] = (float)v2[0];
		a[i+7] = (float)v2[1];
		a[i+8] = (float)v2[2];

		VPoint v3 (a[i+9],a[i+10],a[i+11]);
		v3 = tm.Transform (v3);
		a[i+9] = (float)v3[0];
		a[i+10] = (float)v3[1];
		a[i+11] = (float)v3[2];

		i+=12;
	}
	CalcMinMax ();
}

void FMesh::Serialize (Bofstream &f)const
{
	int version = 1;
	f.WriteInt (version);
	if (version == 1)
	{
		f.WriteInt(m_asize);
		for (int i=0;i<m_asize;++i)
		{
			f.WriteFloat (m_tri[i]);
		}
		f.WriteBool (m_isTransformed);
		m_tm.Serialize (f);
	}
}
void FMesh::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt(version);
	if(version == 1)
	{
		int mysize;
		f.ReadInt(mysize);
		MakeArray(mysize);
		for (int i=0;i<mysize;++i)
		{
			float tmp;
			f.ReadFloat (tmp);
			m_tri[i] = tmp;
		}
		f.ReadBool (m_isTransformed);
		m_tm.Serialize (f);
	}
}

VTriangleE FMesh::GetVTri(int i)const
{
	int j = 12 * i;
	VPoint c1((double)m_tri[j+3],(double)m_tri[j+4],(double)m_tri[j+5]);
	VPoint c2((double)m_tri[j+6],(double)m_tri[j+7],(double)m_tri[j+8]);
	VPoint c3((double)m_tri[j+9],(double)m_tri[j+10],(double)m_tri[j+11]);
	VTriangleE t (c1,c2,c3);
	//if (m_isTransformed)
	//{
	//	t = m_tm.Transform (t);
	//}
	return t;
}

void FMesh::GetTriCorners (int i,VPoint &c1 , VPoint &c2 , VPoint &c3)const
{
	int j = 12 * i;
	c1 = VPoint ((double)m_tri[j+3],(double)m_tri[j+4],(double)m_tri[j+5]);
	c2 = VPoint ((double)m_tri[j+6],(double)m_tri[j+7],(double)m_tri[j+8]);
	c3 = VPoint ((double)m_tri[j+9],(double)m_tri[j+10],(double)m_tri[j+11]);
	
	//if (m_isTransformed)
	//{
	//	c1 = m_tm.Transform (c1);
	//	c2 = m_tm.Transform (c2);
	//	c3 = m_tm.Transform (c3);
	//}
}

int FMesh::Size()const
{
	return (int) (m_asize/12);
}

VPoint FMesh::GetMin()const
{
	return m_min;
}
VPoint FMesh::GetMax()const
{
	return m_max;
}

void FMesh::CalcMinMax ()
{
	VPoint mymin (MAX_NUMBER_LIMIT);
	VPoint mymax (MIN_NUMBER_LIMIT);
	size_t mysize = Size();
	for (size_t i=0;i<mysize;++i)
	{
		//VTriangleE tri = GetVTri((int)i);
		VPoint c1,c2,c3;
		GetTriCorners ((int)i,c1,c2,c3);
		mymin.ReplaceMin (c1);//(tri.GetCorner1());
		mymin.ReplaceMin (c2);//(tri.GetCorner2());
		mymin.ReplaceMin (c3);//(tri.GetCorner3());
		mymax.ReplaceMax (c1);//(tri.GetCorner1());
		mymax.ReplaceMax (c2);//(tri.GetCorner2());
		mymax.ReplaceMax (c3);//(tri.GetCorner3());
	}
	m_min = mymin;
	m_max = mymax;
}

float * FMesh::GetArray()
{
	return m_tri;
}

const float * FMesh::GetArray()const
{
	return m_tri;
}

void FMesh::MakeArray (int asize)
{
	DestroyArray();
	m_asize = asize;
	m_tri = new float [asize];
}

void FMesh::DestroyArray()
{
	if (m_tri)
		delete [] m_tri;
	m_tri = 0;
	m_asize = 0;
}

int FMesh::GetArraySize() const
{
	return m_asize;
}

const VPoint FMesh::GetMinPointInCs(const CoordinateSystem &cs) const
{
	VPoint mymin (MAX_NUMBER_LIMIT);
	int mysize = Size();
	for (int i=0;i<mysize;++i)
	{
		VPoint c1,c2,c3;
		GetTriCorners (i,c1,c2,c3);
		mymin.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (c1));
		mymin.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (c2));
		mymin.ReplaceMin (cs.AbsoluteCoordToRelativeCoord (c3));
	}
	return mymin;
}
const VPoint FMesh::GetMaxPointInCs (const CoordinateSystem &cs) const
{
	VPoint mymax (MIN_NUMBER_LIMIT);
	int mysize = Size();
	for (int i=0;i<mysize;++i)
	{
		VPoint c1,c2,c3;
		GetTriCorners (i,c1,c2,c3);
		mymax.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (c1));
		mymax.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (c2));
		mymax.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (c3));
	}
	return mymax;
}

void FMesh::GetMinMaxAlongXInCs (const CoordinateSystem &cs , VPoint &mn , VPoint &mx)const
{
	vreal mymax (MIN_NUMBER_LIMIT);
	vreal mymin (MAX_NUMBER_LIMIT);
	int mysize = Size();
	for (int i=0;i<mysize;++i)
	{
		VPoint c1,c2,c3;
		GetTriCorners (i,c1,c2,c3);
		VPoint c1r =  cs.AbsoluteCoordToRelativeCoord (c1);
		VPoint c2r = cs.AbsoluteCoordToRelativeCoord (c2);
		VPoint c3r = cs.AbsoluteCoordToRelativeCoord (c3);
		if (c1r.x() < mymin)
		{
			mn = c1;
			mymin = c1r.x();
		}
		if (c2r.x() < mymin)
		{
			mn = c2;
			mymin = c2r.x();
		}
		if (c3r.x() < mymin)
		{
			mn = c3;
			mymin = c3r.x();
		}
		if (c1r.x() > mymax)
		{
			mx = c1;
			mymax = c1r.x();
		}
		if (c2r.x() > mymax)
		{
			mx = c2;
			mymax = c2r.x();
		}
		if (c3r.x() > mymax)
		{
			mx = c3;
			mymax = c3r.x();
		}
	}
}

VPoint FMesh::GetCenterOfGravity(vreal &totalArea)const
{
	int mySize = Size();
	totalArea = 0.0;
	VPoint sigmaP (0.0);
	for (int i=0;i<mySize;++i)
	{
		VTriangleE t = this->GetVTri(i);
		VPoint cg = t.GetCenterOfMass();
		vreal A = t.GetArea();
		totalArea += A;
		sigmaP += (A * cg);
	}
	return (sigmaP / totalArea);
}