// VpTestCode.h

#ifndef __VpTestCode_h__
#define __VpTestCode_h__

class VpTestCode
{
public:

void /*OGL_Frame::*/DrawMultiTextForPoints ()
{
	vreal xwidth = 100.;
	vreal ywidth = 20.;
	PointSet tmp(6);
	tmp.InsertPoint ( VPoint (0,0,0) );
	tmp.InsertPoint ( VPoint (5,0,0) );
	tmp.InsertPoint ( VPoint (0,5,0) );
	tmp.InsertPoint ( VPoint (5,5,0) );

	std::vector <CString> tmpvec;
	tmpvec.push_back ( ( "p1 : 2.1" ) );
	tmpvec.push_back ( ( "p2 : 1.7" ) );
	tmpvec.push_back ( ( "p3 : 1.9" ) );
	tmpvec.push_back ( ( "p4 : 2.0" ) );
	
	MultiTextToPointsObject mto 
		( tmp , tmpvec , view_volume , xwidth , ywidth );
	mto.Draw();
}

//#############################################################################

};

#endif

