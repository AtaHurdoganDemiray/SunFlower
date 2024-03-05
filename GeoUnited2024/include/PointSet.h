/******************************************************************************
*               File: PointSet.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __PointSet_h__
#define __PointSet_h__

#include "VPoint.h"
#include "PrimitiveGeoVec.h"

struct DoubleList;
class TransformationMatrix;

class PointSet:public PrimitiveGeoVec<VPoint,3>
{
public:
	PointSet();
	PointSet( const long &initialcapacity );
	PointSet ( const long &initialcapacity , DoubleList & mylist );
	PointSet ( const PointSet & Foo );
	PointSet & operator = (const PointSet &toCopy);
	PointSet (const std::vector<VPoint> &pvec);
	virtual ~PointSet();
	//bool operator == (const PointSet &toCompare)const;
	//bool operator != (const PointSet &toCompare)const;	
	//const VPoint & GetMin() const;
	//const VPoint & GetMax() const;
	//void UpdateMinMax() ;
	//long Size() const;
	void InsertPoint ( const VPoint & newpoint );	
	void InsertPoint ( const vreal *coord );
	VPoint * operator [] ( const long &index );
	const VPoint * operator [] (const long &index) const;
	//void UpdateMinMaxForSinglePoint ( const VPoint & pnt );
	//void Clear();	
	bool GetIndex ( const VPoint & p , int & index , const vreal tol = EPSILON) const;
	void Transform ( const TransformationMatrix & tm );
	void Serialize (Bofstream &binfile)const;
	void Serialize (Bifstream &binfile);
	void TransformAsVectors (const TransformationMatrix &tm);
	void RemoveDuplicate (std::vector<int> &removed);
private:
	//std::vector<VPoint>points;
	//VPoint m_min;
	//VPoint m_max;
public:
	void AddVecBack(PointSet *ps);
	void TransformAsUnitVectors(const TransformationMatrix &tm);
};




#endif //__PointSet_h__