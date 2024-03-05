// ViewPointDefaults.cpp

#include "stdafx.h"
#include "ViewPointDefaults.h"

ViewPointDefaults::ViewPointDefaults()
{
	LoadFactoryDefaults();
}

//VColor VGlobalValues::viewpointdefaults.default_colupleft = VColor(0.0,0.0,0.25);// ( 0.462745 , 0.666666 , 0.819607 );
//VColor VGlobalValues::viewpointdefaults.default_colbottomleft = VColor(0.5,0.5,0.5);//( 1.0 , 1.0 , 1.0 );
//VColor VGlobalValues::viewpointdefaults.default_colbottomright = VColor(0.5,0.5,0.5);//( 1.0 , 1.0 , 1.0 );
//VColor VGlobalValues::viewpointdefaults.default_colupright = VColor(0.0,0.0,0.25);//( 0.462745 , 0.666666 , 0.819607 );
//VColor VGlobalValues::viewpointdefaults.m_defaultWireframeColor = VColor(1.0,1.0,1.0);
//VColor VGlobalValues::viewpointdefaults.m_defaultSurfaceColor = VColor(0.0,0.5,0.0);//(0.25,0.25,0.25);
//VColor VGlobalValues::viewpointdefaults.m_selectionColor = VColor (1.0,0.0,0.0);
//VColor VGlobalValues::viewpointdefaults.m_defaultPointColor = VColor(1.0,1.0,0.5);
//VColor VGlobalValues::viewpointdefaults.m_defaultGridColor = VColor(0.5,0.5,0.75);
//VColor VGlobalValues::viewpointdefaults.m_defaultOriginAxLineColor = VColor(0.75,0.75,0.75);
////bool VGlobalValues::viewpointdefaults.m_askSaveAtExit = false;
//VColor VGlobalValues::viewpointdefaults.m_defaultSurfaceWireColor = VColor (0.5,1.0,0.5);
//vreal VGlobalValues::viewpointdefaults.m_defaultLineWidth = 0.25;
//int VGlobalValues::viewpointdefaults.m_defaultLineFont = 0;
//int VGlobalValues::viewpointdefaults.m_defaultIsTranslucent = 0;
//vreal VGlobalValues::viewpointdefaults.m_defaultPointSize = 7.;
////int VGlobalValues::viewpointdefaults.m_isFreeSelectionEnabled = 1;
////int VGlobalValues::viewpointdefaults.m_isFreeSelectionFormVisible = 0;
//vreal VGlobalValues::viewpointdefaults.m_quickDisplayTol = 2.0;
//VColor VGlobalValues::viewpointdefaults.m_defaultTreeBackgroundColor = VColor (0.0,0.0,0.25);
//VColor VGlobalValues::viewpointdefaults.m_defaultTreeFontColor = VColor (1.0,1.0,1.0);
//VColor VGlobalValues::viewpointdefaults.m_defaultOverlapColor = VColor(0.0,1.0,0.0);
//VColor VGlobalValues::viewpointdefaults.m_solid2dGraphicOuterLoopColor = VColor(0.0, 1.0, 0.0);
//VColor VGlobalValues::viewpointdefaults.m_solid2dGraphicInnerLoopColor = VColor(1.0, 1.0, 0.843);
//vreal VGlobalValues::viewpointdefaults.m_solid2dGraphicOuterLoopLineWidth = 0.5;
//vreal VGlobalValues::viewpointdefaults.m_solid2dGraphicInnerLoopLineWidth = 0.2;
//int VGlobalValues::viewpointdefaults.m_solid2dGraphicOuterLoopLineFont = 0;
//int VGlobalValues::viewpointdefaults.m_solid2dGraphicInnerLoopLineFont = 0;
//vreal VGlobalValues::viewpointdefaults.m_defaultSnapPointSize = 4.0;
//VColor VGlobalValues::viewpointdefaults.m_defaultSnapPointColor = VColor(1.0,1.0,0.5);
//vreal VGlobalValues::viewpointdefaults.m_defaultFontWidth = 11.0;
//vreal VGlobalValues::viewpointdefaults.m_defaultFontHeight = 10.0;
//VColor VGlobalValues::viewpointdefaults.m_defaultFontColor = VColor(1.,1.,1.);
//VStr VGlobalValues::viewpointdefaults.m_defaultFont = "Courier New";

