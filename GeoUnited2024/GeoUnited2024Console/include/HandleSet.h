/******************************************************************************
*               File: HandleSet.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  14.09.2003 09:23:43 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __HandleSet_h__
#define __HandleSet_h__

//#include <vector>
#include "ModelID.h"
using namespace std;
class VTransformable;

class HandleSet
{
public:
	HandleSet();
	HandleSet (const std::vector<VTransformable *> &items);
	~HandleSet();
	//unsigned int operator [] ( int index );
	//const unsigned int operator [] (int index)const;
	ModelID & GetID (int index);
	const ModelID & GetID (int index)const;
	const long Size()const;
	// returns false if newid already exist , else true
	bool AddItem (const ModelID &newid);
	void AddItemBrudeForceChecked(const ModelID &newid);
	void Clear();
	void AddBack (const HandleSet &toAdd);
	void AddBackBrudeForceChecked(const HandleSet &toAdd);
	void RemoveItem (const ModelID &toremove);
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
	bool IsExist (const ModelID &id)const;
	const ModelID & GetLastID ()const;
	ModelID & GetLastID();
	static void EquilizeModelIDsWithHS (std::vector<ModelID *> &destiny , HandleSet &hs);
	void RemoveAll (const HandleSet &hs);
	bool RemoveIf (const ModelID &id);
	void ReverseOrder();
private:
	vector <ModelID> mydata;

	std::vector<std::vector<std::list<std::pair<ModelID,int>> *> *> m_listCells;
	static int m_xCellCount;
	static int m_yCellCount;
	static int m_cellItemCount;
	bool AddToCellsIf(const ModelID &id,int ind);
public:
	int FindID(const ModelID &id)const; // returns -1 if not found , otherwise index to mydata vector
};



#endif //__HandleSet_h__
