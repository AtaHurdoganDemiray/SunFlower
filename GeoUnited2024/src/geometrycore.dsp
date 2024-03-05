# Microsoft Developer Studio Project File - Name="geometrycore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=geometrycore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "geometrycore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "geometrycore.mak" CFG="geometrycore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "geometrycore - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "geometrycore - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "geometrycore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"pch.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "..\core" /I "..\ogl" /I "..\igesserialize" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"pch.h" /FD /c
# ADD BASE RSC /l 0x41f /d "NDEBUG"
# ADD RSC /l 0x41f /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\geometrycore.lib ..\PlugIns\Library\Release\geometrycore.lib	copy gmE128.h ..\PlugIns\Include\gmE128.h	copy gmpoint.h ..\PlugIns\Include\gmpoint.h	copy gmDoubleList.h ..\PlugIns\Include\gmDoubleList.h	copy GeometryDef.h ..\PlugIns\Include\GeometryDef.h	copy gmE126.h ..\PlugIns\Include\gmE126.h
# End Special Build Tool

!ELSEIF  "$(CFG)" == "geometrycore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"pch.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\core" /I "..\ogl" /I "..\igesserialize" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"pch.h" /FD /GZ /c
# ADD BASE RSC /l 0x41f /d "_DEBUG"
# ADD RSC /l 0x41f /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\geometrycore.lib ..\PlugIns\Library\debug\geometrycore.lib	copy gmE128.h ..\PlugIns\Include\gmE128.h	copy gmpoint.h ..\PlugIns\Include\gmpoint.h	copy gmDoubleList.h ..\PlugIns\Include\gmDoubleList.h	copy GeometryDef.h ..\PlugIns\Include\GeometryDef.h	copy gmE126.h ..\PlugIns\Include\gmE126.h
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "geometrycore - Win32 Release"
# Name "geometrycore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddTrimLoop2SurfPointsPack.cpp
# End Source File
# Begin Source File

SOURCE=.\AllTrimLoopUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicGeoIntr2dUI.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicGeoIntrUI.cpp
# End Source File
# Begin Source File

SOURCE=.\BSplineKnots.cpp
# End Source File
# Begin Source File

SOURCE=.\BSplineSurface2QuadSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcMeshByDelaunay2D.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\DistanceSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\DoubleVec.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicPosCubes2d.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeCurveSet.cpp
# End Source File
# Begin Source File

SOURCE=.\gmBoundedSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\gmBSplineCurve2Points.cpp
# End Source File
# Begin Source File

SOURCE=.\gmBSplineSurface2Points.cpp
# End Source File
# Begin Source File

SOURCE=.\gmDoubleList.cpp
# End Source File
# Begin Source File

SOURCE=.\gmE126.cpp
# End Source File
# Begin Source File

SOURCE=.\gmE128.cpp
# End Source File
# Begin Source File

SOURCE=.\gmGeneralIgesInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\gmNurbTesselator.cpp
# End Source File
# Begin Source File

SOURCE=.\gmQuad2d.cpp
# End Source File
# Begin Source File

SOURCE=.\gmQuadSet.cpp
# End Source File
# Begin Source File

SOURCE=.\gmQuadSetUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\gmQuadTrimmer.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTriangleUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTrimLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTrimLoopUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurface2TriangleMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurfaceSet.cpp
# End Source File
# Begin Source File

SOURCE=.\gmVectorUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\gmVoronoi.cpp
# End Source File
# Begin Source File

SOURCE=.\LongList.cpp
# End Source File
# Begin Source File

SOURCE=.\MgcDelaunay2D.cpp
# End Source File
# Begin Source File

SOURCE=.\NearestNeighbors.cpp
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsAngleSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsPointCollector.cpp
# End Source File
# Begin Source File

SOURCE=.\NeighborsData.cpp
# End Source File
# Begin Source File

SOURCE=.\NurbsSurfaceErrorChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\NurbsTessSpecialCases.cpp
# End Source File
# Begin Source File

SOURCE=.\PeriodicOpenBSplineCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSet2LineSet.cpp
# End Source File
# Begin Source File

SOURCE=.\PolarPoint2d.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubes.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubes2d.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubes2dCore.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubes2dGeoHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubesCore.cpp
# End Source File
# Begin Source File

SOURCE=.\PosCubesGeoHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadSetHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\RoughPolarP2d.cpp
# End Source File
# Begin Source File

SOURCE=.\RoughPolarPSet2d.cpp
# End Source File
# Begin Source File

SOURCE=.\SetMerger.cpp
# End Source File
# Begin Source File

SOURCE=.\SolidPosCube2d.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"pch.h"
# End Source File
# Begin Source File

SOURCE=.\SurfacePointSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfPointsPack.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfPointsPack2TriangleMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Tri2TriCollCheck2d.cpp
# End Source File
# Begin Source File

SOURCE=.\TriangleMeshCollCheck2d.cpp
# End Source File
# Begin Source File

SOURCE=.\TriangleMeshSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\TriangleSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\UntrimmedSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\UntrimmedSurfaceSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddTrimLoop2SurfPointsPack.h
# End Source File
# Begin Source File

SOURCE=.\AllTrimLoopUtilities.h
# End Source File
# Begin Source File

SOURCE=.\AngleSupport.h
# End Source File
# Begin Source File

SOURCE=.\BasicGeoIntr2dUI.h
# End Source File
# Begin Source File

SOURCE=.\BasicGeoIntrUI.h
# End Source File
# Begin Source File

SOURCE=.\BasicLineFunctions.h
# End Source File
# Begin Source File

SOURCE=.\BasicLineFunctions2d.h
# End Source File
# Begin Source File

SOURCE=.\BSplineKnots.h
# End Source File
# Begin Source File

SOURCE=.\BSplineM.h
# End Source File
# Begin Source File

SOURCE=.\BSplineM3.h
# End Source File
# Begin Source File

SOURCE=.\BSplineM4.h
# End Source File
# Begin Source File

SOURCE=.\BSplineSurface2QuadSet.h
# End Source File
# Begin Source File

SOURCE=.\CalcMeshByDelaunay2D.h
# End Source File
# Begin Source File

SOURCE=.\CurveSplit.h
# End Source File
# Begin Source File

SOURCE=.\DistanceSupport.h
# End Source File
# Begin Source File

SOURCE=.\DoubleVec.h
# End Source File
# Begin Source File

SOURCE=.\DynamicPosCubes2d.h
# End Source File
# Begin Source File

SOURCE=.\FreeCurveSet.h
# End Source File
# Begin Source File

SOURCE=.\GeometryDef.h
# End Source File
# Begin Source File

SOURCE=.\gmBoundedSurface.h
# End Source File
# Begin Source File

SOURCE=.\gmBSplineCurve2Points.h
# End Source File
# Begin Source File

SOURCE=.\gmBSplineSurface2Points.h
# End Source File
# Begin Source File

SOURCE=.\gmDoubleList.h
# End Source File
# Begin Source File

SOURCE=.\gmE126.h
# End Source File
# Begin Source File

SOURCE=.\gmE128.h
# End Source File
# Begin Source File

SOURCE=.\gmGeneralIgesInfo.h
# End Source File
# Begin Source File

SOURCE=.\gmLine.h
# End Source File
# Begin Source File

SOURCE=.\gmLineSet.h
# End Source File
# Begin Source File

SOURCE=.\gmNurbsTesselator.h
# End Source File
# Begin Source File

SOURCE=.\gmPoint.h
# End Source File
# Begin Source File

SOURCE=.\gmPointSet.h
# End Source File
# Begin Source File

SOURCE=.\gmQuad2d.h
# End Source File
# Begin Source File

SOURCE=.\gmQuadSet.h
# End Source File
# Begin Source File

SOURCE=.\gmQuadSetUtility.h
# End Source File
# Begin Source File