void ViewPointDefaults::LoadFactoryDefaults ()
{
	default_colupleft = VColor(0.0,0.0,0.25);// ( 0.462745 , 0.666666 , 0.819607 );
	default_colbottomleft = VColor (0.5,0.5,0.5);//( 1.0 , 1.0 , 1.0 );
	default_colbottomright = VColor (0.5,0.5,0.5);//( 1.0 , 1.0 , 1.0 );
	default_colupright = VColor (0.0,0.0,0.25);//( 0.462745 , 0.666666 , 0.819607 );
	//VColor VGlobalValues::viewpointdefaults.default_colupleft ( 0.0 , 0.0 , 0.0 );
	//VColor VGlobalValues::viewpointdefaults.default_colbottomleft ( 0.75 , 0.75 , 0.75 );
	//VColor VGlobalValues::viewpointdefaults.default_colbottomright ( 0.75 , 0.75 , 0.75 );
	//VColor VGlobalValues::viewpointdefaults.default_colupright ( 0.0 , 0.0 , 0.0 );
	m_defaultWireframeColor = VColor (1.0,1.0,1.0);
	m_defaultSurfaceColor = VColor (0.0,0.5,0.25);
	m_selectionColor = VColor (1.0,0.0,0.0);
	m_defaultPointColor = VColor (1.0,1.0,0.5);
	m_defaultGridColor = VColor (0.5,0.5,0.75);
	m_defaultOriginAxLineColor = VColor (0.75,0.75,0.75);
	//m_askSaveAtExit = false;
	m_defaultSurfaceWireColor = VColor (0.5,1.0,0.5);
	m_defaultLineWidth = 0.25;
	m_defaultLineFont = 0;
	m_defaultIsTranslucent = 0;
	m_defaultPointSize = 7.;
	//m_isFreeSelectionEnabled = 1;
	//m_isFreeSelectionFormVisible = 0;
	m_quickDisplayTol = 2.0;
	m_defaultOverlapColor = VColor(0.0,1.0,0.0);
	m_defaultTreeBackgroundColor = VColor (0.0,0.0,0.25);
	m_defaultTreeFontColor = VColor (1.0,1.0,1.0);

	m_solid2dGraphicOuterLoopColor = VColor(0.0, 1.0, 0.0);
	m_solid2dGraphicInnerLoopColor = VColor(1.0, 1.0, 0.843);
	m_solid2dGraphicOuterLoopLineWidth = 0.5;
	m_solid2dGraphicInnerLoopLineWidth = 0.2;
	m_solid2dGraphicOuterLoopLineFont = 0;
	m_solid2dGraphicInnerLoopLineFont = 0;
	m_defaultSnapPointSize = 4.0;
	m_defaultSnapPointColor = VColor(1.0 , 1.0 , 0.5);

	m_defaultFontWidth = 11.0;
	m_defaultFontHeight = 10.0;
	m_defaultFontColor = VColor(1., 1., 1.);
	m_defaultFont = "Courier New";

	m_doShowStatusText = TRUE;
	m_defaultSurfaceWireLineWidth = 1.0;
	m_colorMode = 0;

	m_surfaceColor2 = VColor(1.0,1.0,1.0);
	m_surfaceColor3 = VColor(1.0, 1.0, 1.0);
	m_surfaceColor4 = VColor(1.0, 1.0, 1.0);
	m_surfaceColor5 = VColor(1.0, 1.0, 1.0);
	m_surfaceColor6 = VColor(1.0, 1.0, 1.0);
	m_surfaceWireColor2 = VColor(1.0, 1.0, 1.0);
	m_surfaceWireColor3 = VColor(1.0, 1.0, 1.0);
	m_surfaceWireColor4 = VColor(1.0, 1.0, 1.0);
	m_surfaceWireColor5 = VColor(1.0, 1.0, 1.0);
	m_surfaceWireColor6 = VColor(1.0, 1.0, 1.0);
	m_wireColor2 = VColor(1.0, 1.0, 1.0);
	m_wireColor3 = VColor(1.0, 1.0, 1.0);
	m_wireColor4 = VColor(1.0, 1.0, 1.0);
	m_wireColor5 = VColor(1.0, 1.0, 1.0);
	m_wireColor6 = VColor(1.0, 1.0, 1.0);
	m_toolpathColor = VColor(1, 1, 0);
	m_toolpathColor2 = VColor(0, 0, 1);
	m_toolpathColor3 = VColor(1, 1, 1);
	m_toolpathColor4 = VColor(1, 0, 0);
	m_toolpathColor5 = VColor(0, 1.0, 0);
	m_toolpathColor6 = VColor(0, 1.0, 0.5);
	m_verySmallHitRadius = 0.01;
	m_hitRadiusRatio = 150.0;
	m_valueBoxColor = VColor(1.0,1.0,1.0);
	m_valueTextColor = VColor(1.,1.,0.0);
	m_valueFontHeight = 10.0;
	m_valueBoxHeight = 14.0;
	m_applicationLook = 0;
	m_toolbarsTransparent = FALSE;
	m_maxHistoryItemCount = 6;
	
	m_moveOnWorkSnapDist1 = 0.01;
	m_moveOnWorkSnapDist2 = 0.1;
	m_moveOnWorkSnapDist3 = 0.1;
	m_moveOnWorkSnapDist4 = 0.1;

	m_tableCellsColor1 = VColor(220./255., 230./255., 241./255.);
	m_tableCellsColor2 = VColor(184./255., 204./255., 228./255.);
	m_tableBackgroundColor = VColor(44.0 / 255.0, 44.0 / 255.0, 44.0 / 255.0);

	m_dialogBackgroundColorLight = VColor(211/255,211/255,211/255);
	m_dialogBackgroundColorDark = VColor(54/255,54/255,54/255);
	m_fontColorLight = VColor(211/255,211/255,211/255);
	m_fontColorDark = VColor(28/255,28/255,28/255);
	m_undefinedDXFColor = VColor(187,255,255);
}

