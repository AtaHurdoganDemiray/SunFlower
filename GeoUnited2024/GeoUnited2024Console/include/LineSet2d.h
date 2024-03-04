/******************************************************************************
*               File: LineSet2d.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __LineSet2d_h__
#define __LineSet2d_h__

#include "VLine2d.h"
#include "PrimitiveGeoVec.h"

struct DoubleList;
class ModelSelectionFilters;
class SequentialLineSet2d;
struct VLine2dE;

class LineSet2d : public PrimitiveGeoVec<VLine2d , 2>
{
public:
	LineSet2d();
	LineSet2d ( int initialcapacity );
	LineSet2d ( DoubleList &mylist );
	LineSet2d ( const LineSet2d & Foo );
	LineSet2d ( const std::vector<VLine2d> &v);
	LineSet2d ( const SequentialLineSet2d &v);
	LineSet2d ( const std::vector <VLine2dE> &v);
	LineSet2d & operator = (const LineSet2d &toCopy);
	virtual ~LineSet2d();
	//void UpdateMinMax();
	//void UpdateMinMaxForLine ( const VLine2d & newline );
	//long Size() const;
	void AddLine ( const VLine2d &newline );	
	void AddLine ( const vreal *p1 , const vreal *p2 );
	void AddLine ( const VPoint2d &p1 , const VPoint2d &p2 );
	//VPoint2d GetMin () const;
	//VPoint2d GetMax () const;
	//void Clear();
	VLine2d * operator[] ( const long &index );
	const VLine2d * operator[] ( const long &index ) const;
	//void Serialize (Bofstream &outfile)const;
	//void Serialize (Bifstream &infile);
	//bool operator == (const LineSet2d &toCompare)const;
	//bool operator != (const LineSet2d &toCompare)const;	
	void AddBack (const SequentialLineSet2d &sl);
protected:
private:
	//std::vector<VLine2d>lines;
	//VPoint2d m_min;
	//VPoint2d m_max;
};

#endif //__LineSet2d_h__
