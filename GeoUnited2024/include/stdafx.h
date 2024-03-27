// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef __VpCoreStdAfx_h__
#define __VpCoreStdAfx_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _MSC_VER_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning( disable : 4786 )

#undef __MWDXFREADER__

#include "stdafxwin.h"
#include "stdafxstd.h"

#define SAFE_DELETE(ptr) if (ptr) { delete ptr; ptr = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] p ;p = NULL ;}

// Vp 

#include "basicdefinitions.h"
#include "VStr.h"
#include "VTime.h"
#include "VpPreCompileDefinitions.h"
#include "Bifstream.h"
#include "Bofstream.h"
#include "TypeConverters.h"

//V_NO_MFC
#define V_NO_MFC
#ifdef V_NO_MFC
	#include "VMenu.h"
#endif


// viewpoint most used geometry types

#include "VException.h"
#include "LogFileWriter.h"
#include "VPoint.h"
#include "VPoint2d.h"
#include "VLine2d.h"
//#include "VLine2dE.h"
#include "CoordinateSystem.h"
#include "VTriangle.h"
#include "VTriangleE.h"
#include "VTriangle2d.h"
#include "VTriangle2dE.h"
#include "StrPool.h"

#ifdef __MWDXFREADER__
#include "VStringTableTranslater.h"
#include "VTTextMap.h"
#else
#include "VStringTableTranslater.h"
#include "VTTextMap.h"
#endif

#include "ViewPointDefaults.h"
#include "GUICustomize.h"
typedef std::vector<double> dVec;
typedef std::vector<VPoint> vpVec;
typedef std::vector<VLine> vlVec;
typedef std::vector<VPoint2d> vp2dVec;
typedef std::vector<VLine2d> vl2dVec;
#define sozluk VGlobalFunctions::GetVTTextMapToolbar()->Add
void VReadUnitParams();
void VWriteUnitParams();
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


class VGlobalFunctions
{
public:
	static std::vector<VStr> ParseSolidCncCommandLine ();
	static VStr GetSolidCncExeFolder (); // contains slash at the end
	static VStr GetSolidCncExeFolderOld(); // contains slash at the end


	static HINSTANCE m_solidCncResourceDllHi;
	static void SetVSolidCncResourceDllHi ( HINSTANCE solidCncResourceDllHi);
	//static const int v_maxChrN;
	//static VChr * v_chrArr;
	static VStr VLoadString (UINT nID);
	static CString VLoadString2 (UINT nID);
	static bool GetProductAndVersion(CString & strProductName, CString & strProductVersion);
private:
	static VStringTableTranslater *m_vstTranslater; 
	static VTTextMap * m_vttMapTree;
	static VTTextMap * m_vttMapToolbar;
public:
	static GUICustomize m_guiCustomize;
	static VStringTableTranslater * GetStringTableTranslater();
	static VTTextMap * GetVTTextMapTree();
	static VTTextMap * GetVTTextMapToolbar();
	static VStr m_solidCncExeFolder;
	static VStr EatOneLine(VStr &str);
};
class VGlobalValues
{
public:
	static const long MAX_ARRAY_CAPACITY;
	static const int DEFAULT_INITIAL_SIZE;
	static const vreal MYDERV_DIFF;
	static const vreal DERV_DIFF;
	static const int DOUBLE_SIGNIFICANT_DIGITS;
	static const int FLOAT_SIGNIFICANT_DIGITS;
	static const int DOUBLE_MAX_POWER_OF_TEN;
	static const int FLOAT_MAX_POWER_OF_TEN;
	static vreal TF_PRECISION;
	static vreal VUNIT_TO_MICRON;
	static vreal VUNIT_TO_TENMICRONS;
	static vreal VUNIT_TO_HUNDREDMICRONS;
	static vreal VUNIT_TO_MM;
	static vreal VUNIT_TO_CM;
	static vreal VUNIT_TO_DM;
	static vreal VUNIT_TO_METRE;
	static vreal ONEMICRON_TO_VUNIT;
	static vreal TENMICRONS_TO_VUNIT;
	static vreal HUNDREDMICRONS_TO_VUNIT;
	static vreal ONEMM_TO_VUNIT;
	static vreal ONECM_TO_VUNIT;
	static vreal ONEDM_TO_VUNIT;
	static vreal ONEMETER_TO_VUNIT;
	static VChr * VCHR_LINE_READER;
	static int VCHR_LINE_READERSIZE;
	static ViewPointDefaults viewpointdefaults;
	static vreal m_unitMultiplier;
	static bool m_silentMode;
	static unsigned int m_runTimeSeperator;
	static VChr m_numberSeperator;
	static int GCODE_PRECISION;// used for post processor
};

class TRNDC
{
private:
	std::vector<VStr, VStr> m_dataMap;
	VStr m_groupName  = "Global";
public :
	VStr Add(const VStr& str);
private:
	//void GetDataMapFromFile(const VStr & filePath);
	//void SaveDataMapToFile(const VStr & filePath);
};

class DC
{
private :
	static TRNDC m_trnDc;
public :
	static VStr Add(const VStr & str);
};

#include "VLine.h"
#include "VArc.h"

#ifdef __MWDXFREADER__
#include  "resource.h"
#else
#include "resource.h"
#endif

#endif 
