// ModelList.cpp: implementation of the ModelList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicDefinitions.h"
#include "ModelList.h"
#include "HandleSet.h"
#include "ModelSelectionFilters.h"
#include "SelectionFlags.h"
#include "DoubleList.h"
#include "ModelID.h"
#include "ViewPointItem.h"
#include "Model.h"
#include "VTransformable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//vreal ModelList::m_invXYZRes = 0.2;

int ModelList::m_cellItemCount = 500;
int ModelList::m_xCellCount = 1000;
int ModelList::m_yCellCount = 1000;

ModelList::ModelList()
{
	//iterating = false;
	//modelhandle = 0;
	m_listCells.resize(m_xCellCount, 0);
}

ModelList::~ModelList()
{
}

//#############################################################################

//Model * ModelList::BeginIterating () 
//{
//	if ( !size() )
//	{
//		return 0;
//	}

//	iterating = true;
//	position = begin();
//	return *(position++);
//}

//#############################################################################

//Model* ModelList::Iterate() 
//{

//	if ( !size() )
//	{
//		return 0;
//	}
//	else
//	{
//		if (iterating)
//		{
//			if ( position != end() )
//			{
//				return *(position++);
//			}
//			else
//			{
//				iterating = false;
//				position = end();
//				return 0;
//			}
//		}
//		else
//		{
//			position = begin();
//			iterating = true;
//			return *position;
//		}
//	}
//}

//#############################################################################

//Model* ModelList::IterateBackwards() 
//{
//	if ( !size() )
//	{
//		return 0;
//	}
//	else
//	{
//		if (iterating)
//		{
//			if ( position != begin() )
//			{
//				return *(position--);
//			}
//			else
//			{
//				iterating = false;
//				position = end();
//				return 0;
//			}
//		}
//		else
//		{
//			position = end();
//			iterating = true;
//			return *position;
//		}
//	}

//}

//#############################################################################

//void ModelList::DeleteAll()
//{
//	for (position = begin(); position != end(); position++ )
//	{
//		Model * p_model = * position; 
//		delete p_model;
//	}
//}

//#############################################################################

//void ModelList::AddModel(Model* p_model)
//{
//	push_back( p_model );	
//	if ( alltypes.IsTypeSelectable(p_model->GetModelType()) == false ) 
//		alltypes.AddSelectableType ( p_model->GetModelType () );
//}

//#############################################################################

//bool ModelList::GetMinMaxOfAllVisibleModels ( VPoint & min , VPoint & max )
//{
//	min.x(1.0/ EPSILON); min.y(1/EPSILON); min.z(1/EPSILON);
//	max.x(-1.0/EPSILON); max.y(-1/EPSILON); max.z(-1/EPSILON);
//
//	// returns false if there is no visible model
//	bool returnval = false;
//
//	Model * pmodel = BeginIterating();
//
//	while ( pmodel != 0 )
//	{
//		if ( pmodel->IsVisible() )
//		{
//			//pmodel->UpdateMinMax();
//			min.ReplaceMin ( pmodel->GetMin() );
//			max.ReplaceMax ( pmodel->GetMax() );
//			returnval = true;
//		}
//
//		pmodel = Iterate();
//	}
//	
//	if (returnval == false)
//	{
//		VPoint temp (10.,10.,10.);
//		min = -temp;
//		max = temp;
//		returnval = true;
//	}
//
//	return returnval;
//}

//#############################################################################

//Model * ModelList::GetModelByHandle ( unsigned int handle ) 
//{
//	ViewPointItem * result=(ViewPointItem *)(handle);
//	if (result->GetDeleted() == true)
//		return 0;
//	Model * res = (Model *)(result);
//	return res;	
//
//	//Model * p_model = BeginIterating();
//
//	//while ( p_model != NULL )
//	//{
//	//	if ( p_model->GetTreeViewHandle() == handle )
//	//		return p_model;
//
//	//	p_model = Iterate ();
//	//}
//
//	//return 0;
//	
//}

//#############################################################################

//const SelectableTypes & ModelList::GetAllTypes ()
//{
//	return alltypes;
//}

/*
void ModelList::SetCurrCoordSystem ( const CoordinateSystem * cs )
{
	currCoordSystem = cs;
}
*/

