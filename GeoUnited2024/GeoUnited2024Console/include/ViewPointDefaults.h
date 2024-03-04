// ViewPointDefaults.h

#ifndef __ViewPointDefaults_h__
#define __ViewPointDefaults_h__

#include "VColor.h"

const VStr VIEWPOINTRESOURCEDIR = ("C:/positive/viewpoint/UI/_Turkce/res");
class PointVisualAtt;
class LineVisualAtt;
class FacetVisualAtt;

class ViewPointDefaults
{
public:
	ViewPointDefaults();
	VColor default_colupleft;
	VColor default_colbottomleft;
	VColor default_colbottomright;
	VColor default_colupright;
	VColor m_defaultWireframeColor;
	VColor m_defaultSurfaceColor;
	VColor m_selectionColor;
	VColor m_defaultPointColor;
	VColor m_defaultGridColor;
	VColor m_defaultOriginAxLineColor;
	void LoadFactoryDefaults();
	//static bool m_askSaveAtExit;
	VColor m_defaultSurfaceWireColor;
	vreal m_defaultLineWidth;
	int m_defaultLineFont;
	int m_defaultIsTranslucent;
	vreal m_defaultPointSize;
	//static int m_isFreeSelectionEnabled;
	//static int m_isFreeSelectionFormVisible;
	vreal m_quickDisplayTol;
	VColor m_defaultOverlapColor;
	VColor m_defaultTreeBackgroundColor;
	VColor m_defaultTreeFontColor;
	VColor m_solid2dGraphicOuterLoopColor;
	VColor m_solid2dGraphicInnerLoopColor;
	vreal m_solid2dGraphicOuterLoopLineWidth;
	vreal m_solid2dGraphicInnerLoopLineWidth;
	int m_solid2dGraphicOuterLoopLineFont;
	int m_solid2dGraphicInnerLoopLineFont;
	vreal m_defaultSnapPointSize;
	VColor m_defaultSnapPointColor;
	vreal m_defaultFontWidth;
	vreal m_defaultFontHeight;
	VColor m_defaultFontColor;
	VStr m_defaultFont;
	int m_doShowStatusText;
	vreal m_defaultSurfaceWireLineWidth;
	int m_colorMode;
	VColor m_surfaceColor2;
	VColor m_surfaceColor3;
	VColor m_surfaceColor4;
	VColor m_surfaceColor5;
	VColor m_surfaceColor6;
	VColor m_surfaceWireColor2;
	VColor m_surfaceWireColor3;
	VColor m_surfaceWireColor4;
	VColor m_surfaceWireColor5;
	VColor m_surfaceWireColor6;
	VColor m_wireColor2;
	VColor m_wireColor3;
	VColor m_wireColor4;
	VColor m_wireColor5;
	VColor m_wireColor6;
	VColor m_toolpathColor;
	VColor m_toolpathColor2;
	VColor m_toolpathColor3;
	VColor m_toolpathColor4;
	VColor m_toolpathColor5;
	VColor m_toolpathColor6;
	vreal m_hitRadiusRatio;
	vreal m_verySmallHitRadius;
	VColor m_valueBoxColor;
	VColor m_valueTextColor;
	vreal m_valueFontHeight;
	vreal m_valueBoxHeight;
	int m_applicationLook;
	int m_toolbarsTransparent;
	int m_maxHistoryItemCount;
	vreal m_moveOnWorkSnapDist1;
	vreal m_moveOnWorkSnapDist2;
	vreal m_moveOnWorkSnapDist3;
	vreal m_moveOnWorkSnapDist4;
	VColor m_tableCellsColor1;
	VColor m_tableCellsColor2;
	VColor m_tableBackgroundColor;
	VColor m_dialogBackgroundColorLight;
	VColor m_dialogBackgroundColorDark;
	VColor m_fontColorLight;
	VColor m_fontColorDark;
	VColor m_undefinedDXFColor;
	std::map<VStr, vreal> GetNumericParameters();
	std::map<VStr, VColor> GetColorParameters();
	std::map<VStr, VStr> GetTextParameters();
	std::map<VStr, std::pair<std::vector<VStr>,int>> GetEnumParameters();

	void Serialize(Bifstream &f);
	void Serialize(Bofstream &f)const;

	void SetNumericParameters(std::map<VStr,vreal> &par);
	void SetColorParameters(std::map<VStr,VColor> &par);
	void SetTextParameters(std::map<VStr, VStr> &textParameters);
	void SetEnumParameters(std::map<VStr,std::pair<std::vector<VStr>,int>> &par);

	static void FillDefaults(PointVisualAtt &pvis , LineVisualAtt &lvis , FacetVisualAtt &fvis);
	static void FillDefaults2(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis);
	static void FillDefaults3(PointVisualAtt &pvis, LineVisualAtt &lvis, FacetVisualAtt &fvis);
};

#endif
