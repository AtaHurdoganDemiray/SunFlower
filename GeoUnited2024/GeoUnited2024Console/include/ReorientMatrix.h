// ReorientMatrix.h

#ifndef __ReorientMatrix_h__
#define __ReorientMatrix_h__

#include "TransformationMatrix.h"
#include "CoordinateSystem.h"

class ReorientMatrix : public TransformationMatrix
{
public:
	ReorientMatrix();
	ReorientMatrix (const ReorientMatrix &toCopy);
	ReorientMatrix ( const CoordinateSystem &_base , const CoordinateSystem &_destiny );
	ReorientMatrix &operator=(const ReorientMatrix &toCopy);
	virtual ~ReorientMatrix();
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
protected:
	CoordinateSystem base;
	CoordinateSystem destiny;
};

#endif //__ReorientMatrix_h__