std::map<VStr, vreal> ViewPointDefaults::GetNumericParameters()
{
	std::map<VStr, vreal> par;
	par[sozluk("LineWidth")] = m_defaultLineWidth;
	par[sozluk("PointSize")] = m_defaultPointSize;
	par[sozluk("QuickDisplayTol")] = m_quickDisplayTol;
	par[sozluk("Solid2dGraphicOuterLoopLineWidth")] = m_solid2dGraphicOuterLoopLineWidth;
	par[sozluk("Solid2dGraphicInnerLoopLineWidth")] = m_solid2dGraphicInnerLoopLineWidth;
	par[sozluk("SnapPointSize")] = m_defaultSnapPointSize;
	par[sozluk("FontWidth")] = m_defaultFontWidth;
	par[sozluk("FontHeight")] = m_defaultFontHeight;
	par[sozluk("SurfaceWireLineWidth")] = m_defaultSurfaceWireLineWidth;
	par[sozluk("HitRadiusRatio")] = m_hitRadiusRatio;
	par[sozluk("VerySmallHitRadius")] = m_verySmallHitRadius;
	par[sozluk("ValueBoxHeight")] = m_valueBoxHeight;
	par[sozluk("ValueFontHeight")] = m_valueFontHeight;
	par[sozluk("MaxHistoryItemCount")] = m_maxHistoryItemCount;
	par[sozluk("MoveOnWorkSnapDist1")] = m_moveOnWorkSnapDist1;
	par[sozluk("MoveOnWorkSnapDist2")] = m_moveOnWorkSnapDist2;
	par[sozluk("MoveOnWorkSnapDist3")] = m_moveOnWorkSnapDist3;
	par[sozluk("MoveOnWorkSnapDist4")] = m_moveOnWorkSnapDist4;
	return par;
}

