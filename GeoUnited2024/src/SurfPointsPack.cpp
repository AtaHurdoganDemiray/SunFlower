// SurfPointsPack.cpp

#include "stdafx.h"
#include "SurfPointsPack.h"


#include "PointSet2d.h"


SurfPointsPack::SurfPointsPack ( const long &initialcapacity )
: modelpoints ( initialcapacity ) , normals ( initialcapacity ) , parampoints ( initialcapacity )
{

}

//#############################################################################

SurfPointsPack::SurfPointsPack ( const SurfPointsPack & Foo )
:parampoints(Foo.parampoints),modelpoints(Foo.modelpoints),normals(Foo.normals)
{

}

const VPoint& SurfPointsPack::GetModelPoint ( const long &i )const
{
	return (*(modelpoints[i]));
}

//#############################################################################

const VPoint2d& SurfPointsPack::GetParamPoint ( const long &i )const
{
	return (*(parampoints[i]));
}

//#############################################################################

const VPoint& SurfPointsPack::GetNormal ( const long &i )const
{
	return (*(normals[i]));
}

//#############################################################################

int SurfPointsPack::InsertPoint(const VPoint2d &par,const VPoint &mod,const VPoint &nor)
{
	parampoints.InsertPoint(par);
	modelpoints.InsertPoint(mod);
	normals.InsertPoint(nor);
	return parampoints.Size()-1;
}

bool SurfPointsPack::operator == (const SurfPointsPack &toCompare)const
{
	if(this==&toCompare)
		return true;
	
	if(parampoints!=toCompare.parampoints)
		return false;
	if(modelpoints!=toCompare.modelpoints)
		return false;
	if(normals!=toCompare.normals)
		return false;
	
	return true;
}

bool SurfPointsPack::operator != (const SurfPointsPack &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void SurfPointsPack::Serialize (Bofstream &outfile)const
{
	int version = 1;
	outfile.write ((VChr *)&version,sizeof(int));
	if (version == 1)
	{
		parampoints.Serialize(outfile);
		modelpoints.Serialize(outfile);
		normals.Serialize(outfile);
	}
	else
	{
		throw VException (( IDS_SurSerExc),1);
	}
}

void SurfPointsPack::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr *)&version , sizeof(int));
	if (version == 1)
	{
		parampoints.Serialize(infile);
		modelpoints.Serialize(infile);
		normals.Serialize(infile);
	}
	else
	{
		throw VException (( IDS_SurSerExc),2);
	}
}

void SurfPointsPack::RemoveDuplicate()
{
	std::vector<int>removed;
	int mysize = modelpoints.Size();
	parampoints.RemoveDuplicate(removed);
	PointSet tmpm (mysize);
	PointSet tmpn (mysize);
	int remsize = static_cast<int>(removed.size());
	for (int i=0;i<mysize;i++)
	{
		bool remck = false;
		for (int j=0;j<remsize;j++)
		{
			int remi = removed[j];
			if (remi == i)
			{
				remck =true;
				break;
			}
		}
		if (remck == false)
		{
			//point not removed
			tmpm.InsertPoint (* (modelpoints.operator [](i)) );
			tmpn.InsertPoint (* (normals.operator [](i)) );
		}
	}
	modelpoints=tmpm;
	normals=tmpn;
}

void SurfPointsPack::Transform (const TransformationMatrix &tm)
{
	modelpoints.Transform (tm);
	normals.TransformAsUnitVectors (tm);
}

void SurfPointsPack::Clear ()
{
	parampoints.Clear();
	modelpoints.Clear();
	normals.Clear();
}
