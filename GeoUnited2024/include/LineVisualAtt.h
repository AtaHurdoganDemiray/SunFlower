// LineVisualAtt.h

#ifndef __LineVisualAtt_h__
#define __LineVisualAtt_h__

#include "VColor.h"

class LineVisualAtt
{
public:
	enum LineFont { LINEFONT_NORMAL , LINEFONT_DASHDOT , LINEFONT_DOTTED };
	enum LineWidth {thin,normal,mid,thick};
	LineVisualAtt ();
	//LineVisualAtt ( VIFStream & infile );
	const VColor & GetWireColor() const;	
	void ChangeWireColor ( const VColor & newcolor );
	void SetLineWidth ( const vreal & newwidth );
	void SetLineWidth (LineWidth widthtype);
	const vreal & GetLineWidth () const;
	void SetLineFont ( LineFont newfont );
	LineFont GetLineFont () const;
	void SetDraw ( bool newdraw );
	bool GetDraw() const;
	//void operator >> ( VofStream & outfile ) const;
	//void operator << ( VIFStream & infile );
	void Serialize(Bofstream &outfile)const;
	void Serialize(Bifstream &infile);
	const bool operator == (const LineVisualAtt &tock)const;
protected:
	bool draw;	
	VColor wirecolor;
	vreal linewidth;
	LineFont linefont;
};

#endif