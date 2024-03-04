// VTriangle2d.h

#ifndef __VTriangle2d_h__
#define __VTriangle2d_h__
#include "VPoint2d.h"

struct VTriangle2d
{
public:
	friend struct VTriangle2dE;
	VTriangle2d();
	VTriangle2d (const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3);
	VTriangle2d (const vreal cor1[3] , const vreal cor2[3] , const vreal cor3[3] );
	VTriangle2d (const VTriangle2d &toCopy);
	VTriangle2d & operator = (const VTriangle2d &toCopy);
	const VPoint2d & GetCorner1() const;
	const VPoint2d & GetCorner2() const;
	const VPoint2d & GetCorner3() const;
	VPoint2d & GetCorner1();
	VPoint2d & GetCorner2();
	VPoint2d & GetCorner3();	
	bool operator == (const VTriangle2d &toCompare)const;
	bool operator != (const VTriangle2d &toCompare)const;
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
	sVector<double,2> GetMin()const;
	sVector<double,2> GetMax()const;
private:
	VPoint2d corner1;
	VPoint2d corner2;
	VPoint2d corner3;
};

#endif