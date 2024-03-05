// SpaceConverter.h

#ifndef __SpaceConverter_h__
#define __SpaceConverter_h__

struct VPoint;

class SpaceConverter
{
public:
	virtual VPoint Convert (const VPoint &p) = 0;
	virtual VPoint ConvertThrowIfOut (const VPoint &p , double tol) = 0;
	// ConvertLoop expects source and destiny sizes to be equal 
	// , this should be done by initializing destiny by copying source at the begining
	void ConvertLoop (const std::vector<VPoint> &src , std::vector<VPoint> &dest);
	bool ConvertLoopIfInside (const std::vector<VPoint> &src , std::vector<VPoint> &dest , double tol);
};

#endif