std::map<VStr, VColor> ViewPointDefaults::GetColorParameters()
{
	std::map<VStr, VColor> par;
	par[sozluk("ColUpleft")] = default_colupleft;
	par[sozluk("ColBottomLeft")] = default_colbottomleft;
	par[sozluk("ColBottomRight")] = default_colbottomright;
	par[sozluk("ColUpRight")] = default_colupright;
	par[sozluk("WireframeColor")] = m_defaultWireframeColor;
	par[sozluk("SurfaceColor")] = m_defaultSurfaceColor;
	par[sozluk("SelectionColor")] = m_selectionColor;
	par[sozluk("PointColor")] = m_defaultPointColor;
	par[sozluk("GridColor")] = m_defaultGridColor;
	par[sozluk("OriginAxLineColor")] = m_defaultOriginAxLineColor;
	par[sozluk("SurfaceWireColor")] = m_defaultSurfaceWireColor;
	par[sozluk("OverlapColor")] = m_defaultOverlapColor;
	par[sozluk("TreeBackgroundColor")] = m_defaultTreeBackgroundColor;
	par[sozluk("TreeFontColor")] = m_defaultTreeFontColor;
	par[sozluk("Solid2dGraphicOuterLoopColor")] = m_solid2dGraphicOuterLoopColor;
	par[sozluk("Solid2dGraphicInnerLoopColor")] = m_solid2dGraphicInnerLoopColor;
	par[sozluk("SnapPointColor")] = m_defaultSnapPointColor;
	par[sozluk("FontColor")] = m_defaultFontColor;
	par[sozluk("SurfaceColor2")] = m_surfaceColor2;
	par[sozluk("SurfaceColor3")] = m_surfaceColor3;
	par[sozluk("SurfaceColor4")] = m_surfaceColor4;
	par[sozluk("SurfaceColor5")] = m_surfaceColor5;
	par[sozluk("SurfaceColor6")] = m_surfaceColor6;
	par[sozluk("SurfaceWireColor2")] = m_surfaceWireColor2;
	par[sozluk("SurfaceWireColor3")] = m_surfaceWireColor3;
	par[sozluk("SurfaceWireColor4")] = m_surfaceWireColor4;
	par[sozluk("SurfaceWireColor5")] = m_surfaceWireColor5;
	par[sozluk("SurfaceWireColor6")] = m_surfaceWireColor6;
	par[sozluk("WireColor2")] = m_wireColor2;
	par[sozluk("WireColor3")] = m_wireColor3;
	par[sozluk("WireColor4")] = m_wireColor4;
	par[sozluk("WireColor5")] = m_wireColor5;
	par[sozluk("WireColor6")] = m_wireColor6;
	par[sozluk("ToolpathColor")] = m_toolpathColor;
	par[sozluk("ToolpathColor2")] = m_toolpathColor2;
	par[sozluk("ToolpathColor3")] = m_toolpathColor3;
	par[sozluk("ToolpathColor4")] = m_toolpathColor4;
	par[sozluk("ToolpathColor5")] = m_toolpathColor5;
	par[sozluk("ToolpathColor6")] = m_toolpathColor6;
	par[sozluk("ValueBoxColor")] = m_valueBoxColor;
	par[sozluk("ValueFontColor")] = m_valueTextColor;
	par[sozluk("TableCellsColor1")] = m_tableCellsColor1;
	par[sozluk("TableCellsColor2")] = m_tableCellsColor2;
	par[sozluk("TableBackgroundColor")] = m_tableBackgroundColor;
	par[sozluk("DialogBackgroundColorDark")] = m_dialogBackgroundColorDark;
	par[sozluk("DialogBackgroundColorLight")] = m_dialogBackgroundColorLight;
	par[sozluk("FontColorDark")] = m_fontColorDark;
	par[sozluk("FontColorLight")] = m_fontColorLight;
	par[sozluk("UndefinedDXFColor")] = m_undefinedDXFColor;
	return par;
}

std::map<VStr, VStr> ViewPointDefaults::GetTextParameters()
{
	std::map<VStr, VStr> par;
	par[sozluk("Font")] = m_defaultFont;
	return par;
}

