// IgesTrimLoop2d.h

#ifndef __IgesTrimLoop2d_h__
#define __IgesTrimLoop2d_h__

//#include <vector>

class IgesEntity;
struct VPoint;
class IgesEntitySet;

class IgesTrimLoop2d
{
public:
	std::vector <IgesEntity*> loopcurves;
	std::vector<bool> m_isReversed;
	void Order(IgesEntitySet *entSet);
};


#endif

