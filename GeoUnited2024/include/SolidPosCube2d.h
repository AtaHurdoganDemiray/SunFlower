// SolidPosCube2d.h

#ifndef __SolidPosCube2d_h__
#define __SolidPosCube2d_h__

struct gmQuad2d;
struct VLine2d;
struct VTriangle2dE;
class PosCubes2d;

struct SolidPosCube2d
{
public:
	SolidPosCube2d ( const PosCubes2d &newcore , const long &cubeindex );
	~SolidPosCube2d ();
	
	bool IsLineRelated ( const VLine2d &myline );
	bool IsTriRelated ( const VTriangle2dE &mytri );
		

private:
	//gmQuad2d *quad;
};

#endif //__SolidPosCube2d_h__