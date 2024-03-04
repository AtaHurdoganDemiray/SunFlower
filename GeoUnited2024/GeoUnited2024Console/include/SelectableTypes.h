// SelectableTypes.h

#ifndef __SelectableTypes_h__
#define __SelectableTypes_h__

#include "VColor.h"

class SelectableTypes
{
public:
	SelectableTypes();
	void AddSelectableType ( VStr newtype );
	void RemoveSelectableType(VStr removetype);
	bool IsTypeSelectable ( VStr mytype ) const;
	void ClearSelectableTypesVector ();
	void ClearUnselectableTypesVector();
	long Size() const;
	const VStr & operator[] ( int index ) const;
	const SelectableTypes & operator = ( const SelectableTypes & Foo );
	
	void SetSelectableColor ( const VColor &col );
	const VColor & GetSelectableColor()const;
	bool IsColorSelectable ( const VColor & mycolor , const VColor &mycolor2) const;
	void SetColorFilter ( bool newval );
	bool GetColorFilter () const;
	bool GetChainingCurvesDisabledFlag ()const;
	void SetChainingCurvesDisabledFlag (bool isChainingCurvesDisabled);
private:
	std::vector <VStr> sel_model_type;
	VColor m_selectablecolor;
	bool colorfilter;
	bool m_chainingCurvesDisabledFlag;
};

#endif
