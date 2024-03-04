#ifndef __VpPreCompileDefinitions_h__
#define __VpPreCompileDefinitions_h__

//#ifdef VP_COMPILING_WIRE_MODULE
//
//#define VP_LOAD_WIRE_MODULE 1
//#undef VP_LOAD_MILLING_MODULE
//#undef VP_LOAD_MILLING_MW
//
//#endif
//
//#ifdef VP_COMPILING_MILLING_MODULE
//
//#undef VP_LOAD_WIRE_MODULE
//#define VP_LOAD_MILLING_MODULE 1
//#undef VP_LOAD_MILLING_MW
//
//#endif
//
//#ifdef VP_COMPILING_MILLING_MW_MODULE
//
//#undef VP_LOAD_WIRE_MODULE 
//#define VP_LOAD_MILLING_MODULE  1
//#define VP_LOAD_MILLING_MW  1
//
//#endif


//#ifdef	VP_COMPILING_ALL
#define VP_LOAD_WIRE_MODULE 1
#define VP_LOAD_MILLING_MODULE  1
#define VP_LOAD_MILLING_MW  1
//#endif

#endif