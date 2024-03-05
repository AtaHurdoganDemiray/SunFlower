/******************************************************************************
*               File: IgesEntity.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  28.11.2003 20:33:59 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH	                                 *
******************************************************************************/

#include "stdafx.h"
#include "IgesEntity.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "CastIf.h"
#include "TransformationMatrix.h"
#include "IgesFileWrite.h"

IgesEntity::IgesEntity()
{
	directorylineno = 1;
	//parameterlineno = 1;
	entitytype = 0;
	//m_subsetIndex = 0;
//	m_isRead = false;
}

//#############################################################################

IgesEntity::~IgesEntity()
{

}

//#############################################################################

void IgesEntity::WriteParam (const vreal &param , std::vector <VStr> &strvec)const
{
	ostringstream mystream;
	mystream << std::setiosflags(std::ios_base::scientific);
	mystream << setprecision (VGlobalValues::DOUBLE_SIGNIFICANT_DIGITS) <<param;
	strvec.push_back (mystream.str());
}

void IgesEntity::WriteParam (const int param , std::vector<VStr> &strvec)const
{
	ostringstream mystream;
	mystream << param;
	strvec.push_back (mystream.str());
}
void IgesEntity::WriteParam (const VStr &param , std::vector<VStr> &strvec)const
{
	size_t l = param.length();
	VOStrStream mystr;
	mystr << l;
	mystr << 'H';
	mystr << param.c_str();
	strvec.push_back(mystr.str());
}

void IgesEntity::GetDirectoryChild (std::vector<int> &childVec)const
{
	if (DE.pXform)
		childVec.push_back (DE.pXform);
	if (DE.pColor < 0)
		childVec.push_back (-DE.pColor);
}

void IgesEntity::GetAllParam ( DoubleVector &myvect , FileLines &mylines  )
{
	ProcessLine * aline;
	for ( unsigned int i = 0; i < mylines.size(); i++ )
	{
		aline = mylines[i];
		aline->GetAllParamData( myvect );
	}
}

void IgesEntity::GetAllParam ( std::vector<VStr> &myvect , FileLines &mylines  )
{
	ProcessLine * aline;
	for ( unsigned int i = 0; i < mylines.size(); i++ )
	{
		aline = mylines[i];
		aline->GetAllParamData( myvect );
	}
}

void IgesEntity::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

bool IgesEntity::CopyDE (IgesEntity &newe ,const  IgesEntitySet &mainSet,IgesEntitySet &newSet)const
{
	newe.DE = DE;
	if (DE.pXform)
	{
		// for catching error
		//if (newe.entitytype == 124)
		//{
		//	int stp = 1;
		//}
		IgesEntity *mat = mainSet.GetEntityPointer (DE.pXform);
		if (mat)
		{
			IgesEntity *newmat = mat->MakeCopyAndAddToSet(mainSet,newSet);
			newe.DE.pXform = newmat->directorylineno;
		}
	}
	if (DE.pColor < 0)
	{
		IgesEntity *col = mainSet.GetEntityPointer(-DE.pColor);
		if (col)
		{
			IgesEntity *newcol = col->MakeCopyAndAddToSet(mainSet,newSet);
			newe.DE.pColor = -newcol->directorylineno;
		}
	}
	newSet.SetDEParamLineBeginAndCount (newe);
	return true;
}

//IgesEntity & IgesEntity::operator =(const IgesEntity &toCopy)
//{
//	DE = toCopy.DE;
//	directorylineno = toCopy.directorylineno;
//	entitytype = toCopy.entitytype;
//	m_subsetIndex = toCopy.m_subsetIndex;
//	return *this;
//}

void IgesEntity::OverrideBigZCoordinates (double zLimit , double zWork)
{
	
}

const VStr & IgesEntity::GetModelIdStr()const
{
	return m_modelid;
}

void IgesEntity::SetModelIdStr(const VStr &id)
{
	m_modelid = id;
}

void IgesEntity::WriteModelId(std::vector<VStr> &strvec)const
{
	int idstrlen = (int)m_modelid.size();
	for (int i = 0; i < idstrlen; ++i)
	{
		const char &curchr = m_modelid.at(i);
		vreal tmppar = (vreal)(curchr);
		WriteParam(tmppar, strvec);
	}
}

void IgesEntity::ReadModelId(int starti, const DoubleVector &vec)
{
	int dsize = (int)vec.size();
	for (int i = starti; i < dsize; ++i)
	{
		vreal curpar = vec.at(i);
		char curchr = (char)(curpar);
		m_modelid += curchr;
	}
}