//void ModelList::ReverseVisibleOfAllModels ()
//{
//	Model * pmodel = BeginIterating();
//	while ( pmodel != 0 )
//	{
//		bool reversed = !(pmodel->IsVisible());
//		pmodel->SetVisible (reversed);
//		pmodel = Iterate();
//	}	
//}

//void ModelList::DeleteModel (unsigned int handle)
//{
//	//std::list<Model *>::iterator i = begin();
//	//std::list<Model *>::iterator e = end();
//	//for(i;i!=e;i++)
//	//{
//	//	if((*i)->GetTreeViewHandle()==handle)
//	//	{
//	//		Model *p = *i;
//	//		erase (i);
//	//		delete p;
//	//		return;
//	//	}
//	//}
//}

Model * ModelList::GetModel(const ModelID &id)const
{
	////ViewPointItem * result=(ViewPointItem *)(id.GetModelPtr());
	//////if (result->GetDeleted() == true)
	//////	return 0;
	////if (result == 0)
	////	throw VException (("ModelList::GetModel exception:modelid dont have model pointer"),1);
	////Model * res = (Model *)(result);
	////return res;	
	//std::list<Model *>::const_iterator i = begin();
	//std::list<Model *>::const_iterator e = end();
	//for (i;i!=e;++i)
	//{
	//	if ((*i)->GetModelID() == id)
	//		return *i;
	//}
	////throw VException (("ModelList::GetModel exception:modelid dont have model pointer"),1);

	//VPoint cen;
	//vreal rad;
	//id.GetCenterAndRadius(cen, rad);
	//int xi = (int)(cen.x() * m_invXYZRes);
	//int yi = (int)(cen.y() * m_invXYZRes);
	//int zi = (int)(cen.z() * m_invXYZRes);
	////int radi = (int)(rad * 1000.0);
	//citrx fix = m_xyzModelMap.find(xi);
	//if (fix == m_xyzModelMap.end())
	//{
	//	throw VException("ModelList::GetModel exception , model not found , id missing", 1);//return 0;
	//}
	//else
	//{
	//	citry fiy = fix->second.find(yi);
	//	if (fiy == fix->second.end())
	//	{
	//		throw VException("ModelList::GetModel exception , model not found , id missing",1);//return 0;
	//	}
	//	else
	//	{
	//		citrz fiz = fiy->second.find(zi);
	//		if (fiz == fiy->second.end())
	//		{
	//			throw VException("ModelList::GetModel exception , model not found , id missing", 1);//return 0;
	//		}
	//		else
	//		{
	//			const std::list<Model *> &curList = fiz->second.second;
	//			std::list<Model*>::const_iterator ii = curList.begin();
	//			std::list<Model*>::const_iterator ee = curList.end();
	//			for (ii; ii != ee; ++ii)
	//			{
	//				Model *tempm = *ii;
	//				if (tempm->GetModelID() == id)
	//					return tempm;
	//			}
	//		}
	//	}
	//}
	//throw VException("ModelList::GetModel exception , model not found , id missing", 1);//return 0;
	int mcounter = id.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	std::vector<std::list<Model *> *> *vec = m_listCells.at(xno);
	if (vec == 0)
		throw VException("ModelList::GetModel exception , model not found , id missing", 1);
	std::list<Model *> *curlist = vec->at(yno);
	if (curlist == 0)
		throw VException("ModelList::GetModel exception , model not found , id missing", 1);
	std::list<Model *>::iterator iii = curlist->begin();
	std::list<Model*>::iterator eee = curlist->end();
	for (iii; iii != eee; ++iii)
	{
		if ((*iii)->GetModelID() == id)
			return (*iii);
	}
	throw VException("ModelList::GetModel exception , model not found , id missing", 1);
}
Model * ModelList::GetModelNoThrow(const ModelID &id)const

