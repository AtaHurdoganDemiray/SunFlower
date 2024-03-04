// CoordSystemDisplay.h

#ifndef __CoordSystemDisplay_h__
#define __CoordSystemDisplay_h__

#include "VColor.h"
#include "Display.h"
#include "LineVisualAtt.h"
#include "CoordinateSystem.h"

class ViewVolume;
class TransformationMatrix;
struct TransformData;
class Display;

class CoordSystemDisplay
{
public:
	CoordSystemDisplay ( const CoordinateSystem * cs , LineVisualAtt & _visatt );
	CoordSystemDisplay ( CoordSystemDisplay & Foo );
	void Draw(const ViewVolume & view_volume, const std::vector<Display *> *asciiDisp) const;
	bool IsHit ( vreal & depthvalue ) const;
	void GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues ) const;
	LineVisualAtt & GetVisualAtt ();
	//void UpdateCoordSystemData ( const TransformationMatrix & tmatrix ,OglMatrix *x);
	//void UpdateCoordSystemData (const CoordinateSystem &newc);
	bool IsDefined() const;
	//std::list<TransformData*> xmats;
	//void AddMatrix(TransformData *x);
	//void MatrisClear();
	void DrawByScreenCoord (const float inform[] 
							,const float infx[] 
							,const float infy[] 
							,const float infz[]
							,const ViewVolume &view_volume
							, const std::vector<Display*> *asciiDisp)const;
	//TransformationMatrix GetOverallMat()const;
	void SetCs (const CoordinateSystem *cs);
//protected:
	//VColor displaycolor;
	LineVisualAtt visatt;
	VColor fontcolor;
	//vreal origin[3],dirx[3],diry[3],dirz[3];
	Display org;
	//CoordinateSystem originalCS;
	const CoordinateSystem *m_cs;
};

#endif