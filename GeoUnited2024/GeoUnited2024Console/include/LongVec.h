/******************************************************************************
*               File: LongVec.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.09.2003 13:58:54 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/
#ifndef __LongVec_h__
#define __LongVec_h__

struct LongVec
{
public:
	LongVec();
	//LongVec(long i);
	~LongVec();
	long  operator [] ( long index ) const;
	void AddLong ( long item );
	long Size() const;
	long & At ( int index );
	void RemoveAll();
	void Sort();
	void RemoveDuplicate();
	std::vector<long> & GetVector();
	const std::vector<long> & GetVector()const;
private:
	std::vector<long> quad;
};

#endif //__LongVec_h__