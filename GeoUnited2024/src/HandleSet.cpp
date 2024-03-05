/******************************************************************************
*               File: HandleSet.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.09.2003 10:08:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "HandleSet.h"

#include "ViewPointItem.h"

#include "VTransformable.h"
#include "Model.h"

int HandleSet::m_cellItemCount = 50;
int HandleSet::m_xCellCount = 100;
int HandleSet::m_yCellCount = 100;


HandleSet::HandleSet()
{
	m_listCells.resize(m_xCellCount, 0);
}

//#############################################################################

HandleSet::HandleSet (const std::vector<VTransformable *> &items)
{
	m_listCells.resize(m_xCellCount, 0);
	size_t mysize = items.size();
	for (size_t i=0;i<mysize;++i)
		AddItem (items.at(i)->GetTrModel()->GetModelID());
}

HandleSet::~HandleSet ()
{

}

//#############################################################################

const long HandleSet::Size()const 
{
	return (long)(mydata.size ());
}

//#############################################################################

bool HandleSet::AddItem(const ModelID &newid)
{
	int ind = (int)mydata.size();
	bool ck = AddToCellsIf(newid, ind);
	if (ck == true)
		mydata.push_back(newid);
	return ck;
}

void HandleSet::AddItemBrudeForceChecked (const ModelID &newid)
{
	//std::vector<ModelID>::iterator fi = std::find (mydata.begin(),mydata.end(),newid);
	//if (fi == mydata.end())
	//	mydata.push_back ( newid );
	//else
	//{
	//	int stp = 1;
	//}
	AddItem(newid);
}

//#############################################################################

//unsigned int HandleSet::operator [] ( int index )
//{
//	//if ( index < 0 || index > Size() )
//	//	throw VException ( ("set index invalid") , 0 );
//
//	return mydata.at(index).treeviewhandle;
//}

//#############################################################################

ModelID & HandleSet::GetID ( int index )
{
	//if ( index < 0 || index > Size() )
	//	throw VException ( ("set index invalid") , 0 );
	return ( mydata.at(index) );
}

//#############################################################################

void HandleSet::Clear()
{
	//mydata.clear();
	HandleSet tmp;
	*this = tmp;
}

void HandleSet::AddBackBrudeForceChecked(const HandleSet &toAdd)
{
	int mysize = toAdd.Size();
	for (int i = 0; i<mysize; ++i)
	{
		const ModelID &modid = toAdd.GetID(i);
		AddItemBrudeForceChecked(modid);
	}
}

void HandleSet::AddBack (const HandleSet &toAdd)
{
	int mysize=toAdd.Size();
	for (int i=0;i<mysize;++i)
	{
		const ModelID &modid = toAdd.GetID(i);
		AddItem (modid);
	}
	//mydata.insert(mydata.end(), toAdd.mydata.begin() , toAdd.mydata.end());
}

void HandleSet::RemoveItem(const ModelID &toremove)
{
	//std::vector<ModelID>::iterator i=mydata.begin();
	//std::vector<ModelID>::iterator e=mydata.end();
	//bool foundid=false;
	//for(i;i!=e;++i)
	//{
	//	if ((*i) == toremove)
	//	{
	//		foundid=true;
	//		break;
	//	}
	//}
	//if(foundid)
	//	mydata.erase(i);

	int mcounter = toremove.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	std::vector<std::list<std::pair<ModelID, int>> *> *vec = m_listCells.at(xno);
	if (vec == 0)
		return;
	std::list<std::pair<ModelID, int>> *curlist = vec->at(yno);
	if (curlist == 0)
		return;
	std::list<std::pair<ModelID, int>>::iterator iii = curlist->begin();
	std::list<std::pair<ModelID, int>>::iterator eee = curlist->end();
	for (iii; iii != eee; ++iii)
	{
		if (iii->first == toremove)
		{
			int ind = iii->second;
			curlist->erase(iii);
			int mysize = (int)mydata.size();
			std::vector<ModelID> tempvec(mysize-1);
			int counter = 0;
			for (int mi = 0; mi < mysize; ++mi)
			{
				if (mi != ind)
				{
					tempvec[counter] = mydata[mi];
					++counter;
				}
			}
			mydata = tempvec;
		}
	}
}

void HandleSet::Serialize(Bofstream &f)const
{
	int mysize=Size();
	f.WriteInt (mysize);
	for(int i=0;i<mysize;++i)
	{
		ModelID currid=mydata[i];
		currid.Serialize(f);
	}
}

void HandleSet::Serialize(Bifstream &f)
{
	int mysize;
	f.ReadInt (mysize);
	for(int i=0;i<mysize;++i)
	{
		ModelID nid;
		nid.Serialize(f);
		this->AddItem (nid);
	}
}

bool HandleSet::IsExist(const ModelID &id) const
{
	//vector<ModelID>::const_iterator i=mydata.begin();
	//vector<ModelID>::const_iterator e=mydata.end();
	//for(i;i!=e;++i)
	//{
	//	if (*i==id)
	//		return true;
	//}
	//return false;
	int ck = FindID(id);
	if (ck == -1)
		return false;
	else
		return true;
}

//const unsigned int HandleSet::operator [](int index) const
//{
//	return mydata.at (index).treeviewhandle;
//}

const ModelID & HandleSet::GetID(int index) const
{
	return mydata.at(index);
}

const ModelID & HandleSet::GetLastID() const
{
	if (mydata.size() == 0)
		throw VException (( IDS_HanGetExcSetIsE),1);
	return mydata.back();
}

ModelID & HandleSet::GetLastID()
{
	if (mydata.size() == 0)
		throw VException (( IDS_HanGetExcSetIsE),1);
	return mydata.back();
}

void HandleSet::EquilizeModelIDsWithHS(std::vector<ModelID *> &destiny , HandleSet &hs)
{
	if (destiny.size() != hs.Size())
	{
		CString temp;
		temp.Format(( IDS_InsEntDHanD),destiny.size() , hs.Size());
		LogFileWriter::WriteUnknownExceptionLog(( IDS_HanSizIsNotEquT));
		LogFileWriter::WriteUnknownExceptionLog ((LPCTSTR)temp);
		throw VException (( IDS_SerEquExcHanSiz),1);
	}
	size_t mysize = destiny.size();
	for (size_t i=0;i<mysize;++i)
	{
		ModelID *curid = destiny[i];
		ModelID &originalId = hs.GetID ((int)i);
		(*curid) = originalId;
		//curid->SetBirthDate(hs.GetID((int)i).GetBirthDate());
		//curid->SetBirthPlace(hs.GetID((int)i).GetBirthPlace());
		//ViewPointItem *vi = (ViewPointItem *)curid->GetModelPtr();
		//vi->m_id = *curid;
		//hs.GetID ((int)i).SetModelPtr(curid->GetModelPtr());
	}
}

void HandleSet::RemoveAll(const HandleSet &hs)
{
	int mysize = hs.Size();
	for (int i=0;i<mysize;++i)
	{
		const ModelID &curid = hs.GetID(i);
		this->RemoveIf (curid);
	}
}

bool HandleSet::RemoveIf(const ModelID &id)
{
	bool rsck = false;
	HandleSet *tmphs = new HandleSet();
	int mysize = Size();
	for (int i=0;i<mysize;++i)
	{
		const ModelID &curid = GetID (i);
		if (curid != id)
			tmphs->AddItem (curid);
		else
			rsck = true;
	}
	*this = *tmphs;
	return rsck;
}

void HandleSet::ReverseOrder()
{
	//std::vector<ModelID> reversed;
	//reversed.insert(reversed.end(), mydata.rbegin(), mydata.rend());
	//mydata = reversed;
	HandleSet *temphs = new HandleSet;
	int mysize = (int)mydata.size();
	for (int i = mysize - 1; i >= 0; --i)
	{
		temphs->AddItem(mydata.at(i));
	}
	*this = *temphs;
}

bool HandleSet::AddToCellsIf (const ModelID &id , int ind)
{
	int mcounter = id.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	if (m_listCells.at(xno) == 0)
	{
		m_listCells.at(xno) = new std::vector< std::list<std::pair<ModelID,int>> *>(m_yCellCount, 0);
	}
	std::list<std::pair<ModelID,int>> * curlist = m_listCells.at(xno)->at(yno);
	if (curlist == 0)
	{
		m_listCells.at(xno)->at(yno) = new std::list<std::pair<ModelID,int>>();
		m_listCells.at(xno)->at(yno)->push_back(std::pair<ModelID,int>(id,ind));
		return true;
	}
	else
	{
		std::list<std::pair<ModelID,int>>::iterator iii = curlist->begin();
		std::list<std::pair<ModelID,int>>::iterator eee = curlist->end();
		for (iii; iii != eee; ++iii)
		{
			if (iii->first == id)
				return false;
		}
		curlist->push_back(std::pair<ModelID,int>(id,ind));
		return true;
	}
}

int HandleSet::FindID(const ModelID &id)const // returns -1 if not found , otherwise index to mydata vector
{
	int mcounter = id.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	std::vector<std::list<std::pair<ModelID,int>> *> *vec = m_listCells.at(xno);
	if (vec == 0)
		return -1;
	std::list<std::pair<ModelID,int>> *curlist = vec->at(yno);
	if (curlist == 0)
		return -1;
	std::list<std::pair<ModelID,int>>::iterator iii = curlist->begin();
	std::list<std::pair<ModelID,int>>::iterator eee = curlist->end();
	for (iii; iii != eee; ++iii)
	{
		if (iii->first == id)
			return iii->second;
	}
	return -1;
}