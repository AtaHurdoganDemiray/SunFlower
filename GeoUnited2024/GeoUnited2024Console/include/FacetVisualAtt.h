// FacetVisualAtt.h

#ifndef __FacetVisualAtt_h__
#define __FacetVisualAtt_h__

#include "VColor.h"

class FacetVisualAtt
{
public:
	FacetVisualAtt();
	//FacetVisualAtt ( VIFStream & infile );
	const VColor & GetShadeColor() const;
	void ChangeShadeColor ( const VColor & newcolor );
	void SetTranslucent ( bool newval );
	bool GetTranslucent () const;
	void SetDraw ( bool newdraw );
	bool GetDraw () const;
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	//const VUChr * GetHalfTone ()const;
	//void operator >> ( VofStream & outfile )const;
	//void operator << ( VIFStream & infile );
	const bool operator == (const FacetVisualAtt &tock)const;
protected:
	bool draw;
	VColor shadecolor;
	bool translucent;
};

#endif