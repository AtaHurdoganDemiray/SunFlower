// CopiousData.cpp

#include "stdafx.h"
#include "CopiousData.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


CopiousData::CopiousData()
{
	entitytype = 106;
}

void CopiousData::GetEntityIn (FileLines & mylines,int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	int vecsize = (int)myvect.size();
	if (vecsize <= 2)
		throw VException (( IDS_CopGetExcNotEno),1);

	m_IP = static_cast <int> (myvect[1]);
	m_N = static_cast <int> (myvect[2]);
	
	if (m_IP == 20 || m_IP == 21 || m_IP == 31 || m_IP == 32 || m_IP == 33 || m_IP == 34 || m_IP == 35 || m_IP == 36 || m_IP == 37 || m_IP == 38 || m_IP == 40 || m_IP == 63)
		m_IP = 1;
	if (m_IP == 12)
		m_IP = 2;
	if (m_IP == 13)
		m_IP = 3;

	int finalindex = 0;
	if (m_IP == 1)
	{
		if (vecsize <= 2*(m_N-1) + 5)
			throw VException (( IDS_CopGetExcNotEno),1);

		//this 2d points
		m_zoffset2d = myvect[3];
		tuples.resize (m_N);
		for (int i=0;i<m_N;i++)
		{
			tuples[i].resize(2);
			int begini = 2*i + 4;
			tuples[i][0] = (vreal)(myvect[begini]);
			tuples[i][1] = (vreal)(myvect[begini+1]);
			finalindex = begini + 2;
		}
	}
	else if (m_IP == 2)
	{
		if (vecsize <= 3*(m_N-1) + 5)
			throw VException (( IDS_CopGetExcNotEno),1);

		//this 3d points
		tuples.resize (m_N);
		for (int i=0;i<m_N;i++)
		{
			tuples[i].resize(3);
			int begini = 3*i + 3;
			tuples[i][0] = (vreal)(myvect[begini]);
			tuples[i][1] = (vreal)(myvect[begini+1]);
			tuples[i][2] = (vreal)(myvect[begini+2]);
			finalindex = begini + 3;
		}
	}
	else if (m_IP == 3)
	{
		if (vecsize <= 6*(m_N-1) + 8)
			throw VException (( IDS_CopGetExcNotEno),1);

		//this 6d points
		tuples.resize (m_N);
		for (int i=0;i<m_N;i++)
		{
			tuples[i].resize(6);
			int begini = 6*i + 3;
			tuples[i][0] = (vreal)(myvect[begini]);
			tuples[i][1] = (vreal)(myvect[begini+1]);
			tuples[i][2] = (vreal)(myvect[begini+2]);	
			tuples[i][3] = (vreal)(myvect[begini+3]);
			tuples[i][4] = (vreal)(myvect[begini+4]);
			tuples[i][5] = (vreal)(myvect[begini+5]);
			finalindex = begini + 6;
		}
	}
	if (finalindex > 0)
	{
		ReadModelId(finalindex, myvect);
	}
}

void CopiousData::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_IP,strvec);
	WriteParam (m_N,strvec);
	if (m_IP == 1)
	{
		WriteParam (m_zoffset2d , strvec);
		//this 2d points
		for (int i=0;i<m_N;i++)
		{
			WriteParam (tuples[i][0],strvec);
			WriteParam (tuples[i][1],strvec); 
		}
	}
	else if (m_IP == 2)
	{
		//this 3d points
		for (int i=0;i<m_N;i++)
		{
			WriteParam (tuples[i][0],strvec);
			WriteParam (tuples[i][1],strvec); 	
			WriteParam (tuples[i][2],strvec); 	
		}
	}
	else if (m_IP == 3)
	{
		//this 6d points
		for (int i=0;i<m_N;i++)
		{
			WriteParam (tuples[i][0],strvec);
			WriteParam (tuples[i][1],strvec); 	
			WriteParam (tuples[i][2],strvec); 	
			WriteParam (tuples[i][3],strvec);
			WriteParam (tuples[i][4],strvec); 	
			WriteParam (tuples[i][5],strvec); 	
		}
	}
	WriteModelId(strvec);
}

void CopiousData::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void CopiousData::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool CopiousData::IsPrimitiveEntity()const
//{
//	return true;
//}

//double CopiousData::GetSortNo ()const
//{
//	return 7.5;
//}

IgesEntity * CopiousData::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);

	CopiousData *mycopy = new CopiousData (*this);
	bool ck = CopyDE (*mycopy,mainSet,newSet);
	if (!ck)
		throw VException (( IDS_CopMakExc),1);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
