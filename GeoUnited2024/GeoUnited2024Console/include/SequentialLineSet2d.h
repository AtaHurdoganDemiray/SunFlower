// SequentialLineSet2d.h

#ifndef __SequentialLineSet2d_h__
#define __SequentialLineSet2d_h__

#include "PointSet2d.h"
struct VLine2d;
class SequentialLineSet;
class TransformationMatrix;

class SequentialLineSet2d
{
public:
	SequentialLineSet2d ( int initialcapacity );
	SequentialLineSet2d ( const SequentialLineSet2d & Foo );
	SequentialLineSet2d (const SequentialLineSet &sl);
	VLine2d operator[] ( int index ) const;
	const VPoint2d * GetPoint ( int index ) const;
	VPoint2d * GetPoint ( int index );
	int Size() const;
	int GetPointSize()const;
	void InsertPoint ( const VPoint2d & p );
	VPoint2d GetMin() const;
	VPoint2d GetMax() const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	PointSet2d & GetPointSet();
	const PointSet2d & GetPointSet()const;
	bool operator == (const SequentialLineSet2d &toCompare)const;
	bool operator != (const SequentialLineSet2d &toCompare)const;
	vreal GetLength()const;
	VPoint2d GetP (const vreal &t)const;
	VPoint2d GetP (const vreal &t,const vreal &setLength)const;
	const VPoint2d * Start()const;
	const VPoint2d * End()const;
	VPoint2d * Start();
	VPoint2d * End();
	void Reverse();
	void AddBack (const SequentialLineSet2d &toAdd);
	void Reserve (int newcapacity);
	void TransformToSquare (const vreal u0 , const vreal u1
							,const vreal v0 , const vreal v1);
	int GetMinRef ()const;
	void MakeTenDigits ();
protected:
	PointSet2d sequentialpoints;
public:
	void Transform (const TransformationMatrix &tm);
	void FixDiscontinuetyInXForTrimLoop01(vreal tol);
	void FixDiscontinuetyInYForTrimLoop01(vreal tol);
	int GetFirstPrevXNot01(int pi, vreal tol);
	int GetFirstNextXNot01(int pi, vreal tol);
	int GetFirstPrevYNot01(int pi, vreal tol);
	int GetFirstNextYNot01(int pi, vreal tol);
};

#endif