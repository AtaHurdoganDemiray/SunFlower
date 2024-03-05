// OglMatrix.h

#ifndef __OglMatrix_h__
#define __OglMatrix_h__

class TransformationMatrix;

struct OglMatrix
{
public:
	OglMatrix();
	OglMatrix (const TransformationMatrix &tm);
	OglMatrix (const OglMatrix &toCopy);
	OglMatrix & operator=(const OglMatrix &toCopy);
	~OglMatrix();
	const vreal * GetArray()const;
private:
	vreal *xmat;
};

#endif
