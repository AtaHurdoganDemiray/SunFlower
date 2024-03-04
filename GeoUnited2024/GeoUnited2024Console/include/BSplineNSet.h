// BSplineNSet.h

#ifndef __BSplineNSet_h__
#define __BSplineNSet_h__

//#include <vector>

class BSplineN;

class BSplineNSet
{
public:
	const BSplineN & operator [] ( int index ) const;
	int Size() const;
	void AddItem ( const BSplineN & newitem );

protected:

	std::vector <BSplineN> set;

};

#endif