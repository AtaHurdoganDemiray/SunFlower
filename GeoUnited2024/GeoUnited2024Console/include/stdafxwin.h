#ifndef __stdafxwin_h__
#define __stdafxwin_h__

// Windows MFC 
#include <afx.h>
#include <afxwin.h>
#include <afxcmn.h>
#include "atltime.h"
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxdhtml.h>
#include "atltime.h"
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef __MWDXFREADER__
#include "gl.h"
#else
#include "gl.h"
#endif

#include <afxole.h> 
#include <afxodlgs.h>

#include <afxcontrolbars.h>     
#include <afxrich.h>            
// openGL
//#include "../ogl/gl/gl.h"

// resource

// resource

int VpAfxMessageBox(LPCTSTR message, UINT nType = 0U, UINT nHelpId = 0U);


#endif