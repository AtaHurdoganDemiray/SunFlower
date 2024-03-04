// ModelOrientation.cpp

#include "stdafx.h"
#include "ModelOrientation.h"
#include "Model.h"

void ModelOrientation::OrientModel ( Model * pmodel ) const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix ();
	
	double * p = pmodel->GetMatrixArr();
	double m[16] = { p[0] , p[4] , p[8] , p[12] 
				, p[1] , p[5] , p[9] , p[13] 
				, p[2] , p[6] , p[10] , p[14]
				, p[3] , p[7] , p[11] , p[15] };
	glMultMatrixd(m); 
	
}

void ModelOrientation::OriginalOrientation () const
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix ();
}
