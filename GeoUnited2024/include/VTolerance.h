// VTolerance.h

#ifndef __VTolerance_h__
#define __VTolerance_h__

struct VTolerance
{
public:
	VTolerance ( const vreal & _tolerance , bool _ismetric )
	{
		tolerance = _tolerance;
		ismetric = _ismetric;
	};

	vreal tolerance;
	bool ismetric;

};

#endif
