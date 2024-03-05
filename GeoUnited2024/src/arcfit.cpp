
#include "stdafx.h"
#include "ArcFit.h"
#include "VLine.h"
#include "VArc.h"
#include <float.h>
#include "GaussElemination.h"
#include "Matrix.h"

#include "CoordinateSystem.h"
#include "VPlane.h"
//#include "SVD.h"


#define DBL_EQ(a, b)  (fabs(a - b) < FLT_EPSILON)


ArcFit::ArcFit()
{}

int ArcFit::FitTr(const std::vector<VPoint> &Points , vreal tol , std::vector<LineOrArc> &result , const CoordinateSystem &cs)
{
	// if cs ZDir is UZ do not transform
	double dp = fabs (cs.GetDirz() % UZ);
	if (fabs(1.-dp)<EPSILON)
	{
		return Fit (Points,tol,result);
	}

	// Transform all points to XY plane
	size_t mysize = Points.size();
	std::vector<VPoint>trPoints (mysize);
	for (size_t i=0;i<mysize;++i)
	{
		const VPoint &cp = Points[i];
		trPoints[i] = cs.AbsoluteCoordToRelativeCoord(cp);
	}
	std::vector<LineOrArc> tmpres;
	int ckres = Fit (trPoints , tol , tmpres);
	
	// Back transform result
	LineOrArc::TransformBackFromLocalCsToAbs (tmpres  , cs , result);
	return ckres;
}

// error -1
// successfull 0

int ArcFit::Fit(const std::vector<VPoint> &originalPoints, vreal tol, std::vector<LineOrArc> &result)
{
	std::vector<VPoint>Points;
	try
	{
		// remove coincident sequential points
		int noriginal = int(originalPoints.size());
		if (noriginal == 0)
			return -1;//throw VException (("ArcFit::Fit exception , original point vector is empty!"),1);
		Points.reserve (noriginal);
		Points.push_back (originalPoints[0]);
		double coinTol = tol * tol * 0.01;
		for (int k=1;k<noriginal;++k)
		{
			const VPoint &curp = originalPoints[k];
			if (curp.Compare (originalPoints[k-1] , coinTol) == false)
				Points.push_back (curp);
		}
		int n = (int)(Points.size());

		if(n < 2)
			return -1;//throw VException (("ArcFit::Fit exception , Points is empty!"),1);

		if(n == 2)	//definitely a line
		{
			VLine l0(Points[0], Points[1]);
			LineOrArc loa0(l0);

			result.push_back(loa0);

			return 0;
		}
		//start and end points for the current arc
		int i0 = 0;
		int i1 = n-1;
		bool done = false;
		// try a quick check with all the points 
		LineOrArc la000;
		bool ckfit000 = ArcFit::FitLineOrArc(Points, la000, tol, i0, i1);
		if (ckfit000)
		{
			result.push_back (la000);
			done = true;
		}
		//start and end points for the current arc
		i0 = 0;
		i1 = 2;

		LineOrArc lastGoodArc;

		while(!done)
		{
			while(true)
			{
				//fit line or arc through i0 and i1
				LineOrArc la;
				bool ckfit = ArcFit::FitLineOrArc(Points, la, tol, i0, i1);
				
				//vreal err = ArcFit::CalcErr(Points, i0, i1, la);

				//if(err <= tol)	//add more points to the current line or arc
				if (ckfit == true)
				{
					lastGoodArc = la;

					i1++;
					if(i1 >= n)
					{
						//add this last good line or arc as the final line or arc
						result.push_back(lastGoodArc);

						done = true;
						break;
					}
				}
				else
				{
					//save last good line or arc and move to the next line or arc
					result.push_back(lastGoodArc);

					//rollback the last point
					i1--;

					break;
				}
			}

			if(!done)
			{
				//update i0 and i1 for the next line or arc
				i0 = i1;
				i1 = i0 + 2;

				if(i1 >= n)	//no points for the next line or arc, but there can be 1 point left
				{
					VLine ll(Points[i0], Points[i0 + 1]);
					LineOrArc la(ll);
					result.push_back(la);

					done = true;
				}
			}
		}
		//CleanCorruptedArc (result);
		ReplaceArcsWithLinesIf(result, tol);

		return 0;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc));
		LogFileWriter::WriteExceptionLog(e);
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc));
		LogFileWriter::WriteExceptionLog (e);
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitUnkExc));
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
}

