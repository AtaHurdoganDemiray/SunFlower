// QueryVisible.h

#ifndef __QueryVisible_h__
#define __QueryVisible_h__
#include "VPoint.h"

class OGL_Frame;
class VRefTriangleMesh;
class Display;
struct VPoint;
class PointSet;

class QueryVisible
{
public:
	QueryVisible (OGL_Frame &oglFrame , const vreal &zoffset);
	void Query (const VRefTriangleMesh &body
				, const Display *myself
				, const std::list<Display *> &others
				, std::list <int> &visible
				, std::list <int> &invisible );
	void Query (const PointSet &body
				, const Display *myself
				, const std::list<Display *> &others
				//, std::list<bool> &isHidden
				, std::list <int> &visible
				, std::list <int> &invisible );

private:
	OGL_Frame & m_oglFrame;

	void SetViewVolumeAround (const VPoint &p);
	void DrawPointInSelectionMode (const VPoint &p , vreal &z);
	void DrawOthersInSelectionMode (const std::list<Display *> &others
									, vreal &znear);
	void DrawOthersInSelectionMode (const std::list<Display *> &others
									, vreal &znear
									, const vreal &zbody
									, std::list<bool> &isHidden);
	bool IsMySelfVis (const VPoint &p , const Display *myself , const vreal &zmyself);
	VVector m_zOffset;
	VVector m_xOffset;
	VVector m_yOffset;
	static vreal m_resolution;
};

#endif