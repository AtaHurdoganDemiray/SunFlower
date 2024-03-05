// VUnit.h

#ifndef __VUnit_h__
#define __VUnit_h__

struct VUnit
{
public:
    enum vunit{mm,inch,unknown};
	VUnit();
    VUnit (VUnit::vunit u);
    VUnit::vunit theunit;
};

#endif