std::map<VStr, std::pair<std::vector<VStr>, int>> ViewPointDefaults::GetEnumParameters()
{
	std::map<VStr, std::pair<std::vector<VStr>,int>> par;
	std::vector<VStr> v1 = { sozluk("FALSE"), sozluk("TRUE") };
	//par["AskSaveAtExit"] = std::pair<std::vector<VStr>,int>(v1,m_askSaveAtExit);
	std::vector<VStr> v2 = { sozluk("Continueous"), sozluk("Dash dot"), sozluk("Dot") };
	par[sozluk("LineFont")] = std::pair<std::vector<VStr>, int>(v2, m_defaultLineFont);
	par[sozluk("IsTranslucent")] = std::pair<std::vector<VStr>, int>(v1, m_defaultIsTranslucent);
	//par["IsFreeSelectionEnabled"] = std::pair<std::vector<VStr> , int>(v1,m_isFreeSelectionEnabled);
	//par["IsFreeSelectionFormVisible"] = std::pair<std::vector<VStr>,int>(v1,m_isFreeSelectionFormVisible);
	par[sozluk("Solid2dGraphicOuterLoopLineFont")] = std::pair<std::vector<VStr>, int>(v2, m_solid2dGraphicOuterLoopLineFont);
	par[sozluk("Solid2dGraphicInnerLoopLineFont")] = std::pair<std::vector<VStr>, int>(v2, m_solid2dGraphicInnerLoopLineFont);
	par[sozluk("DoShowStatusText")] = std::pair<std::vector<VStr>, int>(v1,m_doShowStatusText);
	std::vector<VStr> v3 = {sozluk("Variable") , sozluk("Standard")};
	par[sozluk("ColorMode")] = std::pair<std::vector<VStr>, int>(v3,m_colorMode);
	std::vector<VStr> v4 = { sozluk("Blue style"), sozluk("Black style"), sozluk("Silver style"), sozluk("Aqua style") };
	par[sozluk("Application look")] = std::pair<std::vector<VStr>, int>(v4,m_applicationLook);
	par[sozluk("Toolbars transparent")] = std::pair<std::vector<VStr>, int>(v1, m_toolbarsTransparent);
	return par;
}

