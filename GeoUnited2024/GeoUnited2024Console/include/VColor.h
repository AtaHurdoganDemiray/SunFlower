// VColor.h

#ifndef __VColor_h__
#define __VColor_h__
#include "Bifstream.h"
#include "Bofstream.h"
class ColorDefinition;

struct VColor
{
public:
	VColor ();
	VColor ( const vreal & _R , const vreal & _G , const vreal & _B );
	VColor ( VIFStream & infile );
	VColor (COLORREF color);
	const vreal & R() const;
	const vreal & G() const;
	const vreal & B() const;
	void R ( const vreal & _R );
	void G ( const vreal & _G );
	void B ( const vreal & _B );
	void operator >> ( VofStream & outfile ) const;
	void operator << ( VIFStream & infile );
	void Serialize (Bofstream &binfile)const;
	void Serialize (Bifstream &binfile);
	const ColorDefinition GetColorDefinition ()const;
	const bool operator == (const VColor &tock)const;
	COLORREF Convert ()const;
	VColor GetInverseColor()const;
protected:
	vreal r , g , b;
public:
	int Ri()const;
	int Gi()const;
	int Bi()const;
	VColor (int rgb[3]);
};

#endif