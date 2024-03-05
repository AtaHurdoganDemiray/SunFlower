// ViewPointItemTypes.cpp

#include "stdafx.h"
#include "ViewPointItemTypes.h"


 VStr ViewPointItemTypes::m_stlmodeltype(( IDS_StlModel));
 VStr ViewPointItemTypes::m_coordinatesystemtype=( IDS_CoordinateSyste);
 VStr ViewPointItemTypes::m_pointtype=( IDS_Point);
 VStr ViewPointItemTypes::m_curvetype=( IDS_Curve);	
 VStr ViewPointItemTypes::m_surfacetype=( IDS_Surface);	
 VStr ViewPointItemTypes::m_treeviewfoldertype=( IDS_Folder);
 VStr ViewPointItemTypes::m_multitext=( IDS_InfoTexts);
 VStr ViewPointItemTypes::m_recyclefolder=( IDS_RecycleBin);
 VStr ViewPointItemTypes::m_deleteditem=( IDS_RecycledItem);
 VStr ViewPointItemTypes::m_transform=( IDS_Transformation);
 VStr ViewPointItemTypes::m_viewpointmodel=( IDS_Point);
 VStr ViewPointItemTypes::m_viewvectormodel=( IDS_Vector);
 VStr ViewPointItemTypes::m_boxtype=( IDS_Box);
 VStr ViewPointItemTypes::m_dimension=( IDS_Dimension);
 VStr ViewPointItemTypes::m_view=( IDS_View);
 VStr ViewPointItemTypes::m_table=( IDS_Table2);
 VStr ViewPointItemTypes::m_text=( IDS_Text1);
 VStr ViewPointItemTypes::m_triangles=( IDS_TriangleModel);
 VStr ViewPointItemTypes::m_labeltext=( IDS_Label1);
 VStr ViewPointItemTypes::m_vismod = ( IDS_VisualModel);
 VStr ViewPointItemTypes::m_sergroup = ( IDS_SerGroup);
 VStr ViewPointItemTypes::m_boundarycurve = ( IDS_BCurve);
 VStr ViewPointItemTypes::m_allgeoser = ( IDS_AllGeoSer);
 VStr ViewPointItemTypes::m_vsolidmesh = ( IDS_VSolidMesh);
 VStr ViewPointItemTypes::m_wrlparser = ( IDS_WRLParser);
 VStr ViewPointItemTypes::m_containerSheet = ( IDS_ContainerSheet);
 VStr ViewPointItemTypes::m_wrlIndexedFaceSet = ( IDS_WRLIndexedFaceS);
 VStr ViewPointItemTypes::m_textureMap = ( IDS_TextureMap);
 VStr ViewPointItemTypes::m_solid2_5d = ( IDS_SolidD);
 VStr ViewPointItemTypes::m_segmentedSolid = "SegmentedSolid";
 VStr ViewPointItemTypes::m_net = "Net";
 VStr ViewPointItemTypes::m_multiPoints = "MultiPoints";
 VStr ViewPointItemTypes::m_glassModule = "GlassModule";
 VStr ViewPointItemTypes::m_solid2dGraphic = "Solid2dGraphic";
 VStr ViewPointItemTypes::m_vHatch = "VHatch";
 VStr ViewPointItemTypes::m_sAutomation = "SAutomation";
 VStr ViewPointItemTypes::m_path2dGraphic = "Path2dGraphic";
 VStr ViewPointItemTypes::m_vTransformationGroup = "VTransformationGroup";
 VStr ViewPointItemTypes::m_vRuntimeSeperatorRecorder = "VRuntimeSeperatorRecorder";

void ViewPointItemTypes::SetLocalText (std::vector<VStr> &v)
{
	m_stlmodeltype=v[0];
	m_coordinatesystemtype=v[1];
	m_pointtype=v[2];
	m_curvetype=v[3];
	m_surfacetype=v[4];
	m_treeviewfoldertype=v[5];
	m_multitext=v[6];
	m_recyclefolder=v[7];
	m_deleteditem=v[8];
	m_transform=v[9];
	m_viewpointmodel=v[10];
	m_viewvectormodel=v[11];
	m_boxtype=v[12];
	m_dimension=v[13];
	m_view=v[14];
	m_table=v[15];
	m_text=v[16];
	m_triangles=v[17];
	m_labeltext=v[18];
	m_vismod = v[19];
	m_boundarycurve = v[20];
}

