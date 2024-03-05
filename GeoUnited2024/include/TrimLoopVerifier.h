// TrimLoopVerifier.h

#ifndef __TrimLoopVerifier_h__
#define __TrimLoopVerifier_h__

class SequentialLineSet2d;

class TrimLoopVerifier
{
public:
	static bool IsClosed (const SequentialLineSet2d &l,const vreal tol);
	static bool IsCCW (const SequentialLineSet2d &l);
	static bool IsInUnitSquare (const SequentialLineSet2d &l,const vreal tol);
	static void RemoveDuplicatePoints (const SequentialLineSet2d &sour
										,SequentialLineSet2d &dest
										,const vreal tol);
private:

};

#endif
