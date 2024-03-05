// CurvePointGeneratorFactory.h

#ifndef __CurvePointGeneratorFactory_h__
#define __CurvePointGeneratorFactory_h__
class IgesEntity;
class IgesEntitySet;
class CurvePointGenerator;
class Edge;

class CurvePointGeneratorFactory
{
public:
	static CurvePointGenerator * MakeCurve ( IgesEntity *pcurve ,  IgesEntitySet &allent);
	static CurvePointGenerator * MakeCurve ( Edge *pcurve ,  IgesEntitySet &allent , int ei , double tol);
private:
	static CurvePointGenerator * MakeConicGenerator ( IgesEntity *pcurve, IgesEntitySet &allent);

};

#endif