SOURCE=.\gmQuadTrimmer.h
# End Source File
# Begin Source File

SOURCE=.\gmTriangle.h
# End Source File
# Begin Source File

SOURCE=.\gmTriangleMesh.h
# End Source File
# Begin Source File

SOURCE=.\gmTriangleUtility.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimLoop.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimLoopUtility.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurface.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurface2TriangleMesh.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurfaceData.h
# End Source File
# Begin Source File

SOURCE=.\gmTrimmedSurfaceSet.h
# End Source File
# Begin Source File

SOURCE=.\gmVectorUtility.h
# End Source File
# Begin Source File

SOURCE=.\gmVoronoi.h
# End Source File
# Begin Source File

SOURCE=.\InfiniteLinesNearestPoints.h
# End Source File
# Begin Source File

SOURCE=.\LineIntersection.h
# End Source File
# Begin Source File

SOURCE=.\LineIntersection2d.h
# End Source File
# Begin Source File

SOURCE=.\LongList.h
# End Source File
# Begin Source File

SOURCE=.\MgcDelaunay2D.h
# End Source File
# Begin Source File

SOURCE=.\MgcDelaunay2D.inl
# End Source File
# Begin Source File

SOURCE=.\NearestNeighbors.h
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsAngleSupport.h
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsLoop.h
# End Source File
# Begin Source File

SOURCE=.\NearestNeighborsPointCollector.h
# End Source File
# Begin Source File

SOURCE=.\NeighborsData.h
# End Source File
# Begin Source File

SOURCE=.\NurbsSurfaceErrorChecker.h
# End Source File
# Begin Source File

SOURCE=.\NurbsTessSpecialCases.h
# End Source File
# Begin Source File

SOURCE=.\PeriodicOpenBSplineCurve.h
# End Source File
# Begin Source File

SOURCE=.\PointSet2LineSet.h
# End Source File
# Begin Source File

SOURCE=.\PolarCalc2d.h
# End Source File
# Begin Source File

SOURCE=.\PolarPoint2d.h
# End Source File
# Begin Source File

SOURCE=.\PosCubes.h
# End Source File
# Begin Source File

SOURCE=.\PosCubes2d.h
# End Source File
# Begin Source File

SOURCE=.\PosCubes2dCore.h
# End Source File
# Begin Source File

SOURCE=.\PosCubes2dGeoHandling.h
# End Source File
# Begin Source File

SOURCE=.\PosCubesCore.h
# End Source File
# Begin Source File

SOURCE=.\PosCubesGeoHandling.h
# End Source File
# Begin Source File

SOURCE=.\QuadSetHolder.h
# End Source File
# Begin Source File

SOURCE=.\RoughPolarP2d.h
# End Source File
# Begin Source File

SOURCE=.\RoughPolarPSet2d.h
# End Source File
# Begin Source File

SOURCE=.\SetMerger.h
# End Source File
# Begin Source File

SOURCE=.\SolidPosCube2d.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SurfacePoint.h
# End Source File
# Begin Source File

SOURCE=.\SurfacePointSet.h
# End Source File
# Begin Source File

SOURCE=.\SurfPointsPack.h
# End Source File
# Begin Source File

SOURCE=.\SurfPointsPack2TriangleMesh.h
# End Source File
# Begin Source File

SOURCE=.\Tri2TriCollCheck2d.h
# End Source File
# Begin Source File

SOURCE=.\TriangleMeshCollCheck2d.h
# End Source File
# Begin Source File

SOURCE=.\TriangleMeshSplitter.h
# End Source File
# Begin Source File

SOURCE=.\TriangleSplitter.h
# End Source File
# Begin Source File

SOURCE=.\UntrimmedSurface.h
# End Source File
# Begin Source File

SOURCE=.\UntrimmedSurface2QuadSet.h
# End Source File
# Begin Source File

SOURCE=.\UntrimmedSurfaceSet.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\gmR
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
