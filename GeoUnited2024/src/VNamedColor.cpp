// VNamedColor.cpp

#include "stdafx.h"
#include "VNamedColor.h"

VNamedColor::VNamedColor()
{
	m_color = VColor (0.,1.,0.);
	m_colorName = VStr("Green");
}

VNamedColor::VNamedColor (const VStr &colorName , const VColor &color)
	:m_colorName (colorName),m_color(color)
{

}

const VStr & VNamedColor::GetColorName()const
{
	return m_colorName;
}

void VNamedColor::SetColorName (const VStr &colorName)
{
	m_colorName = colorName;
}

const VColor & VNamedColor::GetColor()const
{
	return m_color;
}

void VNamedColor::SetColor (const VColor &color)
{
	m_color = color;
}

void VNamedColor::Serialize (Bofstream &f)const
{
	int version = 1;
	f.WriteInt (version);
	f.WriteString (m_colorName);
	m_color.Serialize (f);
}

void VNamedColor::Serialize (Bifstream &f)
{
	int version;
	f.ReadInt (version);
	if (version >=1)
	{
		f.ReadString (m_colorName);
		m_color.Serialize (f);
	}
}