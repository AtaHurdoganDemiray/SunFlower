/******************************************************************************
*               File: DoubleVec.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  06.09.2003 13:58:54 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/
#ifndef __DoubleVec_h__
#define __DoubleVec_h__

struct DoubleVec
{
public:
	DoubleVec();
	//DoubleVec(long i);
	~DoubleVec();
	const vreal & operator [] ( long index ) const;
	void Addvreal ( const vreal &item );
	bool Addvrealif (const vreal item);
	long Size() const;
	void RemoveAll();
	void Sort(){ std::sort( mydata.begin() , mydata.end() );}
	void SortAndRemoveDuplicate();
	bool IsValueARepeat ( vreal value , long index ) const;
	bool IsValueARepeat ( const vreal &value , const vreal &tolerance ) const;
	void Reserve (const int mysize);
	void Reverse ();
private:
	std::vector<vreal> mydata;
};

#endif //__DoubleVec_h__
