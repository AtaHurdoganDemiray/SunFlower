// ModelOrientation.h

#ifndef __ModelOrientation_h__
#define __ModelOrientation_h__

class Model;

class ModelOrientation
{
public:
	void OrientModel ( Model * pmodel ) const;
	void OriginalOrientation () const;

};

#endif