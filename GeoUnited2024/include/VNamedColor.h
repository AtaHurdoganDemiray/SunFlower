// VNamedColor.h

#ifndef __VNamedColor_h__
#define __VNamedColor_h__

#include "VColor.h"

class VNamedColor
{
public:
	VNamedColor ();
	VNamedColor (const VStr &colorName , const VColor &color);
private:
	VStr m_colorName;
	VColor m_color;
public:
	const VStr & GetColorName()const;
	void SetColorName (const VStr &colorName);
	const VColor & GetColor()const;
	void SetColor (const VColor &color);
	void Serialize (Bofstream &f)const;
	void Serialize (Bifstream &f);
};

#endif