// TransformationMatrixSet.h

#ifndef __TransformationMatrixSet_h__
#define __TransformationMatrixSet_h__
#include "Serialized.h"

class TransformationMatrix;

class TransformationMatrixSet:public Serialized
{
public:
	TransformationMatrixSet ();
	~TransformationMatrixSet();
	TransformationMatrixSet ( int initialcapacity );
	long Size() const;
	TransformationMatrix * operator[] ( int index );
	const TransformationMatrix * operator[] ( int index ) const;
	void AddMatrix ( const TransformationMatrix & mt , unsigned int transformation_handle , bool isactive = true);
	void operator >> ( VofStream & outfile ) const;
	void ActivateMatrix (int matrixno);
	void DeactivateMatrix (int matrixno);
	TransformationMatrix GetOverallMatrix () const;
	bool IsActive ( int matrixno );
	unsigned int GetHandle( int matrixno );
	int GetTransformNoByHandle ( int handle );
	void Serialize (Bofstream &outfile )const;
	void Serialize (Bifstream &infile);
	void SerializeOldVersion(Bofstream &outfile )const;
	void SerializeOldVersion(Bifstream &infile);
private: 
	std::vector<TransformationMatrix> mtrx;
	std::vector <bool> ismatrixActive;
	std::vector<unsigned int>allhandles;
};

#endif