int ArcFit::FitRecursive (const std::vector<VPoint> &originalPoints , vreal tol , std::vector<LineOrArc> &result)
{
	std::vector<VPoint> Points;
	try
	{
		// remove coincident sequential points
		int noriginal = int(originalPoints.size());
		if (noriginal == 0)
			return -1;//throw VException (("ArcFit::Fit exception , original point vector is empty!"),1);
		Points.reserve (noriginal);
		Points.push_back (originalPoints[0]);
		double coinTol = tol * tol * 0.01;
		for (int k=1;k<noriginal;++k)
		{
			const VPoint &curp = originalPoints[k];
			if (curp.Compare (originalPoints[k-1] , coinTol) == false)
				Points.push_back (curp);
		}
		int n = (int)(Points.size());

		if(n < 2)
			return -1;//throw VException (("ArcFit::Fit exception , Points is empty!"),1);

		std::vector<LineOrArc> temp;
		std::map<int,int> sortHelper;
		int i0 = 0;
		int i1 = (int)Points.size()-1;
		FitLineOrArcRecursive (Points,tol,temp,i0,i1,sortHelper);
		std::map<int,int>::iterator ii = sortHelper.begin();
		std::map<int,int>::iterator ee = sortHelper.end();
		for (ii;ii!=ee;++ii)
		{
			result.push_back (temp.at(ii->second));
		}
		ReplaceArcsWithLinesIf(result, tol);
		return 0;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc1));
		LogFileWriter::WriteExceptionLog(e);
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc1));
		LogFileWriter::WriteExceptionLog (e);
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitUnkExc1));
		result.clear();
		return HandleArcFitFitException (Points , result);
	}
}
bool ArcFit::FitLineOrArcRecursive (const std::vector<VPoint> &Points , vreal tol , std::vector<LineOrArc> &results , int i0 , int i1 , std::map<int,int> &sortHelper)//first int of sortHelper is start indice of point in Points , second int is LineOrArc indice in results
{
	try
	{	
		int n = int(Points.size());

		if(i0 < 0)
			i0 = 0;

		if(i1 < 0)
			i1 = n - 1;

		//real number of points
		n = (i1 - i0) + 1;

		if(n < 2)	//this shouldn't happen anyway
			throw VException (( IDS_FitExcPoiNoIsLe),1);;

		//check if points form a straight line
		if(ArcFit::CalcLineErr(Points, i0, i1) <= tol)
		{
			//it is a line
			VLine ll(Points[i0], Points[i1]);
			results.push_back (ll);
			sortHelper.insert (std::pair<int,int>(i0 , (int)results.size()-1));
			return true;
		}

		//ia and ib are indexes of effective start and end points
		int ia = i0, ib = i1;

		//check if first and last points are coincident
		vreal dx = Points[ia][0] - Points[ib][0];
		vreal dy = Points[ia][1] - Points[ib][1];
		vreal dist = sqrt(dx*dx + dy*dy);

		if(DBL_EQ(dist, 0))
		{
			//find new start and end points (as the largest consecutive gap, since points are sorted)
			vreal maxDist2 = 0;

			for(int i = i0; i < i1; i++)
			{
				vreal dx = Points[i][0] - Points[i+1][0];
				vreal dy = Points[i][1] - Points[i+1][1];
				vreal dist2 = dx*dx + dy*dy;

				if(dist2 > maxDist2)
				{
					maxDist2 = dist2;
					ia = i;
					ib = i+1;
				}
			}

			//check if first and last points are still equal
			if(DBL_EQ(sqrt(maxDist2), 0))
			{
				//here, all points must be the same since points are given sorted
				throw VException (( IDS_ArcFitExcAllPoi), 1);
			}
		}

		//find the mid point P, while a and b are the start and end points
		vreal ax = Points[ia][0];
		vreal ay = Points[ia][1];

		vreal bx = Points[ib][0];
		vreal by = Points[ib][1];

		vreal px = (ax + bx) / 2;
		vreal py = (ay + by) / 2;

		//Q is the direction from P through the arc center
		vreal qx = by - ay;
		vreal qy = ax - bx;
		vreal rq = sqrt(qx*qx + qy*qy);
		
		if (fabs(rq) < EPSILON)
			throw VException (( IDS_ArcFitExcRq),1);

		qx /= rq;
		qy /= rq;

		/* center c = p + alpha*d */
		/* radius r = |a - c| or |b - c| */

		//initial alpha
		vreal alpha = 0;

		//center and radius of the arc
		vreal cx = 0, cy = 0;
		vreal r = 0;

		int maxIter = 1500;
		int nIter = 0;
		//Gauss-newton iterations for alpha
		while(true)
		{
			//calculate center and radius for the current alpha
			cx = px + alpha * qx;
			cy = py + alpha * qy;

			vreal rx = ax - cx;
			vreal ry = ay - cy;

			r = sqrt(rx*rx + ry*ry);

			//derivative of r wrt alpha
			vreal dra = (-rx*qx - ry*qy) / r;

			vreal JtF = 0;
			vreal JtJ = 0;

			//compute JtF and JtJ
			for(int i = i0; i <= i1; i++)
			{
				if(i == ia || i == ib)
					continue;

				vreal rix = Points[i][0] - cx;
				vreal riy = Points[i][1] - cy;

				vreal ri = sqrt(rix*rix + riy*riy);
				
				//this really shouldn't happen because if q != 0 then r cannot be either
				if (fabs(ri) < EPSILON)
					throw VException (( IDS_ArcFitExcRi),1);

				vreal fi = r - ri;
				vreal ji = (-rix*qx - riy*qy) / ri - dra;

				JtF += (ji*fi);
				JtJ += (ji*ji);
			}

			//update alpha
			if (JtJ == 0)
				break;

			vreal h = JtF / JtJ;
			alpha += h;

			if((h*h) <= 1e-10)
				break;

			++nIter;
			if (nIter > maxIter)
				throw VException (( IDS_ArcFitExcNItMax),1);
		}

		//form the resulting arc
		VPoint center(cx, cy, Points[i0][2]);

		VArc arc;
		bool ckArcMake = MakeArc(Points, i0, i1, center, r, arc);
	
		if ( ckArcMake && CalcArcErr (Points , i0 , i1 , arc) < tol )
		{
			results.push_back(arc);
			sortHelper.insert (std::pair<int,int>(i0,(int)results.size()-1));
			return true;
		}
		else
		{
			int imid = (int)((i0+i1)*0.5);
			FitLineOrArcRecursive (Points , tol , results , i0 , imid , sortHelper);
			FitLineOrArcRecursive (Points , tol , results , imid , i1 , sortHelper);
			return true;
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc2));
		LogFileWriter::WriteExceptionLog (e);
		int imid = (int)((i0+i1)*0.5);
		FitLineOrArcRecursive (Points , tol , results , i0 , imid , sortHelper);
		FitLineOrArcRecursive (Points , tol , results , imid , i1 , sortHelper);
		return true;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc2));
		LogFileWriter::WriteExceptionLog (e);
		int imid = (int)((i0+i1)*0.5);
		FitLineOrArcRecursive (Points , tol , results , i0 , imid , sortHelper);
		FitLineOrArcRecursive (Points , tol , results , imid , i1 , sortHelper);
		return true;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitUnkExc2));
		int imid = (int)((i0+i1)*0.5);
		FitLineOrArcRecursive (Points , tol , results , i0 , imid , sortHelper);
		FitLineOrArcRecursive (Points , tol , results , imid , i1 , sortHelper);
		return true;
	}
}
int ArcFit::HandleArcFitFitException (const std::vector<VPoint> &Points , std::vector<LineOrArc> &result)
{
	// Add line for all sequential points , no arcs added
	size_t mysize = Points.size();
	for (size_t i=1;i<mysize;++i)
	{
		VLine l (Points[i-1] , Points[i]);
		LineOrArc geo(l);
		result.push_back (geo);
	}
	return 0;
}

