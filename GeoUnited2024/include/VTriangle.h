// VTriangle.h

#ifndef __VTriangle_h__
#define __VTriangle_h__
#include "VPoint.h"
class TransformationMatrix;

struct VTriangle
{
public:
	friend struct VTriangleE;
	VTriangle ();
	VTriangle (const VPoint &c1
				,const VPoint &c2
				,const VPoint &c3);
	VTriangle (const VTriangle &toCopy);
	VTriangle & operator = (const VTriangle &toCopy);
	inline const VPoint &GetCorner1()const
	{
		return corner[0];
	};
	inline const VPoint &GetCorner2()const
	{
		return corner[1];
	};
	inline const VPoint &GetCorner3()const
	{
		return corner[2];
	};
	sVector<double,3> GetMin()const;
	sVector<double,3> GetMax()const;
	void Transform (const TransformationMatrix &M);
private:
	//VPoint corner1;
	//VPoint corner2;
	//VPoint corner3;
	VPoint corner[3];
public:
	const VPoint & corner1()const;
	const VPoint & corner2()const;
	const VPoint & corner3()const;
	VPoint & corner1();
	VPoint & corner2();
	VPoint & corner3();
	// i is 0 based corner index  0 , 1 or 2 
	const VPoint & GetCorner(int i)const;
};

#endif