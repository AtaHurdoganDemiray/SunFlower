// DraftEntityVec.cpp

#include "stdafx.h"
#include "DraftEntityVec.h"
#include "DraftEntity.h"
#include "ViewPointItem.h"
#include "ModelID.h"

DraftEntityVec::~DraftEntityVec()
{
	//for ( int i=0; i < Size(); i++ )
	//{
	//	DraftEntity * current = allitems[i];
	//	delete current;
	//}
}

int DraftEntityVec::Size () const
{
	return ( (int)allitems.size() );
}

DraftEntity * DraftEntityVec::operator [] ( int index )
{
	return ( allitems[index] );
}

unsigned int DraftEntityVec::InsertItem ( DraftEntity * newitem )
{
	allitems.push_back ( newitem );
	//newitem->SetHandle ( Size() );
	return Size();
}

DraftEntityVec::pos DraftEntityVec::Begin()const
{
	return allitems.begin();
}

DraftEntityVec::pos DraftEntityVec::End()const
{
	return allitems.end();
}

DraftEntityVec::upos DraftEntityVec::Begin()
{
	return allitems.begin();
}

DraftEntityVec::upos DraftEntityVec::End()
{
	return allitems.end();
}

//DraftEntity * DraftEntityVec::GetEntity(const ModelID &id)
//{
//	ViewPointItem * result= VpEntity::FindVpItem (id);//(ViewPointItem *)(id.GetModelPtr());
//	if (result->GetDeleted() == true)
//		return 0;
//	DraftEntity * res = (DraftEntity *)(result);
//	return res;	
//}

ViewPointItem * DraftEntityVec::GetModel (const ModelID &id)
{
	pos fi = allitems.begin();
	pos fe = allitems.end();
	for (fi;fi!=fe;++fi)
	{
		if ((*fi)->GetModelID() == id)
			return (ViewPointItem *)(*fi)->GetDraftEntityModel();
	}
	return 0;
}
