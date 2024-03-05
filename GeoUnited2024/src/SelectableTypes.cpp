// SelectableTypes.cpp

#include "stdafx.h"
#include "SelectableTypes.h"

#include "VColor.h"

SelectableTypes::SelectableTypes()
{
	m_selectablecolor = VColor(1., 1., 1.);//[0] = 1.;
	//selectablecolor[1] = 1.;
	//selectablecolor[2] = 1.;
	colorfilter = false;
	m_chainingCurvesDisabledFlag = false;
}

void SelectableTypes::AddSelectableType ( VStr newtype )
{
	sel_model_type.push_back ( newtype );
}

//#############################################################################

bool SelectableTypes::IsTypeSelectable ( VStr mytype ) const
{
	for ( unsigned int i = 0; i < sel_model_type.size(); i++ )
	{
		if ( mytype == ( sel_model_type[i] ))
			return true;
	}
	return false;
}

//#############################################################################

void SelectableTypes::ClearSelectableTypesVector ()
{
	sel_model_type.clear();
}

//#############################################################################

long SelectableTypes::Size () const
{
	return (long)(sel_model_type.size());
}

//#############################################################################

const VStr & SelectableTypes::operator [] ( int index ) const
{
	if ( index < 0 || index > Size() )
		throw ( VException ( ( IDS_ModSelFilExc) , 0 ) );

	return sel_model_type[index];
}

//#############################################################################

const SelectableTypes & SelectableTypes::operator = ( const SelectableTypes & Foo )
{
	if (this == &Foo)
		return *this;
	//selectablecolor[0] = Foo.selectablecolor[0];
	//selectablecolor[1] = Foo.selectablecolor[1];
	//selectablecolor[2] = Foo.selectablecolor[2];
	m_selectablecolor = Foo.m_selectablecolor;
	colorfilter = Foo.colorfilter;
	m_chainingCurvesDisabledFlag = Foo.colorfilter;
	ClearSelectableTypesVector ();

	for ( long i = 0; i < Foo.Size(); i++ )
	{
		AddSelectableType ( Foo[i] );
	}
	return *this;
}

bool SelectableTypes::IsColorSelectable ( const VColor & mycolor , const VColor &mycolor2) const
{
	//if ( mycolor.R() == selectablecolor[0]
		//&& mycolor.G() == selectablecolor[1]
		//&& mycolor.B() == selectablecolor[2] )
		//return true;
	//else
	//	return false;
	return (m_selectablecolor == mycolor || m_selectablecolor == mycolor2);
}

//#############################################################################

//void SelectableTypes::ClearSelectableTypesVector ()
//{
//	sel_model_type.ClearSelectableTypesVector ();
//}

//#############################################################################

void SelectableTypes::SetSelectableColor (const VColor &col)//( vreal newcolor[3] )
{
	//selectablecolor[0] = newcolor[0];
	//selectablecolor[1] = newcolor[1];
	//selectablecolor[2] = newcolor[2];
	m_selectablecolor = col;
}

//#############################################################################

const VColor & SelectableTypes::GetSelectableColor()const
{
	return m_selectablecolor;
}
void SelectableTypes::SetColorFilter ( bool newval )
{
	colorfilter = newval; 
}

//#############################################################################

bool SelectableTypes::GetColorFilter () const
{
	return colorfilter;
}

//#############################################################################

void SelectableTypes::RemoveSelectableType (VStr removetype)
{
	std::vector<VStr> temp;
	std::vector <VStr>::iterator i = sel_model_type.begin();
	std::vector<VStr>::iterator e = sel_model_type.end();
	for (i;i!=e;++i)
	{
		if(*i!=removetype)
		{
			temp.push_back (*i);
		}
	}
	sel_model_type=temp;
}

bool SelectableTypes::GetChainingCurvesDisabledFlag() const
{
	return false;//m_chainingCurvesDisabledFlag;
}

void SelectableTypes::SetChainingCurvesDisabledFlag (bool isChainingCurvesDisabled)
{
	m_chainingCurvesDisabledFlag = isChainingCurvesDisabled;
}
