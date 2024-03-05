// PointVisualAtt.h

#ifndef __PointVisualAtt_h__
#define __PointVisualAtt_h__
#include "VColor.h"

class PointVisualAtt
{
public:
	PointVisualAtt();
	//PointVisualAtt ( VIFStream & infile );
	const VColor & GetPointColor() const;	
	void SetPointSize ( const vreal & newsize );
	const vreal & GetPointSize() const;
	void ChangePointColor ( const VColor & newcolor );
	void SetDraw ( bool newdraw );
	bool GetDraw () const;
	//void operator >> ( VofStream & outfile ) const;
	//void operator << ( VIFStream & infile );
	void Serialize(Bofstream &outfile)const;
	void Serialize(Bifstream &infile);
	const bool operator == (const PointVisualAtt &tock)const;
protected:
	bool draw;
	VColor pointcolor;
	vreal pointsize;
};

#endif