int ArcFit::IsLineOrArcTr (const std::vector<VPoint> &Points, vreal tol, LineOrArc &loa , const CoordinateSystem &cs)
{
	// if cs ZDir is UZ do not transform
	double dp = fabs (cs.GetDirz() % UZ);
	if (fabs(1.-dp)<EPSILON)
	{
		return IsLineOrArc (Points,tol,loa);
	}

	// Transform all points to XY plane
	size_t mysize = Points.size();
	std::vector<VPoint>trPoints (mysize);
	for (size_t i=0;i<mysize;++i)
	{
		const VPoint &cp = Points[i];
		trPoints[i] = cs.AbsoluteCoordToRelativeCoord(cp);
	}
	LineOrArc tmpres;
	int ckres = IsLineOrArc (trPoints , tol , tmpres);
	// Back transform result
	if (ckres != 0)
	{
		if (tmpres.GetType() == LineOrArc::line)
		{
			const VLine *l = tmpres.GetLine();
			VLine lb (cs.RelativeCoordToAbsoluteCoord(l->GetP1()) , cs.RelativeCoordToAbsoluteCoord(l->GetP2()));
			LineOrArc nla (lb);
			loa = nla;
		}
		else
		{
			const VArc *a = tmpres.GetArc();
			VArc ab (cs.RelativeCoordToAbsoluteCoord(a->GetCenter()) , a->GetRadius() , cs.RelativeVectorToAbsoluteVector(a->GetAx()),cs.RelativeCoordToAbsoluteCoord(a->GetStartPoint()) , a->GetSweepAngle());		
			LineOrArc nla (ab);
			loa = nla;
		}
	}
	return ckres;	
}

