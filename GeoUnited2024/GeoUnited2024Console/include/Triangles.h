// Triangles.h

#ifndef __Triangles_h__
#define __Triangles_h__

struct VTriangleE;
struct VPoint;
struct VTriangle;

// interface class for triangles vector
class Triangles
{
public:
	Triangles ();
	//Triangles (const Triangles &toCopy);
	//Triangles & operator = (const Triangles &toCopy);
	virtual ~Triangles ();
	virtual const VTriangleE operator[] (int i)const = 0;
	virtual VTriangleE operator[] (int i) = 0;	
	virtual const VTriangle GetTri (int i)const = 0;
	virtual int Size()const = 0;
	virtual const VPoint GetMin()const =0;
	virtual const VPoint GetMax()const =0;
	unsigned int GetPointer()const;
};

#endif