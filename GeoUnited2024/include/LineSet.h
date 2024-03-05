/******************************************************************************
*               File: LineSet.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __LineSet_h__
#define __LineSet_h__

#include "PrimitiveGeoVec.h"
#include "VLine.h"
struct DoubleList;
class TransformationMatrix;
class SequentialLineSet;
class LineSet2d;

class LineSet:public PrimitiveGeoVec<VLine,3>
{
public:
	LineSet();
	LineSet ( int initialcapacity );
	LineSet ( DoubleList &mylist );
	LineSet (const std::vector<VLine> &std_vec);
	LineSet ( const LineSet & Foo );
	LineSet ( const SequentialLineSet &v);
	LineSet (const LineSet2d &ls , const vreal z);
	LineSet & operator = (const LineSet &toCopy);
	LineSet & operator = (const std::vector<VLine> &std_vec);
	virtual ~LineSet();
	//void UpdateMinMax();
	//void UpdateMinMaxForLine ( const VLine & newline );
	//long Size() const;
	void AddLine ( const VLine & newline );
	void AddLine ( const vreal *p1 , const vreal *p2 );
	void AddLine ( const VPoint &p1 , const VPoint &p2 );
	//const VPoint& GetMin () const;
	//const VPoint& GetMax () const;
	//void Clear();
	VLine * operator[] ( const long &index );
	const VLine * operator [] (const long &index )const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	void Transform (const TransformationMatrix &tm);
	//bool operator == (const LineSet &toCompare)const;
	//bool operator != (const LineSet &toCompare)const;
	void ReverseOrder () ; // the last line will be the first and first will be last , for 4ax wire toolpath
protected:
private:
	//std::vector<VLine>lines;
	//VPoint m_min;
	//VPoint m_max;
public:
	VPoint GetMinPointInCs (const CoordinateSystem &cs)const;
	VPoint GetMaxPointInCs (const CoordinateSystem &cs)const;
	void XFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &xCoordYCoordPointMap, vreal tol);
	void YFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &yCoordXCoordPointMap, vreal tol);
};

#endif //__LineSet_h__
