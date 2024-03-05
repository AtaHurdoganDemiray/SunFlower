// OperationTransform.cpp

#include "stdafx.h"
#include "OperationTransform.h"
#include "ViewPointItemTypes.h"
#include "..\UI\resource.h"
#include "HandleSet.h"
#include "ModelList.h"

#include "OperationTransformList.h"

HandleSet OperationTransform::s_hs;
TransformationMatrix OperationTransform::s_tm;

OperationTransform::OperationTransform  ( const TransformationMatrix &tm
										, const std::vector<VTransformable *> & sel_items 
										, VStr type 
										, VStr name)
:Model(type,name)
,m_data (tm)/*,m_active(true)*/,m_selitems(sel_items)
{
	m_data.isactive=true;
}

void OperationTransform::OnTreeViewClicked ()
{
	//visible = (!visible);
	//m_data.isactive=visible;
}

void OperationTransform::OnTreeViewDoubleClicked ()
{

}

void OperationTransform::OnTreeViewRightClicked (const VStr &commandcap)
{

}

const VStr OperationTransform::GetModelTypeEng() const
{
	return ViewPointItemTypes::TransformEng();
}

bool OperationTransform::IsExist(std::vector<VTransformable*> &vec, VTransformable *searchitem)
{
	size_t mysize = vec.size();
	for (size_t i=0;i<mysize;++i)
	{
		VTransformable *curitem = vec.at(i);
		if (curitem == searchitem)
			return true;
	}
	return false;
}

void OperationTransform::Serialize (Bofstream &f)
{
	int version = 1;
	f.WriteInt (version);
	if (version >= 1)
	{
		s_hs.Serialize(f);
		s_tm.Serialize (f);
	}
}
void OperationTransform::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt (version);
	if (version >= 1)
	{
		s_hs.Serialize (f);
		s_tm.Serialize (f);
	}
}
OperationTransform * OperationTransform::MakeOperationTransform(ModelList &mlist,OperationTransformList &trlist)
{	
	std::vector<VTransformable *> selitems;	
	int mysize = s_hs.Size();
	for (int i=0;i<mysize;++i)
	{
		ModelID &curid = s_hs.GetID (i);
		Model *curmodel = mlist.GetModel (curid);
		if (curmodel != 0)
		{
			VTransformable *curtr = curmodel->GetTransformable ();
			selitems.push_back (curtr);
		}
		else
			LogFileWriter::WriteUnknownExceptionLog ( ( IDS_OpeSerErrNoMatM));
	}
	OperationTransform *newoptr = &trlist.MakeOperationTransform (s_tm , selitems);
	return newoptr;
}
