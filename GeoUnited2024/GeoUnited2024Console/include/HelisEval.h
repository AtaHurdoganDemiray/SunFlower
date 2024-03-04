// HelisEval.h

#ifndef __HelisEval_h__
#define __HelisEval_h__

#include "CoordinateSystem.h"
//#include "HelisGraph.h"

struct VPoint;
class CommandToViewPoint;

// The helis is assumed to rotate around z axis of the coordinate system , with pitch 
// , start and end angles are defined relative to x axis of the coordinate system
// startz and endz are the z coordinates 

class HelisEval
{
public:
	HelisEval (const CoordinateSystem &cs , double pitch , double startangle , double startz , double endz , double startradius , double endradius , double tiltangle = 0.,int anglesign = 1);
	const double & GetPitch ()const;
	void SetPitch (double pitch);
	const double & GetStartAngle ()const;
	void SetStartAngle (double startangle);
	const double & GetStartZ ()const;
	void SetStartZ (double startz);
	const double & GetEndZ ()const;
	void SetEndZ(double endz);
	const double & GetStartRadius ()const;
	void SetStartRadius (double startradius);
	const double & GetEndRadius ()const;
	void SetEndRadius (double endradius);
	VPoint GetPoint1 (double angle)const;
	VPoint GetPoint2 (double zcoord)const;
	double GetRadius (double z)const;
	double GetAngle (double z)const;
	double GetZ (double angle)const;
	VPoint GetCylinderNormal1 (double angle)const;
	VPoint GetCylinderNormal2 (double z)const;
	const double & GetEndAngle ()const;
	VStr GetInfoTxt ()const;
	//HelisGraph &GetGraph ();
	//const HelisGraph & GetGraph()const;
	const VStr & GetName()const;
	void SetName (const VStr &name);
	//void MakeGraph (CommandToViewPoint &ap,double step);
	double GetTiltAngle ()const;
	void SetTiltAngle (double tiltangle);
	VPoint GetToolAxis (double angle)const;
	double GetTiltAngle2()const;
	void SetTiltAngle2(double angle);
	int GetAngleSign ()const;
	void SetAngleSign (int anglesign);
	const CoordinateSystem & GetCs ()const;
	void SetCs (const CoordinateSystem &cs);
private:
	CoordinateSystem m_cs;
	double m_pitch;
	double m_startangle;
	double m_startz;
	double m_endz;
	double m_startradius;
	double m_endradius;

	double m_deltaZ;
	double m_deltaRadF;
	double m_endAngle;
	double m_pitchInv;
	
	double m_inv360;
	//HelisGraph m_graph;
	VStr m_name;
	// m_tiltangle is the angle of tool axis around y where z is turning axis x is radial axis (part coordinate system)
	double m_tiltangle;
	double m_angleAroundZ;
	// m_tiltangle2 is the angle between tool axis and x axis around z axis where z is turning axis and x is radial axis (part coordinate system)
	double m_tiltangle2;

	int m_anglesign;
	void ReverseThis();
};

#endif

