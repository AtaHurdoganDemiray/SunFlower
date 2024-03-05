// Face.cpp

#include "stdafx.h"
#include "Face.h"

Face::Face ()
{
	entitytype = 510;
}

void Face::GetEntityIn(FileLines & mylines, int poffs)
{
	DoubleVector myvect;
	GetAllParam(myvect, mylines);
	if (myvect.size() <= 3)
		throw VException("Face::GetEntityIn exception , parameter count is wrong", 1);
	m_SURF = static_cast<int>(myvect[1])+poffs;
	m_N = static_cast<int>(myvect[2]);
	m_OF = static_cast<int>(myvect[3]);
	
	for (int li = 0; li < m_N; ++li)
	{
		int curloopDE = static_cast<int>(myvect.at(li + 4))+poffs;
		m_LOOP.push_back(curloopDE);
	}
}
void Face::WriteEntityOut (std::vector <VStr> &strvec)const
{

}
void Face::GetChildren (std::vector<int> &childVec)const
{

}
void Face::AcceptVisitor (IgesEntityVisitor &visitor)const
{

}
IgesEntity * Face::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	return 0;
}