// Arc returns 2
// Line returns 1
// None 0

int ArcFit::IsLineOrArc(const std::vector<VPoint> &Points, vreal tol, LineOrArc &loa)
{
	int n = int(Points.size());

	if(n < 2)
		return 0;

	//check if points form a straight line
	if(ArcFit::CalcLineErr(Points, 0, n-1) <= tol)
	{
		//it is a line
		VLine ll(Points[0], Points[n-1]);
		loa.SetLine(ll);

		return 1;	//is line
	}

	if(n == 2)	//cannot be an arc
		return 0;

	//initial arc parameters
	vreal a = 0, b = 0, r = 0;

	for(int i = 0; i < n; i++)
	{
		a += Points[i][0];
		b += Points[i][1];
	}
	a /= n;
	b /= n;

	for(int i = 0; i < n; i++)
	{
		vreal dx = Points[i][0] - a;
		vreal dy = Points[i][1] - b;
		
		r += sqrt(dx*dx + dy*dy);
	}
	r /= n;

	//create the initial arc
	//VArc arc0(VPoint(a, b, Points[0][2]), r);

	//vreal err = CalcArcErr(Points, 0, n-1, arc0);
	//if(err <= tol)
	//{
	//	loa.SetArc(arc0);
	//	return 2;	//is arc
	//}

	// Refine the result anyway ! Even it conforms to tolerance

	//refine the arc, if necessary
	vreal JtF[3];
	vreal JtJ[9];

	GaussElemination GE;	//Linear system solver

	int maxIter = 200;
	int iter = 0;

	while(true)
	{
		JtF[0] = JtF[1] = JtF[2] = 0;
		memset(JtJ, 0, sizeof(vreal)*9);

		for(int i = 0; i < n; i++)
		{
			vreal dx = a - Points[i][0];
			vreal dy = b - Points[i][1];
			vreal ri = sqrt(dx*dx + dy*dy);

			if(DBL_EQ(ri, 0))
			{
				//degenerate points, must be non arc
				return 0;
			}

			vreal Ji0 = dx / ri;
			vreal Ji1 = dy / ri;
			vreal Ji2 = -1;

			vreal Fi = r - ri; //-F

			JtF[0] += Ji0*Fi;
			JtF[1] += Ji1*Fi;
			JtF[2] += Ji2*Fi;

			JtJ[0] += (Ji0*Ji0);
			JtJ[1] += (Ji0*Ji1);
			JtJ[2] += (Ji0*Ji2);
			//JtJ[3] += (Ji1*Ji0);
			JtJ[4] += (Ji1*Ji1);
			JtJ[5] += (Ji1*Ji2);
			//JtJ[6] += (Ji0*Ji0);
			//JtJ[7] += (Ji0*Ji0);
			JtJ[8] += (Ji2*Ji2);
		}

		//since JtJ is symmetric
		JtJ[3] = JtJ[1];
		JtJ[6] = JtJ[2];
		JtJ[7] = JtJ[5];

		Matrix MatJtJ(3, 3, JtJ);
		Matrix MatJtF(3, 1, JtF);

		Matrix MatH = GE.Solve(MatJtJ, MatJtF);
		vreal *H = MatH.GetMatrixArr();

		if((H[0]*H[0] + H[1]*H[1] + H[2]*H[2]) < 1e-10)
			break;

		a += H[0];
		b += H[1];
		r += H[2];

		iter++;
		if(iter > maxIter)
			return 0;	//cannot converger, must be non-arc
	}

	//form the resulting arc
	VPoint center(a, b, Points[0][2]);
	VArc arc;
	MakeArc(Points, 0, n-1, center, r, arc);

	if (Points[0].Compare(Points[n-1],1.e-4))
	{
		if (fabs(arc.GetSweepAngle() - 360.0) > EPSILON)
		{
			// Arc should be a full arc , but it is not so force the result to a full arc
			VArc newarc (arc.GetCenter(),arc.GetRadius(),arc.GetAx(),arc.GetStartPoint(),360.0);
			arc = newarc;
		}
	}

	loa.SetArc(arc);

	//check the arc error
	vreal err = CalcArcErr(Points, 0, n-1, arc);
	if(err <= tol)
	{
		return 2;	//is arc
	}

	return 0;	//neither line nor arc
}