void ViewPointDefaults::SetNumericParameters(std::map<VStr, vreal> &par)
{
	try
	{
		m_defaultLineWidth = par[sozluk("LineWidth")];
		m_defaultPointSize = par[sozluk("PointSize")];
		m_quickDisplayTol = par[sozluk("QuickDisplayTol")];
		m_solid2dGraphicInnerLoopLineWidth = par[sozluk("Solid2dGraphicOuterLoopLineWidth")];
		m_solid2dGraphicInnerLoopLineWidth = par[sozluk("Solid2dGraphicInnerLoopLineWidth")];
		m_defaultSnapPointSize = par[sozluk("SnapPointSize")];
		m_defaultFontWidth = par[sozluk("FontWidth")];
		m_defaultFontHeight = par[sozluk("FontHeight")];
		m_defaultSurfaceWireLineWidth = par[sozluk("SurfaceWireLineWidth")];
		m_hitRadiusRatio = par[sozluk("HitRadiusRatio")];
		m_verySmallHitRadius = par[sozluk("VerySmallHitRadius")];
		m_valueBoxHeight = par[sozluk("ValueBoxHeight")];
		m_valueFontHeight = par[sozluk("ValueFontHeight")];
		m_maxHistoryItemCount = par[sozluk("MaxHistoryItemCount")];
		m_moveOnWorkSnapDist1 = par[sozluk("MoveOnWorkSnapDist1")];
		m_moveOnWorkSnapDist2 = par[sozluk("MoveOnWorkSnapDist2")];
		m_moveOnWorkSnapDist3 = par[sozluk("MoveOnWorkSnapDist3")];
		m_moveOnWorkSnapDist4 = par[sozluk("MoveOnWorkSnapDist4")];
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"ViewPointDefaults::SetNumericParameters");
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetNumericParameters");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "ViewPointDefaults::SetNumericParameters");
	}
}
void ViewPointDefaults::SetColorParameters(std::map<VStr, VColor> &par)
{
	try
	{
		default_colupleft = par[sozluk("ColUpleft")];
		default_colbottomleft = par[sozluk("ColBottomLeft")];
		default_colbottomright = par[sozluk("ColBottomRight")];
		default_colupright = par[sozluk("ColUpRight")];
		m_defaultWireframeColor = par[sozluk("WireframeColor")];
		m_defaultSurfaceColor = par[sozluk("SurfaceColor")];
		m_selectionColor = par[sozluk("SelectionColor")];
		m_defaultPointColor = par[sozluk("PointColor")];
		m_defaultGridColor = par[sozluk("GridColor")];
		m_defaultOriginAxLineColor = par[sozluk("OriginAxLineColor")];
		m_defaultSurfaceWireColor = par[sozluk("SurfaceWireColor")];
		m_defaultOverlapColor = par[sozluk("OverlapColor")];
		m_defaultTreeBackgroundColor = par[sozluk("TreeBackgroundColor")];
		m_defaultTreeFontColor = par[sozluk("TreeFontColor")];
		m_solid2dGraphicOuterLoopColor = par[sozluk("Solid2dGraphicOuterLoopColor")];
		m_solid2dGraphicInnerLoopColor = par[sozluk("Solid2dGraphicInnerLoopColor")];
		m_defaultSnapPointColor = par[sozluk("SnapPointColor")];
		m_defaultFontColor = par[sozluk("FontColor")];
		m_surfaceColor2 = par[sozluk("SurfaceColor2")];
		m_surfaceColor3 = par[sozluk("SurfaceColor3")];
		m_surfaceColor4 = par[sozluk("SurfaceColor4")];
		m_surfaceColor5 = par[sozluk("SurfaceColor5")];
		m_surfaceColor6 = par[sozluk("SurfaceColor6")];
		m_surfaceWireColor2 = par[sozluk("SurfaceWireColor2")];
		m_surfaceWireColor3 = par[sozluk("SurfaceWireColor3")];
		m_surfaceWireColor4 = par[sozluk("SurfaceWireColor4")];
		m_surfaceWireColor5 = par[sozluk("SurfaceWireColor5")];
		m_surfaceWireColor6 = par[sozluk("SurfaceWireColor6")];
		m_wireColor2 = par[sozluk("WireColor2")];
		m_wireColor3 = par[sozluk("WireColor3")];
		m_wireColor4 = par[sozluk("WireColor4")];
		m_wireColor5 = par[sozluk("WireColor5")];
		m_wireColor6 = par[sozluk("WireColor6")];
		m_toolpathColor = par[sozluk("ToolpathColor")];
		m_toolpathColor2 = par[sozluk("ToolpathColor2")];
		m_toolpathColor3 = par[sozluk("ToolpathColor3")];
		m_toolpathColor4 = par[sozluk("ToolpathColor4")];
		m_toolpathColor5 = par[sozluk("ToolpathColor5")];
		m_toolpathColor6 = par[sozluk("ToolpathColor6")];
		m_valueBoxColor = par[sozluk("ValueBoxColor")];
		m_valueTextColor = par[sozluk("ValueFontColor")];
		m_tableCellsColor1 = par[sozluk("TableCellsColor1")];
		m_tableCellsColor2 = par[sozluk("TableCellsColor2")];
		m_tableBackgroundColor = par[sozluk("TableBackgroundColor")];
		m_dialogBackgroundColorDark = par[sozluk("DialogBackgroundColorDark")];
		m_dialogBackgroundColorLight = par[sozluk("DialogBackgroundColorLight")];
		m_fontColorDark = par[sozluk("FontColorDark")];
		m_fontColorLight = par[sozluk("FontColorLight")];
		m_undefinedDXFColor = par[sozluk("UndefinedDXFColor")];
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetColorParameters");
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetColorParameters");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "ViewPointDefaults::SetColorParameters");
	}
}
void ViewPointDefaults::SetTextParameters(std::map<VStr, VStr> &par)
{
	try
	{
		m_defaultFont = par[sozluk("Font")];
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetTextParameters");
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetTextParameters");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "ViewPointDefaults::SetTextParameters");
	}

}
void ViewPointDefaults::SetEnumParameters(std::map<VStr, std::pair<std::vector<VStr>, int>> &par)
{
	try
	{
		m_defaultLineFont = par[sozluk("LineFont")].second;
		m_defaultIsTranslucent = par[sozluk("IsTranslucent")].second;
		m_solid2dGraphicOuterLoopLineFont = par[sozluk("Solid2dGraphicOuterLoopLineFont")].second;
		m_solid2dGraphicInnerLoopLineFont = par[sozluk("Solid2dGraphicInnerLoopLineFont")].second;
		m_doShowStatusText = par[sozluk("DoShowStatusText")].second;
		m_colorMode = par[sozluk("ColorMode")].second;
		m_applicationLook = par[sozluk("Application look")].second;
		m_toolbarsTransparent = par[sozluk("Toolbars transparent")].second;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetEnumParameters");
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ViewPointDefaults::SetEnumParameters");
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "ViewPointDefaults::SetEnumParameters");
	}
}

