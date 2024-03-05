// LeastSquaresPlaneFit.cpp

#include "stdafx.h"
#include "LeastSquaresPlaneFit.h"
#include "GaussJordanElimination.h"
#include "Matrix.h"
#include "VPlane.h"
#include "VArc.h"

bool LeastSquaresPlaneFit::Fit(const std::vector<VPoint> &pvec, VPlane &p , const VPoint &preferredPositiveNormalDirectionIf) // returns false if calculation fails , otherwise p is filled with best fit plane
{
	try
	{
		int psize = (int)pvec.size();
		vreal sigmaX(0.0), sigmaY(0.0), sigmaZ(0.0), sigmaX2(0.0), sigmaY2(0.0), sigmaZX(0.0), sigmaZY(0.0), sigmaXY(0.0);
		for (int pi = 0; pi < psize; ++pi)
		{
			const VPoint &p = pvec.at(pi);
			sigmaX += p.x();
			sigmaY += p.y();
			sigmaZ += p.z();
			sigmaX2 += (p.x()*p.x());
			sigmaY2 += (p.y()*p.y());
			sigmaZX += (p.z()*p.x());
			sigmaZY += (p.z()*p.y());
			sigmaXY += (p.x()*p.y());
		}

		// eq1:   A * sigmaX2   +  B * sigmaXY   + C * sigmaX  = sigmaZX
		// eq2:   A * sigmaXY   +  B * sigmaY2   + C * sigmaY  = sigmaZY
		// eq3:   A * sigmaX    +  B * sigmaY    + C   = sigmaZ

		vreal A[9] = {sigmaX2,sigmaXY,sigmaX
		,sigmaXY,sigmaY2,sigmaY
		,sigmaX,sigmaY,(vreal)psize};

		vreal C[3] = {sigmaZX , sigmaZY , sigmaZ};
		GaussJordanElimination gje;
		Matrix MA(3, 3, A);
		Matrix MC(3,1,C);
		bool ck = gje.Solve(MA, MC);
		if (!ck)
			return false;
		vreal planeA = MC.GetElement(1,1);
		vreal planeB = MC.GetElement(2, 1);
		vreal planeC = MC.GetElement(3, 1);
		VPoint nor(-planeA, -planeB, 1.0);
		vreal fac = 1. / nor.GetLength();
		nor *= fac;
		planeC *= fac;
		if (fabs((preferredPositiveNormalDirectionIf % nor) + 1) < 1.e-6)
			nor *= -1.0;
		VPoint o(nor * planeC);
		p = VPlane(o,nor);

		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"LeastSquaresPlaneFit::Fit");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ","LeastSquaresPlaneFit::Fit");
		return false;
	}
}

