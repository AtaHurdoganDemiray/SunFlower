// ModelID.h

#ifndef __ModelID_h__
#define __ModelID_h__

#include "Bofstream.h"
#include "Bifstream.h"
#include "VPoint.h"

struct ModelID
{
public:
	ModelID();
	friend class EntityInfo;
	friend class HandleSet;
	friend class ViewPointItem;
	friend class Entity;
	friend class CommandToViewPoint;
	friend class Project;
	friend class AllGeoSer;
	friend class LuaScriptSolidCncBridge;
	friend class VTextureMapEnt;
	friend class HMIComManager;
	friend class CopyList;
	friend class VpAddTypeToVp;
	friend class VCurve;
	friend class VShapeAdder;
	friend class VSurface;
	friend class SinglePoint;
	const bool operator == (const ModelID &id)const;
	const bool operator != (const ModelID &id)const;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	//void * GetModelPtr ()const;
	//void SetModelPtr (void * model);
	//const VTime & GetBirthDate ()const;
	//void SetBirthDate (const VTime &bd);
	//LONGLONG GetBirthPlace ()const;
	//void SetBirthPlace (LONGLONG bp);
	//void SetBirthPlace2 (const int bp2);
	bool operator < (const ModelID &second)const;
	bool operator > (const ModelID &second)const;
	void SetAsNull ();
	bool IsNull()const;
	const VStr & GetIdString()const;
	int GetCounterNumber ()const;
private:
	//VTime m_birthdate;
	//LONGLONG m_birthplace;
	//mutable void * m_model;
	VStr m_id;
	static int m_modelIdCounter;
	void SetIdString (const VStr &idStr);
	//VPoint m_center;
	unsigned int m_runTimeSeperator;
public:
	//const VPoint & GetCenter()const;
	//void SetCenter(const VPoint &center);
	//void ResetCenterAndRadius(const VPoint &cen, vreal rad);
	//void GetCenterAndRadius(VPoint &cen, vreal &rad)const;
	//void AddCenterAndRadiusIf();
	unsigned int GetRuntimeSeperator()const;
};

#endif