void ViewPointDefaults::Serialize(Bifstream &f)
{
	int version = 7;
	f.ReadInt(version);
	default_colupleft.Serialize(f);
	default_colbottomleft.Serialize(f);
	default_colbottomright.Serialize(f);
	default_colupright.Serialize(f);
	m_defaultWireframeColor.Serialize(f);
	m_defaultSurfaceColor.Serialize(f);
	m_selectionColor.Serialize(f);
	m_defaultPointColor.Serialize(f);
	m_defaultGridColor.Serialize(f);
	m_defaultOriginAxLineColor.Serialize(f);
	m_defaultSurfaceWireColor.Serialize(f);
	f.ReadDouble(m_defaultLineWidth);
	f.ReadInt(m_defaultLineFont);
	f.ReadInt(m_defaultIsTranslucent);
	f.ReadDouble(m_defaultPointSize);
	f.ReadDouble(m_quickDisplayTol);
	m_defaultOverlapColor.Serialize(f);
	m_defaultTreeBackgroundColor.Serialize(f);
	m_defaultTreeFontColor.Serialize(f);
	m_solid2dGraphicOuterLoopColor.Serialize(f);
	m_solid2dGraphicInnerLoopColor.Serialize(f);
	f.ReadDouble(m_solid2dGraphicOuterLoopLineWidth);
	f.ReadDouble(m_solid2dGraphicInnerLoopLineWidth);
	f.ReadInt(m_solid2dGraphicOuterLoopLineFont);
	f.ReadInt(m_solid2dGraphicInnerLoopLineFont);
	f.ReadDouble(m_defaultSnapPointSize);
	m_defaultSnapPointColor.Serialize(f);
	f.ReadDouble(m_defaultFontWidth);
	f.ReadDouble(m_defaultFontHeight);
	m_defaultFontColor.Serialize(f);
	f.ReadString(m_defaultFont);
	if (version >= 2)
	{
		f.ReadInt(m_doShowStatusText);
		f.ReadDouble(m_defaultSurfaceWireLineWidth);
	}
	if (version >= 3)
	{
		f.ReadInt(m_colorMode);
	}
	if (version >= 4)
	{
		m_surfaceColor2.Serialize(f);
		m_surfaceColor3.Serialize(f);
		m_surfaceColor4.Serialize(f);
		m_surfaceColor5.Serialize(f);
		m_surfaceColor6.Serialize(f);
		m_surfaceWireColor2.Serialize(f);
		m_surfaceWireColor3.Serialize(f);
		m_surfaceWireColor4.Serialize(f);
		m_surfaceWireColor5.Serialize(f);
		m_surfaceWireColor6.Serialize(f);
		m_wireColor2.Serialize(f);
		m_wireColor3.Serialize(f);
		m_wireColor4.Serialize(f);
		m_wireColor5.Serialize(f);
		m_wireColor6.Serialize(f);
		m_toolpathColor.Serialize(f);
		m_toolpathColor2.Serialize(f);
		m_toolpathColor3.Serialize(f);
		m_toolpathColor4.Serialize(f);
		m_toolpathColor5.Serialize(f);
		m_toolpathColor6.Serialize(f);
		f.ReadDouble(m_hitRadiusRatio);
		f.ReadDouble(m_verySmallHitRadius);
	}
	if (version >= 5)
	{
		f.ReadInt(m_applicationLook);
		f.ReadInt(m_toolbarsTransparent);
	}
	if (version >= 6)
	{
		f.ReadDouble(m_valueFontHeight);
		f.ReadDouble(m_valueBoxHeight);
		f.ReadInt(m_maxHistoryItemCount);
		f.ReadDouble(m_moveOnWorkSnapDist1);
		f.ReadDouble(m_moveOnWorkSnapDist2);
		f.ReadDouble(m_moveOnWorkSnapDist3);
		f.ReadDouble(m_moveOnWorkSnapDist4);
		m_valueBoxColor.Serialize(f);
		m_valueTextColor.Serialize(f);
		m_tableCellsColor1.Serialize(f);
		m_tableCellsColor2.Serialize(f);
		m_tableBackgroundColor.Serialize(f);
	}
	if (version >= 7)
	{
		m_dialogBackgroundColorDark.Serialize(f);
		m_dialogBackgroundColorLight.Serialize(f);
		m_fontColorDark.Serialize(f);
		m_fontColorLight.Serialize(f);
		m_undefinedDXFColor.Serialize(f);
	}
}

