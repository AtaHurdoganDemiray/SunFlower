// TrimmedSurfaceDigitiserByLength.h

#ifndef __TrimmedSurfaceDigitiserByLength_h__
#define __TrimmedSurfaceDigitiserByLength_h__

class dSurface;
template <class T , int N> struct sVector;
class UntrimmedSurface;
struct VPoint2d;
struct VPoint;

class TrimmedSurfaceDigitiserByLength
{
public:
	void Digitise (const vreal &step 
					, dSurface &surf
					, std::vector<sVector<float , 3>> &pvec
					, std::vector<sVector<float , 3>> &nvec 
						,std::vector<float> &avec);
	void Digitise (const vreal &step
					, dSurface &surf 
					, std::vector<VPoint2d> &pvec
					, std::vector<VPoint> &mvec
					, std::vector<VPoint> &nvec);
private:
	int GetIndex (int uno , int vno
				   , int u , int v);
	void DoVLoop (int vcount 
			   , const vreal &up
			   , vreal &vp
			   , const vreal &vstep
			   , const UntrimmedSurface &s
			   , std::vector<sVector<float,3>> &tmpvec
				   );
	void CalcNormalAndPointAndArea (int u
									,int v
									,int ucount
									,int vcount
									,int neighborUOfs
									,int neighborVOfs
									,std::vector<sVector<float,3>> &tmpvec
									,sVector<float,3> &mp
									,sVector<float,3> &n
									,float &a
									);

	void DoVLoop2
					(int u
					 ,int ucount
					 ,int vcount
					 ,int neighborUOfs
					 ,vreal &up
					 ,vreal &vp
					 ,const vreal &vstep 
					 ,dSurface &surf
					 ,std::vector<sVector<float,3>> &tmpvec
					 ,std::vector<sVector<float,3>> &pvec
					 ,std::vector<sVector<float,3>> &nvec
					 ,std::vector<float> &avec
					 );
	void GetUVSteps (dSurface &surf
					  , const vreal &step
					  , vreal &ustep
					  , vreal &vstep
					  , int &ucount
					  , int &vcount);
	void DoVLoop3 (int vcount 
				 , const vreal &up
				 , vreal &vp
				 , const vreal &vstep
				 , dSurface &surf
			 	 , std::vector<VPoint2d> &pvec
				 , std::vector<VPoint> &mvec
				 , std::vector<VPoint> &nvec
				 );
};

#endif