// ModelList.h: interface for the ModelList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELLIST_H__D6564F10_91EE_42EE_A7D3_D22C73F2BB3F__INCLUDED_)
#define AFX_MODELLIST_H__D6564F10_91EE_42EE_A7D3_D22C73F2BB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Model.h"
#include "list"
using namespace std;
//#include "SelectableTypes.h"
#include "CoordinateSystem.h"

class HandleSet;
class ModelSelectionFilters;
class SelectionFlags;
struct ModelID;

class ModelList //: public list<Model*>
{
public:
	friend class Project;
	typedef std::list<Model*>::const_iterator pos;
	ModelList();
	virtual ~ModelList();

	//list<Model*>::const_iterator position;

	//bool iterating;

	//Model * BeginIterating ();
	//Model* Iterate();
	//Model* IterateBackwards();

	//void AddModel(Model* p_model);
	//bool GetMinMaxOfAllVisibleModels ( VPoint & min , VPoint & max );
	//Model * GetModelByHandle ( unsigned int handle );
	//const SelectableTypes & GetAllTypes ();
	//void DeleteAll();
	//void SetCurrCoordSystem ( const CoordinateSystem * cs );
	//void ReverseVisibleOfAllModels ();
	Model * GetModel (const ModelID &id)const;
	Model * GetModelNoThrow(const ModelID &id)const;
	//bool SearchForModelID (const ModelID &id,ModelID &fid);
private:	
	//SelectableTypes alltypes;
	//void DeleteModel (unsigned int handle);
	//std::multimap < int,std::multimap< int,std::multimap< int,std::multimap<int,ModelID> > > > m_searcMaps;
	
	/*
	std::map<int, std::map<int, std::map<int, std::pair<int , std::list<Model *>>>>> m_xyzModelMap;
	typedef std::map<int, std::map<int, std::map<int, std::pair<int,std::list<Model *>>>>>::iterator itrx;
	typedef std::map<int, std::map<int, std::pair<int,std::list<Model *>>>>::iterator itry;
	typedef std::map<int, std::pair<int,std::list<Model *>>>::iterator itrz;
	typedef std::map<int, std::map<int, std::map<int, std::pair<int,std::list<Model *>>>>>::const_iterator citrx;
	typedef std::map<int, std::map<int, std::pair<int,std::list<Model *>>>>::const_iterator citry;
	typedef std::map<int, std::pair<int,std::list<Model *>>>::const_iterator citrz;
	*/
	
	std::vector<std::vector<std::list<Model *> *> *> m_listCells;
	static int m_xCellCount;
	static int m_yCellCount;
	static int m_cellItemCount;
	std::list<Model*> m_allModel;

public:

	// returns false if model already exist in list and not added again
	bool AddModelIf(Model *m);
	//static vreal m_invXYZRes;
	std::list<Model *>::iterator Begin();
	std::list<Model *>::iterator End();
	void UpdateModelPosition(const ModelID &oldid , const ModelID &newid);
};

#endif // !defined(AFX_MODELLIST_H__D6564F10_91EE_42EE_A7D3_D22C73F2BB3F__INCLUDED_)