void ViewPointDefaults::Serialize(Bofstream &f)const
{
	int version = 7;
	f.WriteInt(version);
	default_colupleft.Serialize(f);
	default_colbottomleft.Serialize(f);
	default_colbottomright.Serialize(f);
	default_colupright.Serialize(f);
	m_defaultWireframeColor.Serialize(f);
	m_defaultSurfaceColor.Serialize(f);
	m_selectionColor.Serialize(f);
	m_defaultPointColor.Serialize(f);
	m_defaultGridColor.Serialize(f);
	m_defaultOriginAxLineColor.Serialize(f);
	m_defaultSurfaceWireColor.Serialize(f);
	f.WriteDouble(m_defaultLineWidth);
	f.WriteInt(m_defaultLineFont);
	f.WriteInt(m_defaultIsTranslucent);
	f.WriteDouble(m_defaultPointSize);
	f.WriteDouble(m_quickDisplayTol);
	m_defaultOverlapColor.Serialize(f);
	m_defaultTreeBackgroundColor.Serialize(f);
	m_defaultTreeFontColor.Serialize(f);
	m_solid2dGraphicOuterLoopColor.Serialize(f);
	m_solid2dGraphicInnerLoopColor.Serialize(f);
	f.WriteDouble(m_solid2dGraphicOuterLoopLineWidth);
	f.WriteDouble(m_solid2dGraphicInnerLoopLineWidth);
	f.WriteInt(m_solid2dGraphicOuterLoopLineFont);
	f.WriteInt(m_solid2dGraphicInnerLoopLineFont);
	f.WriteDouble(m_defaultSnapPointSize);
	m_defaultSnapPointColor.Serialize(f);
	f.WriteDouble(m_defaultFontWidth);
	f.WriteDouble(m_defaultFontHeight);
	m_defaultFontColor.Serialize(f);
	f.WriteString(m_defaultFont);
	f.WriteInt(m_doShowStatusText);
	f.WriteDouble(m_defaultSurfaceWireLineWidth);
	f.WriteInt(m_colorMode);
	m_surfaceColor2.Serialize(f);
	m_surfaceColor3.Serialize(f);
	m_surfaceColor4.Serialize(f);
	m_surfaceColor5.Serialize(f);
	m_surfaceColor6.Serialize(f);
	m_surfaceWireColor2.Serialize(f);
	m_surfaceWireColor3.Serialize(f);
	m_surfaceWireColor4.Serialize(f);
	m_surfaceWireColor5.Serialize(f);
	m_surfaceWireColor6.Serialize(f);
	m_wireColor2.Serialize(f);
	m_wireColor3.Serialize(f);
	m_wireColor4.Serialize(f);
	m_wireColor5.Serialize(f);
	m_wireColor6.Serialize(f);
	m_toolpathColor.Serialize(f);
	m_toolpathColor2.Serialize(f);
	m_toolpathColor3.Serialize(f);
	m_toolpathColor4.Serialize(f);
	m_toolpathColor5.Serialize(f);
	m_toolpathColor6.Serialize(f);
	f.WriteDouble(m_hitRadiusRatio);
	f.WriteDouble(m_verySmallHitRadius);
	f.WriteInt(m_applicationLook);
	f.WriteInt(m_toolbarsTransparent);
	
	// vvvvvv under is version 6 vvvvv
	f.WriteDouble(m_valueFontHeight);
	f.WriteDouble(m_valueBoxHeight);
	f.WriteInt(m_maxHistoryItemCount);
	f.WriteDouble(m_moveOnWorkSnapDist1);
	f.WriteDouble(m_moveOnWorkSnapDist2);
	f.WriteDouble(m_moveOnWorkSnapDist3);
	f.WriteDouble(m_moveOnWorkSnapDist4);
	m_valueBoxColor.Serialize(f);
	m_valueTextColor.Serialize(f);
	m_tableCellsColor1.Serialize(f);
	m_tableCellsColor2.Serialize(f);
	m_tableBackgroundColor.Serialize(f);

	//vvvvvvv version7
	m_dialogBackgroundColorDark.Serialize(f);
	m_dialogBackgroundColorLight.Serialize(f);
	m_fontColorDark.Serialize(f);
	m_fontColorLight.Serialize(f);
	m_undefinedDXFColor.Serialize(f);
}




