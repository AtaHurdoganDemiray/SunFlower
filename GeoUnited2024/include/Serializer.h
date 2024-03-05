// Serializer.h: interface for the Serializer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALIZER_H__59EB8AA9_3C1D_4AA4_899B_8451C608EE32__INCLUDED_)
#define AFX_SERIALIZER_H__59EB8AA9_3C1D_4AA4_899B_8451C608EE32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class coreSerializer  
{
public:
	coreSerializer();
	virtual ~coreSerializer();
	VStr MakeUpper ( const VStr & source ) const;
	VStr MakeLower ( const VStr & source ) const;
	bool IsUpper ( VChr letter ) const;
	bool IsLower ( VChr letter ) const;
	
};

#endif // !defined(AFX_SERIALIZER_H__59EB8AA9_3C1D_4AA4_899B_8451C608EE32__INCLUDED_)
