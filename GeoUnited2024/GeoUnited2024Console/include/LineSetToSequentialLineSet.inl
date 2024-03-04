// LineSetToSequentialLineSet.inl

#ifndef __LineSetToSequentialLineSet_inl__
#define __LineSetToSequentialLineSet_inl__

template <class Line , class Set , class Pos , class Seq>
	std::vector<Seq *> 
						LineSetToSequentialLineSet<Line , Set , Pos , Seq>
::Make (const Set &lineset)
{	
	int mysize = lineset.Size();
	Set tmp (mysize);
	for (int i=0;i<mysize;++i)
	{
		const Line *currl = lineset[i];
		if (currl->GetV().GetLength() < EPSILON)
			continue;
		tmp.AddLine (*currl);
	}

	return NewImp (tmp);
	
	//Set tmp(lineset);
	//std::vector<Seq*>result;
	//bool goon =true;
	//while(goon)
	//{
	//	goon = LoopIf (tmp , result);
	//};
	//return result;
};

template <class Line, class Set , class Pos , class Seq>
bool 
						LineSetToSequentialLineSet<Line , Set , Pos , Seq>
::LoopIf (Set &lineset 
		 , std::vector<Seq *> &result)
{
	int setSize = lineset.Size();
	if (setSize > 0)
	{
		std::vector<bool> isUsed(setSize,false);
		Seq *currSeq = new Seq (setSize);
		Loop (lineset , *currSeq ,isUsed);
		result.push_back (currSeq);
		
		Set tmp(setSize);
		for (int j=0;j<setSize;j++)
		{
			bool currCk = isUsed[j];
			if(currCk == false)
			{
				Line *cl = lineset[j];
				tmp.AddLine (*cl);
			}
		}
		if (tmp.Size() > 0)
		{
			lineset = tmp;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
};

template <class Line , class Set , class Pos , class Seq>
	void 
					LineSetToSequentialLineSet<Line , Set , Pos , Seq>
::Loop (Set &lineset , Seq  &currSeq , std::vector<bool> &isUsed)
{
	int setSize = lineset.Size();
	Pos poscubes (lineset);
	const Line *firstLine = lineset[0];
	currSeq.InsertPoint(firstLine->GetP1());
	currSeq.InsertPoint (firstLine->GetP2());
	isUsed[0] = true;
	int currI = 0;
	bool goon = true;

	while (goon)
	{
		Line *cl = lineset[currI];
		bool neighbourFound = false;
		std::vector <int> iVec1;
		GetCoincidentPointedLines (currI , iVec1 ,lineset , isUsed , poscubes);
		int mysize = iVec1.size();
		
		if (mysize == 1)
		{
				int ind = iVec1[0];
				if (ind>0)
				{
					Line *sl = lineset[ind];
					isUsed[ind] = true;
					currSeq.InsertPoint (sl->GetP2());			
					currI = ind;
					neighbourFound = true;
					if (sl->GetP2().Compare (firstLine->GetP1() , 1.e-6) )
						return;//loop closed
				}
				else 
				{
					ind *= -1;
					Line *sl = lineset[ind];
					isUsed[ind] = true;
					currSeq.InsertPoint (sl->GetP1());		
					Line tmpl (sl->GetP2() , sl->GetP1());
					*sl = tmpl;
					currI = ind;
					neighbourFound = true;
					if (sl->GetP1().Compare (firstLine->GetP1() , 1.e-6) )
						return;//loop closed
				}
		}
		else
		{
			// this is a node point 
			break;
		}

		if (neighbourFound == false)
			goon = false;

	}// while goon			
};

template <class Line , class Set , class Pos , class Seq>
	void LineSetToSequentialLineSet<Line , Set , Pos , Seq>
		::GetCoincidentPointedLines 
									(int currI 
									, std::vector<int> &iVec1 
									, Set &lineset 
									, std::vector<bool> &isUsed
									, Pos &poscubes)
{
	Line *cl = lineset[currI];
	LongVec tmp,nii;
	poscubes.SetRelatedIndiceListForLine (*cl , tmp , nii);
	int size2 = tmp.Size();
	bool neighbourFound = false;

	for (int j=0;j<size2;j++)
	{
		int ind = tmp[j];
		if (ind == currI || isUsed[ind] == true)
			continue;

		Line *sl = lineset[ind];
		if ( cl->GetP2().Compare (sl->GetP1() , 1.e-6) )
		{
			iVec1.push_back (ind);
		}
		else if ( cl->GetP2().Compare (sl->GetP2() , 1.e-6) )
		{
			iVec1.push_back (-ind);
		}
	}
};

const vreal COI_TOL = 1.e-6;

#define DIRECTED_LINES 1

template <class Line , class Set , class Pos , class Seq>
std::vector<Seq *> LineSetToSequentialLineSet<Line,Set,Pos,Seq>::NewImp (const Set &ls)
{
	if (ls.Size() == 0)
	{
		std::vector<Seq *> emptyset;
		return emptyset;
	}
	std::vector<MyLine> myvec (ls.Size());
	Pos posc (ls);
	
	int mysize = ls.Size();
	for (int j=0;j<mysize;++j)
	{
		const Line * l=ls[j];
		myvec[j].m_ref = j;
		
		if (myvec[j].m_n1 == -1)
		{
			LongVec relvec,nii;
			posc.SetRelatedIndiceListForPoint (l->GetP1(),relvec,nii);
			int mysize2 = relvec.Size();
			for (int w = 0;w<mysize2;++w)
			{
				int curri = relvec[w];
				if (curri == j)
					continue;
				const Line *n = ls[curri];
				
#ifndef DIRECTED_LINES
				if (myvec[curri].m_n1 == -1)
				{
					if (l->GetP1().Compare (n->GetP1(),COI_TOL))
					{
						myvec[j].m_n1 = curri;
						myvec[j].m_n1vno = 1;
						myvec[curri].m_n1 = j;
						myvec[curri].m_n1vno = 1;
						break;
					}
				}
#endif					
				if (myvec[curri].m_n2 == -1)
				{
					if (l->GetP1().Compare (n->GetP2() , COI_TOL))
					{
						myvec[j].m_n1 = curri;
						myvec[j].m_n1vno = 2;
						myvec[curri].m_n2 = j;
						myvec[curri].m_n2vno = 1;
						break;
					}
				}
			}
		}

		if (myvec[j].m_n2 == -1	)
		{
			LongVec relvec,nii;
			posc.SetRelatedIndiceListForPoint (l->GetP2(),relvec,nii);
			int mysize2 = relvec.Size();
			for (int i=0;i<mysize2;++i)
			{
				int curri = relvec[i];
				if (curri == j)
					continue;
				const Line * n = ls[curri];
				if (myvec[curri].m_n1 == -1)
				{
					if (l->GetP2().Compare (n->GetP1(),COI_TOL))
					{
						myvec[j].m_n2 = curri;
						myvec[j].m_n2vno = 1;
						myvec[curri].m_n1 = j;
						myvec[curri].m_n1vno = 2;	
						break;
					}
				}
#ifndef DIRECTED_LINES				
				if (myvec[curri].m_n2 == -1)
				{
					if (l->GetP2().Compare (n->GetP2(),COI_TOL))
					{
						myvec[j].m_n2 = curri;
						myvec[j].m_n2vno = 2;
						myvec[curri].m_n2 = j;
						myvec[curri].m_n2vno = 2;
						break;
					}
				}
#endif
			}
		}
	}
	std::vector<Seq *> res;
	int starti = GetAStartIndex (myvec);
	while (starti != -1)
	{
		Seq *aloop = MakeOneLoop (ls,myvec,starti);
		res.push_back (aloop);
		starti = GetAStartIndex (myvec);
	}

	return res;
};

template <class Line , class Set , class Pos , class Seq>
Seq * LineSetToSequentialLineSet<Line,Set,Pos,Seq>::MakeOneLoop
							(const Set &lineset
							, std::vector<MyLine> &lines
							, const int starti)
{
	Seq tmp (60);
	MyLine &l = lines[starti];
	l.m_used = true;
	const Line * firstline = lineset[l.m_ref];
	tmp.InsertPoint (firstline->GetP1());
	tmp.InsertPoint (firstline->GetP2());
	
	int previ = starti;
	do
	{
		int curri = lines[previ].m_n2;
		if (curri == -1)
			break;
		
		MyLine &nextline = lines[curri];
		if (nextline.m_used)
			break;
		nextline.m_used = true;
#ifndef DIRECTED_LINES
		if (lines[previ].m_n2vno == 1)
#endif			
			tmp.InsertPoint (lineset[nextline.m_ref]->GetP2());
#ifndef DIRECTED_LINES
		else
			tmp.InsertPoint (lineset[nextline.m_ref]->GetP1());
#endif
		previ = curri;
	}
	while (!tmp.Start()->Compare (*tmp.End(),COI_TOL));
	Seq *res = new Seq (tmp);
	return res;
};

template <class Line , class Set , class Pos , class Seq>
const int LineSetToSequentialLineSet<Line,Set,Pos,Seq>::GetAStartIndex(const std::vector<MyLine> &lines)
{
	int mysize = (int)lines.size();
	for (int i=0;i<mysize;++i)
	{
		const MyLine &l = lines[i];
		if (l.m_used == false)
			return i;
	}
	return -1;
}

#endif
