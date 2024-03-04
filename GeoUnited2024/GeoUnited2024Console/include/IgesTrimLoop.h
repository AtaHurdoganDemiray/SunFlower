// IgesTrimLoop.h

#ifndef __IgesTrimLoop_h__
#define __IgesTrimLoop_h__

class dCurve;

class IgesTrimLoop
{
public:
	std::vector <IgesEntity *> loopcurves;
	std::vector<bool> m_isReversed;
	std::vector<dCurve *> m_dcurves;
};

#endif