/* STATIC HELPER FUNCTIONS */

//computes the up vector and forms the arc
bool ArcFit::MakeArc(const std::vector<VPoint> &Points, int i0, int i1, VPoint &center, vreal radius, VArc &arc)
{
	VPoint rStart = Points[i0] - center;
	rStart.NormThis();
	//for(int i = i1; i > i0; i--)
	for (int i=i0+1;i<i1+1;++i)
	{
		VPoint rEnd = Points[i] - center;
		rEnd.NormThis();
		VPoint up = rStart ^ rEnd;

		if(up.GetLength2() >= 1.e-10)
		{
			up.NormThis();
			//if (UZ%up<EPSILON)
			//	up*=-1.0;
			VArc arc0(center, radius, up, Points[i0], Points[i1]);
			if ( UZ % up < EPSILON)
				arc0.ReverseAxis ();

			arc = arc0;

			return true;
		}
	}

	return false;
	//if we reach this point, up vector is not computed
	//throw VException (("ArcFit::MakeArc exception , cannot compute up vector!"), 1);
}

//line or arc passes through start and end points (i0 and i1)
bool ArcFit::FitLineOrArc(const std::vector<VPoint> &Points, LineOrArc &result, vreal tol, int i0, int i1)
{
	try
	{
		int n = int(Points.size());

		if(i0 < 0)
			i0 = 0;

		if(i1 < 0)
			i1 = n - 1;

		//real number of points
		n = i1 - i0 + 1;

		if(n < 3)	//this shouldn't happen anyway
			throw VException (( IDS_ArcFitExcN),1);

		//check if points form a straight line
		if(ArcFit::CalcLineErr(Points, i0, i1) <= tol)
		{
			//it is a line
			VLine ll(Points[i0], Points[i1]);
			result.SetLine(ll);
			return true;
		}

		//ia and ib are indexes of effective start and end points
		int ia = i0, ib = i1;

		//check if first and last points are coincident
		vreal dx = Points[ia][0] - Points[ib][0];
		vreal dy = Points[ia][1] - Points[ib][1];
		vreal dist = sqrt(dx*dx + dy*dy);

		if(DBL_EQ(dist, 0))
		{
			//find new start and end points (as the largest consecutive gap, since points are sorted)
			vreal maxDist2 = 0;

			for(int i = i0; i < i1; i++)
			{
				vreal dx = Points[i][0] - Points[i+1][0];
				vreal dy = Points[i][1] - Points[i+1][1];
				vreal dist2 = dx*dx + dy*dy;

				if(dist2 > maxDist2)
				{
					maxDist2 = dist2;
					ia = i;
					ib = i+1;
				}
			}

			//check if first and last points are still equal
			if(DBL_EQ(sqrt(maxDist2), 0))
			{
				//here, all points must be the same since points are given sorted
				throw VException (( IDS_ArcFitExcAllPoi1), 1);
			}
		}

		//find the mid point P, while a and b are the start and end points
		vreal ax = Points[ia][0];
		vreal ay = Points[ia][1];

		vreal bx = Points[ib][0];
		vreal by = Points[ib][1];

		vreal px = (ax + bx) / 2;
		vreal py = (ay + by) / 2;

		//Q is the direction from P through the arc center
		vreal qx = by - ay;
		vreal qy = ax - bx;
		vreal rq = sqrt(qx*qx + qy*qy);
		
		if (fabs(rq) < EPSILON)
			throw VException (( IDS_ArcFitExcRq1),1);

		qx /= rq;
		qy /= rq;

		/* center c = p + alpha*d */
		/* radius r = |a - c| or |b - c| */

		//initial alpha
		vreal alpha = 0;

		//center and radius of the arc
		vreal cx = 0, cy = 0;
		vreal r = 0;

		int maxIter = 500;
		int nIter = 0;
		//Gauss-newton iterations for alpha
		while(true)
		{
			//calculate center and radius for the current alpha
			cx = px + alpha * qx;
			cy = py + alpha * qy;

			vreal rx = ax - cx;
			vreal ry = ay - cy;

			r = sqrt(rx*rx + ry*ry);

			//derivative of r wrt alpha
			vreal dra = (-rx*qx - ry*qy) / r;

			vreal JtF = 0;
			vreal JtJ = 0;

			//compute JtF and JtJ
			for(int i = i0; i <= i1; i++)
			{
				if(i == ia || i == ib)
					continue;

				vreal rix = Points[i][0] - cx;
				vreal riy = Points[i][1] - cy;

				vreal ri = sqrt(rix*rix + riy*riy);
				
				//this really shouldn't happen because if q != 0 then r cannot be either
				if (fabs(ri) < EPSILON)
					throw VException (( IDS_ArcFitExcRi1),1);

				vreal fi = r - ri;
				vreal ji = (-rix*qx - riy*qy) / ri - dra;

				JtF += (ji*fi);
				JtJ += (ji*ji);
			}

			//update alpha
			if (JtJ == 0)
				break;

			vreal h = JtF / JtJ;
			alpha += h;

			if((h*h) <= 1e-10)
				break;

			++nIter;
			if (nIter > maxIter)
				throw VException (( IDS_ArcFitExcNItMax1),1);
		}

		//form the resulting arc
		VPoint center(cx, cy, Points[i0][2]);

		VArc arc;
		bool ckArcMake = MakeArc(Points, i0, i1, center, r, arc);
		result.SetArc(arc);
		if (!ckArcMake)
			return false;
		if (CalcArcErr (Points , i0 , i1 , arc) < tol)
			return true;
		else
			return false;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc3));
		LogFileWriter::WriteExceptionLog (e);
		//HandleFitLineOrArcException (Points , i0 , i1 , result);
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitExc3));
		LogFileWriter::WriteExceptionLog (e);
		//HandleFitLineOrArcException (Points , i0 , i1 , result);
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcFitUnkExc3));
		//HandleFitLineOrArcException (Points , i0 , i1 , result);
		return false;
	}
}

