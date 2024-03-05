// VTransformable.cpp

#include "stdafx.h"
#include "VTransformable.h"
#include "TransformData.h"
#include "VPoint.h"
#include "OperationTransformList.h"

double VTransformable::MAX_COORD_LIMIT = 10000000; // ten million
OperationTransformList * VTransformable::m_transformManager = 0;

VTransformable::VTransformable()
://m_initialTransform(0),
m_matModRef(-1)
{
}

VTransformable::~VTransformable()
{
	//if (m_initialTransform)
	//	delete m_initialTransform;
}

//void VTransformable::AddTransformation(TransformData *ptm)
//{
//	m_transformationHistory.push_back (ptm);
//}

//TransformationMatrix VTransformable::GetOverallMatrix() const
//{
//	TransformationMatrix overall;
//	//if (m_initialTransform)
//	//	overall = *m_initialTransform;
//
//	int mysize = static_cast<int>(m_transformationHistory.size());
//	for (int i=0; i<mysize; i++)
//	{
//		TransformData *currm = m_transformationHistory[i];
//		if (currm->isactive)
//		{
//			overall = overall * currm->m_tm;
//		}
//	}
//	return overall;
//}

//void VTransformable::SetActive(TransformationMatrix *ptm, bool isActive)
//{
//	int mysize = static_cast<int>(m_transformationHistory.size());
//	for (int i=0; i<mysize; i++)
//	{
//		TransformationMatrix *currm = m_transformationHistory[i];
//		if (currm == ptm)
//		{
//			m_isActive[i] = isActive;
//			return;
//		}
//	}
//	return;
//}

//TransformationMatrix VTransformable::GetOverallInverse()const
//{
//	TransformationMatrix tm = GetOverallMatrix();
//	return tm.GetInverse();
//}

//void VTransformable::ClearTransform(IgesEntitySet &entset)
//{
//	TransformationMatrix inv = GetOverallInverse();
//	int mysize = static_cast<int>(m_transformationHistory.size());
//	for (int i=0; i<mysize; i++)
//	{
//		TransformData *currm = m_transformationHistory[i];
//		currm->isactive = false;	
//	}	
//	DoTransform (inv,entset);
//}

//void VTransformable::UpdateTransform(IgesEntitySet &entset)
//{
//	TransformationMatrix tm = GetOverallMatrix();	
//	DoTransform (tm,entset);
//}

//void VTransformable::SetInitialTransformationMatrix(const TransformationMatrix &tm)
//{
//	if (m_initialTransform)
//		delete m_initialTransform;
//	m_initialTransform = new TransformationMatrix (tm);
//}

//void VTransformable::DoLastTransform(IgesEntitySet &entset)
//{
//	TransformData *lastt = m_transformationHistory.back();
//	DoTransform (lastt->m_tm,entset);
//}

const int VTransformable::GetMatModRef()const
{
	return m_matModRef;
}

void VTransformable::SetMatModRef(const int matModRef)
{
	m_matModRef = matModRef;
}

int VTransformable::GetIgesX() const
{
	return m_igesX;
}

void VTransformable::SetIgesX(int igesX)
{
	m_igesX = igesX;
}

void VTransformable::GetMinMax (const std::vector<VTransformable *> &trVec,VPoint &mn , VPoint &mx)
{
	mn = VPoint (MAX_NUMBER_LIMIT);
	mx = VPoint (MIN_NUMBER_LIMIT);
	size_t mysize = trVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curtr = trVec[i];
		const VPoint &curmn = curtr->GetMinPoint();
		const VPoint &curmx = curtr->GetMaxPoint();
		mn.ReplaceMin (curmn);
		mx.ReplaceMax (curmx);
	}
}

void VTransformable::GetMinMaxInCs (const CoordinateSystem &cs , const std::vector<VTransformable *> &trVec,VPoint &mn , VPoint &mx)
{
	mn = VPoint (MAX_NUMBER_LIMIT);
	mx = VPoint (MIN_NUMBER_LIMIT);
	size_t mysize = trVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curtr = trVec[i];
		const VPoint &curmn = curtr->GetMinPointInCs(cs);
		const VPoint &curmx = curtr->GetMaxPointInCs(cs);
		mn.ReplaceMin (curmn);
		mx.ReplaceMax (curmx);
	}
}

const TransformationMatrix & VTransformable::GetTm()const
{
	return m_tm;
}

void VTransformable::SetTm (const TransformationMatrix &tm)
{
	m_tm = tm;
}

TransformationMatrix VTransformable::GetBaseTm()const
{
	return TransformationMatrix();//returns identity matrix
}

TransformationMatrix VTransformable::GetDynamicTm ()const
{
	try
	{
	if (m_matModRef != -1)
	{
		OperationTransformList::Mat_Mod *mm = m_transformManager->GetAllMat().at(m_matModRef);
		return mm->m_tm.m_tm;
	}
	else
	{
		return TransformationMatrix ();
	}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,"VTransformable::GetDynamicTm");
		return TransformationMatrix();
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,"VTransformable::GetDynamicTm");
		return TransformationMatrix();
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Unknown exception ","VTransformable::GetDynamicTm");
		return TransformationMatrix();
	}
}

TransformationMatrix VTransformable::GetOverallTm()const
{
	TransformationMatrix tm = GetBaseTm();
	tm *= GetTm();
	tm *= GetDynamicTm();
	return tm;
}

TransformationMatrix VTransformable::GetTmForIgesExport()const
{
	return TransformationMatrix();
}

void VTransformable::UpdateMinMaxPointAlongCsX (const CoordinateSystem &cs , VPoint &mnx, VPoint &mxx)const
{

}

void VTransformable::UpdateMinMaxPointAlongCsX (const CoordinateSystem &cs , std::vector<VTransformable *> &trVec , VPoint &mnx , VPoint &mxx)
{
	int mysize = (int)trVec.size();
	for (int i=0;i<mysize;++i)
	{
		VTransformable *curtr = trVec.at(i);
		curtr->UpdateMinMaxPointAlongCsX(cs,mnx,mxx);
	}
}