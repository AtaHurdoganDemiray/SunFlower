/******************************************************************************
*               File: LongVec.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.09.2003 13:59:26 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "LongVec.h"
#include "BasicDefinitions.h"
#include <algorithm>
using namespace std;

LongVec::LongVec()
{

}

//#############################################################################

LongVec::~LongVec()
{

}

//#############################################################################

long LongVec::operator [] ( long index ) const
{
	return  quad[index];
}

//#############################################################################

void LongVec::AddLong ( long item )
{
	quad.push_back ( item );
}

//#############################################################################

long LongVec::Size() const
{
	return (long)quad.size();
}

//#############################################################################

void LongVec::RemoveAll()
{
	quad.clear();
}

//#############################################################################

long & LongVec::At ( int index )
{
	return ( quad.at ( index ) );
}

void LongVec::Sort()
{
	std::sort (quad.begin(), quad.end());
}

void LongVec::RemoveDuplicate()
{
	if (!quad.size())
		return;

	std::sort(quad.begin(),quad.end());
	std::vector<long>::iterator pi = quad.begin();
	std::vector<long>::iterator i = quad.begin();
	++i;
	std::vector<long>::iterator e = quad.end();
	std::vector<long> tmp;
	for (i;i!=e;++i)
	{
		if (*pi != *i)
			tmp.push_back(*pi);
		++pi;
	}
	tmp.push_back(quad.back());
	quad = tmp;

	//int mysize = Size();
	//std::vector<long> tmp;
	//for (int i=0;i<mysize;i++)
	//{
	//	int currI =	operator[](i);
	//	bool dupl = false;
	//	for (int j=i+1;j<mysize;j++)
	//	{
	//		int nextI = operator[](j);
	//		if (nextI == currI)
	//		{
	//			dupl = true;
	//			break;
	//		}
	//	}
	//	if (dupl == false)
	//		tmp.push_back (currI);
	//}
	//quad = tmp;
}

std::vector<long> & LongVec::GetVector()
{
	return quad;
}	

const std::vector<long> & LongVec::GetVector()const
{
	return quad;
}