void ArcFit::HandleFitLineOrArcException (const std::vector<VPoint> &Points , int i0 , int i1 , LineOrArc &result)
{
	if (i1 - i0 < 2)
	{
		VLine l (Points.at(i0) , Points.at(i1));
		result.SetLine (l);
	}
	else
	{
		double imidreal = (i1 - i0) * 0.5;
		int imid = (int)imidreal;
		VArc a (Points.at(i0) , Points.at(imid) , Points.at(i1));
		if (a.GetAx() % UZ < 0.0)
			a.ReverseAxis();
		result.SetArc (a);
	}
}
//error is the maximum perpendicular distance of all points
vreal ArcFit::CalcLineErr(const std::vector<VPoint> &Points, int i0, int i1)
{
	vreal err = 0;

	//fit line to the first and last points
	vreal a, b, c;

	if(DBL_EQ(Points[i0][0], Points[i1][0]))
	{
		a = 1;
		b = 0;
		c = -(Points[i0][0] + Points[i1][0]) / 2;
	}
	else
	{
		a = Points[i1][1] - Points[i0][1];
		b = Points[i0][0] - Points[i1][0];
		c = -Points[i0][0]*a - Points[i0][1]*b;

		//normalize so that a*a + b*b = 1
		vreal l = sqrt(a*a + b*b);

		if (fabs(l) < EPSILON)
				throw VException (( IDS_ArcCalExcL),1);

		a /= l;
		b /= l;
		c /= l;
	}

	for(int i = i0; i <= i1; i++)
	{
		vreal d = fabs(a * Points[i][0] + b * Points[i][1] + c);
		err = max(err, d);
	}

	return err;
}

//error is the maximum |ri - r| of all points
vreal ArcFit::CalcArcErr(const std::vector<VPoint> &Points, int i0, int i1, const VArc &arc)
{
	vreal err = 0;

	vreal cx = arc.GetCenter()[0];
	vreal cy = arc.GetCenter()[1];
	vreal r = arc.GetRadius();

	for(int i = i0; i <= i1; i++)
	{
		vreal dx = Points[i][0] - cx;
		vreal dy = Points[i][1] - cy;

		vreal ri = sqrt(dx*dx + dy*dy);
		
		//vreal cErr = fabs(ri - r);
		//err = max(err, cErr);
		err += fabs(ri - r);
	}

	return err / (i1 - i0 + 1);
}

