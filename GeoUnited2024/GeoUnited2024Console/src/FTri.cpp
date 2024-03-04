// FTri.cpp

#include "stdafx.h"
#include "FTri.h"
#include "TransformationMatrix.h"

FTri::FTri ()
{

}
FTri::FTri (const VTriangleE &tri)
{
	VPoint p1 = tri.GetCorner1();
	VPoint p2 = tri.GetCorner2();
	VPoint p3 = tri.GetCorner3();

	v1[0] = (float)p1.x();
	v1[1] = (float)p1.y();
	v1[2] = (float)p1.z();

	v2[0] = (float)p2.x();
	v2[1] = (float)p2.y();
	v2[2] = (float)p2.z();

	v3[0] = (float)p3.x();
	v3[1] = (float)p3.y();
	v3[2] = (float)p3.z();
}
VTriangleE  FTri::GetVTri ()const
{
	VPoint p1 ((double)v1[0] , (double)v1[1] , (double)v1[2]);
	VPoint p2 ((double)v2[0] , (double)v2[1] , (double)v2[2]);
	VPoint p3 ((double)v3[0] , (double)v3[1] , (double)v3[2]);
	VTriangleE tri (p1,p2,p3);
	return tri;
}
void FTri::Transform(const TransformationMatrix &tm)
{
	sVector<double ,3> dv1 , dv2, dv3;
	dv1[0] = (double)v1[0];
	dv1[1] = (double)v1[1];
	dv1[2] = (double)v1[2];

	dv2[0] = (double)v2[0];
	dv2[1] = (double)v2[1];
	dv3[2] = (double)v2[2];

	dv3[0] = (double)v3[0];
	dv3[1] = (double)v3[1];
	dv3[2] = (double)v3[2];
	
	dv1 = tm.Transform(dv1);
	dv2 = tm.Transform(dv2);
	dv3 = tm.Transform(dv3);

	v1[0] = (float)dv1[0];
	v1[1] = (float)dv1[1];
	v1[2] = (float)dv1[2];

	v2[0] = (float)dv2[0];
	v2[1] = (float)dv2[1];
	v2[2] = (float)dv2[2];

	v3[0] = (float)dv3[0];
	v3[1] = (float)dv3[1];
	v3[2] = (float)dv3[2];
}

void FTri::Serialize(Bofstream &f)const
{
	int version = 1;
	f.WriteInt(version);
	if (version == 1)
	{
		f.WriteDouble((double)v1[0]);
		f.WriteDouble((double)v1[1]);
		f.WriteDouble((double)v1[2]);

		f.WriteDouble((double)v2[0]);
		f.WriteDouble((double)v2[1]);
		f.WriteDouble((double)v2[2]);

		f.WriteDouble((double)v3[0]);
		f.WriteDouble((double)v3[1]);
		f.WriteDouble((double)v3[2]);
	}
}

void FTri::Serialize(Bifstream &f)
{
	int version;
	f.ReadInt(version);
	if (version == 1)
	{
		double a[9];
		for (int i=0;i<9;++i)
			f.ReadDouble(a[i]);
		
		v1[0] = (float)a[0];
		v1[1] = (float)a[1];
		v1[2] = (float)a[2];

		v2[0] = (float)a[3];
		v2[1] = (float)a[4];
		v2[2] = (float)a[5];

		v3[0] = (float)a[6];
		v3[1] = (float)a[7];
		v3[2] = (float)a[8];
	}
}