{
	////ViewPointItem * result=(ViewPointItem *)(id.GetModelPtr());
	//////if (result->GetDeleted() == true)
	//////	return 0;
	////if (result == 0)
	////	throw VException (("ModelList::GetModel exception:modelid dont have model pointer"),1);
	////Model * res = (Model *)(result);
	////return res;	
	//std::list<Model *>::const_iterator i = begin();
	//std::list<Model *>::const_iterator e = end();
	//for (i;i!=e;++i)
	//{
	//	if ((*i)->GetModelID() == id)
	//		return *i;
	//}
	////throw VException (("ModelList::GetModel exception:modelid dont have model pointer"),1);


	//VPoint cen;
	//vreal rad;
	//id.GetCenterAndRadius(cen, rad);
	//int xi = (int)(cen.x() * m_invXYZRes);
	//int yi = (int)(cen.y() * m_invXYZRes);
	//int zi = (int)(cen.z() * m_invXYZRes);
	//citrx fix = m_xyzModelMap.find(xi);
	//if (fix == m_xyzModelMap.end())
	//{
	//	return 0;
	//}
	//else
	//{
	//	citry fiy = fix->second.find(yi);
	//	if (fiy == fix->second.end())
	//	{
	//		return 0;
	//	}
	//	else
	//	{
	//		citrz fiz = fiy->second.find(zi);
	//		if (fiz == fiy->second.end())
	//		{
	//			return 0;
	//		}
	//		else
	//		{
	//			const std::list<Model *> &curList = fiz->second.second;
	//			std::list<Model*>::const_iterator ii = curList.begin();
	//			std::list<Model*>::const_iterator ee = curList.end();
	//			for (ii; ii != ee; ++ii)
	//			{
	//				Model *tempm = *ii;
	//				if (tempm->GetModelID() == id)
	//					return tempm;
	//			}
	//		}
	//	}
	//}
	//return 0;
	int mcounter = id.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	std::vector<std::list<Model *> *> *vec = m_listCells.at(xno);
	if (vec == 0)
		return 0;
	std::list<Model *> *curlist = vec->at(yno);
	if (curlist == 0)
		return 0;
	std::list<Model *>::iterator iii = curlist->begin();
	std::list<Model*>::iterator eee = curlist->end();
	for (iii; iii != eee; ++iii)
	{
		if ((*iii)->GetModelID() == id)
			return (*iii);
	}
	return 0;
}

const bool IsModelIDsEqual (const Model *m1 , const Model *m2)
{
	if (m1->GetModelID()== m2->GetModelID())
		return true;
	else
		return false;
}
//bool ModelList::SearchForModelID (const ModelID &id,ModelID &fid)
//{
//	//pos i = begin();
//	//pos e = end();
//	//for (i;i!=e;++i)
//	//{
//	//	if ((*i)->GetModelID() == id)
//	//	{
//	//		fid = (*i)->GetModelID();
//	//		return true;
//	//	}
//	//}
//	return false;
//}

