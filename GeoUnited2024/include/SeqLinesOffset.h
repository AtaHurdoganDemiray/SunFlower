// SeqLinesOffset.h

#ifndef __SeqLinesOffset_h__
#define __SeqLinesOffset_h__

class SequentialLineSet2d;
struct VLine2d;
class LineSet2d;
class SeqLinesOffset
{
public:
	static void Offset (const SequentialLineSet2d &sl , SequentialLineSet2d &res,const vreal offsval , const vreal tol);
	static void TwoSeqLinesOff (const vreal offsval,const vreal tol,VLine2d &currl,VLine2d &nextl,const vreal sgnm,const SequentialLineSet2d &sl,const int i,LineSet2d &ls2);
};

#endif