const VStr ViewPointItemTypes::StlModelEng ()
{
	return ( IDS_StlModel);
}
const VStr ViewPointItemTypes::CoordinateSystemEng ()
{
	return ( IDS_CoordinateSyste);
}
const VStr ViewPointItemTypes::PointEng ()
{
	return ( IDS_Point);
}
const VStr ViewPointItemTypes::CurveEng ()
{
	return ( IDS_Curve);	
}
const VStr ViewPointItemTypes::SurfaceEng ()
{
	return ( IDS_Surface);	
}
const VStr ViewPointItemTypes::TreeViewFolderEng ()
{
	return ( IDS_Folder);
}
const VStr ViewPointItemTypes::MultiTextEng ()
{
	return ( IDS_InfoTexts);
}
const VStr ViewPointItemTypes::RecycleFolderEng ()
{
	return ( IDS_RecycleBin);
}
const VStr ViewPointItemTypes::DeletedItemEng ()
{
	return ( IDS_DeletedItem);
}
const VStr ViewPointItemTypes::TransformEng () 
{
	return ( IDS_Transformation);
}
const VStr ViewPointItemTypes::ViewPointModelEng()
{
	return ( IDS_VPoint);
}
const VStr ViewPointItemTypes::ViewVectorModelEng()
{
	return ( IDS_VVector);
}
const VStr ViewPointItemTypes::BoxEng ()
{
	return ( IDS_Box);
}
const VStr ViewPointItemTypes::DimensionEng ()
{
	return ( IDS_Dimension);
}
const VStr ViewPointItemTypes::ViewEng ()
{
	return ( IDS_View);
}
const VStr ViewPointItemTypes::TableEng ()
{
	return ( IDS_Table2);
}
const VStr ViewPointItemTypes::TextEng ()
{
	return ( IDS_Text1);
}
const VStr ViewPointItemTypes::TrianglesEng ()
{
	return ( IDS_TriangleModel);
}
const VStr ViewPointItemTypes::LabelTextEng ()
{
	return ( IDS_Label1);
}
const VStr ViewPointItemTypes::VisModEng ()
{
	return ( IDS_VisualModel);
}
const VStr ViewPointItemTypes::SerGroupEng ()
{
	return ( IDS_SerGroup);
}
const VStr ViewPointItemTypes::BoundaryCurveEng ()
{
	return ( IDS_BCurve);
}
const VStr ViewPointItemTypes::DimensionAngleEng ()
{
	return ( IDS_DimensionAngle);
}
const VStr ViewPointItemTypes::VpModelEng()
{
	return ( IDS_VpModel);
}

const VStr ViewPointItemTypes::AllGeoSerEng()
{
	return ( IDS_AllGeoSer);
}

const VStr ViewPointItemTypes::VSolidMeshEng()
{
	return ( IDS_VSolidMesh);
}

const VStr ViewPointItemTypes::WRLParserEng()
{
	return ( IDS_WRLParser);
}

const VStr ViewPointItemTypes::ContainerSheetEng()
{
	return ( IDS_ContainerSheet);
}

const VStr ViewPointItemTypes::WRLIndexedFaceSetEng()
{
	return ( IDS_WRLIndexedFaceS);
}

const VStr ViewPointItemTypes::TextureMapEng()
{
	return ( IDS_TextureMap);
}

const VStr ViewPointItemTypes::Solid2_5dEng()
{
	return ( IDS_SolidD);
}

const VStr ViewPointItemTypes::SegmentedSolidEng()
{
	return "SegmentedSolid";
}

const VStr ViewPointItemTypes::NetEng()
{
	return "Net";
}

const VStr ViewPointItemTypes::MultiPointsEng()
{
	return "MultiPoints";
}

const VStr ViewPointItemTypes::GlassModuleEng()
{
	return "GlassModule";
}
const VStr ViewPointItemTypes::Solid2dGraphicEng()
{
	return "Solid2dGraphic";
}
const VStr ViewPointItemTypes::VHatchEng()
{
	return "VHatch";
}

const VStr ViewPointItemTypes::SAutomationEng()
{
	return "SAutomation";
}

const VStr ViewPointItemTypes::Path2dGraphicEng()
{
	return "Path2dGraphic";
}

const VStr ViewPointItemTypes::VTransformationGroupEng()
{
	return ("VTransformationGroup");
}

const VStr ViewPointItemTypes::VRuntimeSeperatorRecorderEng()
{
	return "VRuntimeSeperatorRecorder";
}