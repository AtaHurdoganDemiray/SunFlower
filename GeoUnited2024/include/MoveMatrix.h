// MoveMatrix.h

#ifndef __MoveMatrix_h__
#define __MoveMatrix_h__
#include "VPoint.h"
#include "TransformationMatrix.h"


struct VPoint;

class MoveMatrix : public TransformationMatrix
{
public:
	MoveMatrix();
	MoveMatrix ( const VPoint & _movevector );
	MoveMatrix (const MoveMatrix &toCopy);

    MoveMatrix &MoveMatrix::operator=(const MoveMatrix & Foo);
    virtual ~MoveMatrix();
	virtual void Serialize (Bofstream &outfile) const;
	virtual void Serialize (Bifstream &infile);
	virtual VStr GetInformationText()const;
protected:
	VPoint movevector;
};

#endif //__MoveMatrix_h__