//error of the line or arc based on the CalcLineErr and CalcArcErr functions
vreal ArcFit::CalcErr(const std::vector<VPoint> &Points, int i0, int i1, const LineOrArc &la)
{
	if(la.GetType() == LineOrArc::arc)
		return ArcFit::CalcArcErr(Points, i0, i1, *(la.GetArc()));
	else
		return ArcFit::CalcLineErr(Points, i0, i1);
}

void ArcFit::CloseArcIf(bool isclosed, VArc &a)
{
	if (isclosed)
	{
		VArc newarc (a.GetCenter() , a.GetRadius() , a.GetAx() , a.GetStartPoint() , 360.0);
		a = newarc;
	}
}

//vreal ArcFit::FitPlane(const std::vector<VPoint> &Points, VPlane &plane)
//{
//	int n = Points.size();
//
//	if(n < 3)
//		return -1;
//
//	// Matrix A should have at least 4 rows
//	int n2 = (n == 3 ? 4 : n);
//
//	Matrix A(n2, 4);
//	vreal *aData = A.GetMatrixArr();
//
//	// Fill A
//	for(int i = 0; i < n; i++)
//	{
//		aData[i*4 + 0] = Points[i][0];
//		aData[i*4 + 1] = Points[i][1];
//		aData[i*4 + 2] = Points[i][2];
//		aData[i*4 + 3] = 1;
//	}
//
//	if(n != n2)	// if n = 3, 4-th row can be any of the points
//	{
//		aData[3*4 + 0] = Points[0][0];
//		aData[3*4 + 1] = Points[0][1];
//		aData[3*4 + 2] = Points[0][2];
//		aData[3*4 + 3] = 1;
//	}
//
//	Matrix S(4, 1);
//	Matrix Vt(4, 4);
//
//	SVD svd(n2, 4);
//
//	int err = svd.Calc(&A, &S, 0, &Vt);
//
//	int rank = svd.GetRank();
//
//	if(rank < 3)	// not a plane, rank = 2 -> all points form a line
//	{				//              rank = 1 -> all points are the same
//		return -1;
//	}
//
//	// Plane is the last row of Vt
//	vreal *P = Vt.GetMatrixArr() + 12;
//
//	VPlane aplane(P);
//	plane = aplane;
//
//	return 0;
//}

void ArcFit::CleanCorruptedArc(std::vector<LineOrArc> &result)
{
	// Arc is assumed to be corrupted if it has too big radius and very small angle
	std::vector<LineOrArc> temp;
	size_t mysize = result.size();
	for (size_t j=0;j<mysize;++j)
	{
		try
		{
			LineOrArc &geo = result[j];
			if (geo.GetType() == LineOrArc::arc)
			{
				VPoint ep = geo.GetArc()->GetEndPoint();
			}
			temp.push_back (geo);
		}
		catch (VException &e)
		{
			LogFileWriter::WriteExceptionLog (e);
		}
		catch (std::exception &e)
		{
			LogFileWriter::WriteExceptionLog (e);
		}
		catch (...)
		{
			LogFileWriter::WriteUnknownExceptionLog (( IDS_ArcCleUnkExc));
		}
	}
	result = temp;

}

void ArcFit::ReplaceArcsWithLinesIf(std::vector<LineOrArc> &result, vreal tol)
{
	int geoSize = (int)result.size();
	for (int i = 0; i < geoSize; ++i)
	{
		LineOrArc &geo = result.at(i);
		if (geo.GetType() == LineOrArc::arc)
		{
			VLine l;
			bool ck = ReplaceArcIf(geo,tol,l);
			if (ck)
			{ 
				geo = LineOrArc(l);
			}
		}
	}
}

bool ArcFit::ReplaceArcIf(const LineOrArc &a, vreal tol, VLine &l)
{
	VPoint arcStart = a.GetStartPoint();
	VPoint arcMid = a.GetP(0.5);
	VPoint arcEnd = a.GetEndPoint();
	l = VLine(arcStart, arcEnd);
	vreal errDist = (arcMid - l.GetMidPoint()).GetLength();
	if (errDist < 10.0 * tol + EPSILON)
		return true;
	else
		return false;
}