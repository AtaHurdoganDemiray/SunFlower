// ViewPointItemTypes.h

#ifndef __ViewPointItemTypes_h__
#define __ViewPointItemTypes_h__

class ViewPointItemTypes
{
public:
	static VStr m_stlmodeltype;
	static VStr m_coordinatesystemtype;
	static VStr m_pointtype;
	static VStr m_curvetype;	
	static VStr m_surfacetype;	
	static VStr m_treeviewfoldertype;
	static VStr m_multitext;
	static VStr m_recyclefolder;
	static VStr m_deleteditem;
	static VStr m_transform;
	static VStr m_viewpointmodel;
	static VStr m_viewvectormodel;
	static VStr m_boxtype;
	static VStr m_dimension;
	static VStr m_view;
	static VStr m_table;
	static VStr m_text;
	
	static VStr m_triangles;
	static VStr m_labeltext;
	static VStr m_vismod;
	static VStr m_sergroup;
	static VStr m_boundarycurve;
	static VStr m_allgeoser;
	static VStr m_vsolidmesh;
	static VStr m_wrlparser;
	static VStr m_containerSheet;
	static VStr m_wrlIndexedFaceSet;
	static VStr m_textureMap;
	static VStr m_solid2_5d;
	static VStr m_segmentedSolid;
	static VStr m_net;
	static VStr m_multiPoints;
	static VStr m_glassModule;
	static VStr m_solid2dGraphic;
	static VStr m_vHatch;
	static VStr m_sAutomation;
	static VStr m_path2dGraphic;
	static VStr m_vTransformationGroup;
	static VStr m_vRuntimeSeperatorRecorder;
	void SetLocalText (std::vector<VStr> &v);

	static const VStr StlModelEng ();
	static const VStr CoordinateSystemEng ();
	static const VStr PointEng ();
	static const VStr CurveEng ();
	static const VStr SurfaceEng ();
	static const VStr TreeViewFolderEng ();
	static const VStr MultiTextEng ();
	static const VStr RecycleFolderEng ();
	static const VStr DeletedItemEng ();
	static const VStr TransformEng () ;
	static const VStr ViewPointModelEng();
	static const VStr ViewVectorModelEng();
	static const VStr BoxEng ();
	static const VStr DimensionEng ();
	static const VStr ViewEng ();
	static const VStr TableEng ();
	static const VStr TextEng ();
	static const VStr TrianglesEng ();
	static const VStr LabelTextEng ();
	static const VStr VisModEng ();
	static const VStr SerGroupEng ();
	static const VStr BoundaryCurveEng ();
	static const VStr DimensionAngleEng ();
	static const VStr VpModelEng();
	static const VStr AllGeoSerEng();
	static const VStr VSolidMeshEng();
	static const VStr WRLParserEng();
	static const VStr ContainerSheetEng();
	static const VStr WRLIndexedFaceSetEng();
	static const VStr TextureMapEng();
	static const VStr Solid2_5dEng();
	static const VStr SegmentedSolidEng();
	static const VStr NetEng();
	static const VStr MultiPointsEng();
	static const VStr GlassModuleEng();
	static const VStr Solid2dGraphicEng();
	static const VStr VHatchEng();
	static const VStr SAutomationEng();
	static const VStr Path2dGraphicEng();
	static const VStr VTransformationGroupEng();
	static const VStr VRuntimeSeperatorRecorderEng();
};

#endif
