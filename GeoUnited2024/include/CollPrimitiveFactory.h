// CollPrimitiveFactory.h

#ifndef __CollPrimitiveFactory_h__
#define __CollPrimitiveFactory_h__
#include "ToolPrim.h"
#include "mwTolerance.hpp"
#include "mwOffsetType.hpp"
#include "vTool.h"

namespace cc
{
	class mwCollPrimitiveGeo;
};

class CollPrimitiveFactory:public ToolPrimVisitor
{
public:
	CollPrimitiveFactory();
	void Make (const vTool<float> &src,std::vector<cc::mwCollPrimitiveGeo *> &primVec);
	virtual void Visit (const ToolPrim<float> &visitor);
	virtual void Visit (const ToolLine<float> &visitor);
	virtual void Visit (const ToolVerticalLine<float> &visitor);
	virtual void Visit (const ToolCircle<float> &visitor);
private:
	std::vector<cc::mwCollPrimitiveGeo *> *m_products;
	cc::mwTolerance m_tol;
	cc::offsettype m_offtype;
};

#endif