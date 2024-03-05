// Points.h

#ifndef __Points_h__
#define __Points_h__
struct VPoint;
//interface class for points vector
class Points
{
public:
	Points ();
	Points (const Points &toCopy);
	Points & operator = (const Points &toCopy);
	virtual ~Points();
	virtual const VPoint & operator [] (int i)const = 0;
	virtual int Size()const =0;
	virtual const VPoint GetMin()const =0;
	virtual const VPoint GetMax()const =0;
	unsigned int GetPointer()const;
};

#endif