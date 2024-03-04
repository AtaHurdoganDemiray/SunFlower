/******************************************************************************
*               File: TriangleMesh.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 16:16:52 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#ifndef __TriangleMesh_h__
#define __TriangleMesh_h__

#include "VTriangleE.h"
//#include "PointSet.h"
#include "PrimitiveGeoVec.h"

struct DoubleList;
//class ModelSelectionFilters;
struct VLine;
//struct texture;
//class TriangleMesh2d;
class TransformationMatrix;
struct VPlane;

class TriangleMesh : public PrimitiveGeoVec<VTriangleE , 3> 
{
public:	
	TriangleMesh ();
	TriangleMesh ( const long &initialcapacity );
	TriangleMesh ( const long &initialcapacity , std::list<float> &mylist);//DoubleList & mylist );	
	TriangleMesh ( long initialcapacity , std::list<float> &mylist , const VPlane &clipPlane);
	TriangleMesh ( const TriangleMesh & Foo );
	TriangleMesh & operator = (const TriangleMesh &toCopy);
	virtual ~TriangleMesh ();
	bool AddTriangle 
		( const VPoint & corner1 , const VPoint & corner2 , const VPoint &corner3 );
	bool AddTriangle ( const VTriangleE &newtri );
	void UpdateMinMax();
	//void UpdateMinMaxForSingleTri ( const VTriangleE & newtri );
	//long Size() const;
	//const VPoint & GetMin() const;
	//const VPoint & GetMax() const;
	void Draw();
	//void Clear();
	VTriangleE * operator[] (const long &index);
	const VTriangleE * operator[] (const long &index) const;
	void Transform ( const TransformationMatrix & tm );
	vreal GetArea () const;
	void Serialize(Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	//bool operator == (const TriangleMesh &toCompare)const;
	//bool operator != (const TriangleMesh &toCompare)const;

protected:
private:
	//std::vector<VTriangleE>tri;
	//VPoint m_min;
	//VPoint m_max;
	bool GetPointAtPos (vreal mypoint[] , std::list<float>::iterator &mypos,std::list<float> &mylist);
};

#endif //__TriangleMesh_h__