bool LeastSquaresPlaneFit::Fit(const std::list<const std::vector<VPoint>*> &allp, VPlane &p,const VPoint &preferredPositiveDirectionIf)
{
	try
	{
		VPoint firstp = (*allp.begin())->at(0);
			int psize = 0;
			vreal sigmaX(0.0), sigmaY(0.0), sigmaZ(0.0), sigmaX2(0.0), sigmaY2(0.0), sigmaZX(0.0), sigmaZY(0.0), sigmaXY(0.0);

			std::list<const std::vector<VPoint>*>::const_iterator li = allp.begin();
			std::list<const std::vector<VPoint>*>::const_iterator le = allp.end();
			for (li; li != le; ++li)
			{
				psize += (int)((*li)->size());
				for (size_t pi = 0; pi < (*li)->size(); ++pi)
				{
					const VPoint &p = (*li)->at(pi);
					sigmaX += p.x();
					sigmaY += p.y();
					sigmaZ += p.z();
					sigmaX2 += (p.x()*p.x());
					sigmaY2 += (p.y()*p.y());
					sigmaZX += (p.z()*p.x());
					sigmaZY += (p.z()*p.y());
					sigmaXY += (p.x()*p.y());
				}
			}

			// eq1:   A * sigmaX2   +  B * sigmaXY   + C * sigmaX  = sigmaZX
			// eq2:   A * sigmaXY   +  B * sigmaY2   + C * sigmaY  = sigmaZY
			// eq3:   A * sigmaX    +  B * sigmaY    + C   = sigmaZ

			vreal A[9] = { sigmaX2, sigmaXY, sigmaX
				, sigmaXY, sigmaY2, sigmaY
				, sigmaX, sigmaY, (vreal)psize };

			vreal C[3] = { sigmaZX, sigmaZY, sigmaZ };
			GaussJordanElimination gje;
			Matrix MA(3, 3, A);
			Matrix MC(3, 1, C);
			bool ck = gje.Solve(MA, MC);
			if (!ck)
				return false;
			vreal planeA = MC.GetElement(1, 1);
			vreal planeB = MC.GetElement(2, 1);
			vreal planeC = MC.GetElement(3, 1);
			VPoint nor(-planeA, -planeB, 1.0);
			vreal fac = 1. / nor.GetLength();
			nor *= fac;
			//planeC *= fac;
			VPoint o(0.0,0.0,planeC);
			if (fabs((preferredPositiveDirectionIf % nor) + 1) < 1.e-6)
				nor *= -1.0;
			p = VPlane(o, nor);
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "LeastSquaresPlaneFit::Fit");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "LeastSquaresPlaneFit::Fit");
		return false;
	}
}
bool SortCompareIsLessThanVPoint(const VPoint &p1Left, const VPoint &p2Right)
{
	vreal distToOrigin1 = p1Left.GetLength2();
	vreal distToOrigin2 = p2Right.GetLength2();
	return distToOrigin1 < distToOrigin2 ? true : false;
}
bool LeastSquaresPlaneFit::FitSearchAClosePlaneInitially(const std::list<const std::vector<VPoint>*> &allp, CoordinateSystem &solCs , vreal &maxError , const CoordinateSystem &preferedPositiveDirectionsReferenceIf)// returns true if a plane is found successfully and max error is filled
{
	try
	{
		// find any 3 unlinear points , and make circle for a plane
		std::vector<VPoint> allp2;
		std::list<const std::vector<VPoint>*>::const_iterator ii = allp.begin();
		std::list<const std::vector<VPoint>*>::const_iterator ee = allp.end();
		for (ii; ii != ee; ++ii)
		{
			for (size_t iii = 0; iii < (*ii)->size(); ++iii)
			{
				allp2.push_back((*ii)->at(iii));
			}
		}
		std::sort(allp2.begin(), allp2.end(), SortCompareIsLessThanVPoint);
		size_t psize = allp2.size();
		if (psize < 3)
			return false;
		int pp1(0), pp2((int)(allp2.size()*0.5)), pp3((int)allp2.size()-1);
		std::map<int,int>usedp;
		while (GetUnusedPoint(allp2,usedp,pp1,pp2,pp3))
		{
			const VPoint &p1(allp2.at(pp1-1));
			const VPoint &p2(allp2.at(pp2-1));
			const VPoint &p3(allp2.at(pp3+1));
			try
			{
				VArc a(p1, p2, p3);
				VPoint dirz = a.GetAx();
				if (fabs(preferedPositiveDirectionsReferenceIf.GetDirz() % dirz + 1.0) < 1.e-6)
					dirz *= -1.0;
				solCs = CoordinateSystem::ArbitraryAxisAlg(dirz);
				maxError = GetMaxError(allp2, solCs);
				return true;
			}
			catch (...)
			{
				continue;
			}
		};
		return false;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"LeastSquaresPlaneFit::FitSearchAClosePlaneInitiaally");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "LeastSquaresPlaneFit::FitSearchAClosePlaneInitiaally");
		return false;
	}
}

bool LeastSquaresPlaneFit::GetUnusedPoint(const std::vector<VPoint> &allp, std::map<int,int> &usedp, int &pp1, int &pp2, int &pp3)
{
	bool pp1set(false), pp2set(false), pp3set(false);
	std::map<int, int>::iterator fi1 = usedp.find(pp1);
	while (true)
	{
		if (fi1 == usedp.end())
		{
			pp1set = true;
			++pp1;
			break;
		}
		else
		{
			++pp1;
			if (pp1 >= allp.size())
				return false;
			fi1 = usedp.find(pp1);
		}
	}
	std::map<int, int>::iterator fi2 = usedp.find(pp2);
	while (true)
	{
		if (fi2 == usedp.end())
		{
			pp2set = true;
			++pp2;
			break;
		}
		else
		{
			++pp2;
			if (pp2 >= allp.size())
				return false;
			fi2 = usedp.find(pp2);
		}
	}
	std::map<int, int>::iterator fi3 = usedp.find(pp3);
	while (true)
	{
		if (fi3 == usedp.end())
		{
			pp3set = true;
			--pp3;
			break;
		}
		else
		{
			--pp3;
			if (pp3 < 0)
				return false;
			fi3 = usedp.find(pp3);
		}
	}
	return true;
}

vreal LeastSquaresPlaneFit::GetMaxError(const std::vector<VPoint> &allp2, const CoordinateSystem &cs)
{
	vreal maxDist = MIN_NUMBER_LIMIT;
	int mysize = (int)allp2.size();
	for (int i = 0; i < mysize; ++i)
	{
		const VPoint &p = allp2.at(i);
		VPoint deltaVec = cs.GetOrigin() - p;
		vreal curDist = deltaVec % cs.GetDirz();
		if (curDist > maxDist)
			maxDist = curDist;
	}
	return maxDist;
}
