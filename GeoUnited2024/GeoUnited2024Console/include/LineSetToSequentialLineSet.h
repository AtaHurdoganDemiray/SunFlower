// LineSetToSequentialLineSet.h

#ifndef __LineSetToSequentialLineSet_h__
#define __LineSetToSequentialLineSet_h__

template <class Line , class Set , class Pos , class Seq> class LineSetToSequentialLineSet
{
public:
	std::vector<Seq *> Make (const Set &lineset);	
private:
	void Loop (Set &lineset , Seq &currSeq , std::vector<bool> &isUsed);
	bool LoopIf (Set &lineset , std::vector<Seq *> &result);
	void GetCoincidentPointedLines 	(int currI 
									, std::vector<int> &iVec1 
									, Set &lineset 
									, std::vector<bool> &isUsed
									, Pos &poscubes);
	std::vector<Seq* > NewImp (const Set &ls);
	struct MyLine
	{
	public:
		MyLine ()
		:m_ref(-1),m_n1(-1),m_n2(-1),m_n1vno(-1),m_n2vno(-1),m_used(false)
		{
		};
		int m_ref;
		int m_n1 ;
		int m_n2 ;
		short m_n1vno;
		short m_n2vno;
		bool m_used;
	};
	Seq * MakeOneLoop (const Set &lineset,std::vector<MyLine> &lines,const int starti);
	const int GetAStartIndex (const std::vector<MyLine> &lines);
};

#include "LineSetToSequentialLineSet.inl"

#endif