bool ModelList::AddModelIf(Model *m)
{
	//VTransformable *tr = m->GetTransformable();
	//VPoint cen(0.0);
	//vreal rad(0.0);
	//if (tr != 0)
	//{
	//	VPoint mn = tr->GetMinPoint();
	//	VPoint mx = tr->GetMaxPoint();
	//	cen = (mn+mx)*0.5;
	//	rad = (mx - mn).GetLength() * 0.5;
	//}
	//m->GetModelID().ResetCenterAndRadius(cen, rad);
	//int xi = (int)(cen.x() * m_invXYZRes);
	//int yi = (int)(cen.y() * m_invXYZRes);
	//int zi = (int)(cen.z() * m_invXYZRes);
	//int radi = (int)(rad * 1000);
	//itrx fix = m_xyzModelMap.find(xi);
	//if (fix == m_xyzModelMap.end())
	//{
	//	std::map<int, std::map<int, std::pair<int,std::list<Model*>>>>tempYZ;
	//	std::map<int, std::pair<int,std::list<Model*>>> tempZ;
	//	std::pair<int,std::list<Model *>> newlist;
	//	newlist.first = radi;
	//	newlist.second.push_back(m);
	//	tempZ.insert(std::pair<int, std::pair<int,std::list<Model*>>>(zi,newlist));
	//	tempYZ.insert (std::pair<int, std::map<int, std::pair<int,std::list<Model*>>>>(yi,tempZ));
	//	m_xyzModelMap.insert(std::pair<int, std::map<int, std::map<int, std::pair<int,std::list<Model*>>>>>(xi, tempYZ));
	//	m_allModel.push_back(m);
	//	return true;
	//}
	//else
	//{
	//	itry fiy = fix->second.find(yi);
	//	if (fiy == fix->second.end())
	//	{
	//		std::map<int, std::pair<int,std::list<Model*>>> tempZ;
	//		std::pair<int,std::list<Model *>> newList;
	//		newList.first = radi;
	//		newList.second.push_back(m);
	//		tempZ.insert(std::pair<int, std::pair<int,std::list<Model*>>>(zi, newList));
	//		fix->second.insert(std::pair<int,std::map<int,std::pair<int,std::list<Model*>>>>(yi, tempZ));
	//		m_allModel.push_back(m);
	//		return true;
	//	}
	//	else
	//	{
	//		itrz fiz = fiy->second.find(zi);
	//		if (fiz == fiy->second.end())
	//		{
	//			std::pair<int,std::list<Model *>> newList;
	//			newList.first = radi;
	//			newList.second.push_back(m);
	//			fiy->second.insert(std::pair<int, std::pair<int,std::list<Model*>>>(zi, newList));
	//			m_allModel.push_back(m);
	//			return true;
	//		}
	//		else
	//		{
	//			std::list<Model *> &curList = fiz->second.second;
	//			std::list<Model*>::iterator ii = curList.begin();
	//			std::list<Model*>::iterator ee = curList.end();
	//			for (ii; ii != ee; ++ii)
	//			{
	//				if (*ii == m)
	//					return false;
	//			}
	//			if (fiz->second.first < radi)
	//				fiz->second.first = radi;
	//			curList.push_back(m);
	//			m_allModel.push_back(m);
	//			return true;
	//		}
	//	}
	//}
	//return false;
	const ModelID &mid = m->GetModelID();
	int mcounter = mid.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	if (m_listCells.at(xno) == 0)
	{
		m_listCells.at(xno) = new std::vector< std::list<Model *> *>(m_yCellCount , 0);
	}
	std::list<Model *> * curlist = m_listCells.at(xno)->at(yno);
	if (curlist == 0)
	{
		m_listCells.at(xno)->at(yno) = new std::list<Model *>();
		m_listCells.at(xno)->at(yno)->push_back(m);
		m_allModel.push_back(m);
		return true;
	}
	else
	{
		std::list<Model *>::iterator iii = curlist->begin();
		std::list<Model *>::iterator eee = curlist->end();
		for (iii; iii != eee; ++iii)
		{
			if (*iii == m)
				return false;
		}
		curlist->push_back(m);
		m_allModel.push_back(m);
		return true;
	}
	//return true;
}

std::list<Model *>::iterator ModelList::Begin()
{
	return m_allModel.begin();
}
std::list<Model *>::iterator ModelList::End()
{
	return m_allModel.end();
}

void ModelList::UpdateModelPosition(const ModelID &oldid, const ModelID &newid)
{
	int mcounter = oldid.GetCounterNumber();
	mcounter = mcounter % (m_xCellCount * m_yCellCount * m_cellItemCount);
	int xno = (int)(mcounter / (m_cellItemCount * m_yCellCount));
	int yno = (int)((mcounter - xno * m_yCellCount*m_cellItemCount) / (m_cellItemCount));
	std::vector<std::list<Model *> *> *vec = m_listCells.at(xno);
	if (vec == 0)
		throw VException("ModelList::GetModel exception , model not found , id missing", 1);
	std::list<Model *> *curlist = vec->at(yno);
	if (curlist == 0)
		throw VException("ModelList::GetModel exception , model not found , id missing", 1);
	std::list<Model *>::iterator iii = curlist->begin();
	std::list<Model*>::iterator eee = curlist->end();
	Model *mymodel = 0;
	std::list<Model *>::iterator searchi = curlist->end();
	for (iii; iii != eee; ++iii)
	{
		if ((*iii)->GetModelID() == oldid)
		{
			mymodel = *iii;
			searchi = iii;
			break;
		}
	}
	if (mymodel == 0)
		throw VException("ModelList::UpdateModelPosition exception , model not found , id missing", 1);
	curlist->erase(searchi);
	mymodel->SetModelID(newid);
	AddModelIf(mymodel);
}
