// LinePlaneRelation.h

#ifndef __LinePlaneRelation_h__
#define __LinePlaneRelation_h__

struct VLine;
struct VPlane;

class LinePlaneRelation
{
public:
	enum Type {INTERSECTING , COINCIDENT , POSITIVE , NEGATIVE};
	static Type Clasify (const VLine &l , const VPlane &p , VLine &subpos , VLine &subneg); 
};

#endif