// LinearEquations.h

#ifndef __LinearEquations_h__
#define __LinearEquations_h__

#include "BasicDefinitions.h"
#include "Matrix.h"

class LinearEquations
{
public:
	static bool SolveLinear2Unknown ( const vreal & a1 , const vreal & b1 , const vreal & c1
									, const vreal & a2 , const vreal & b2 , const vreal & c2
									, vreal &x , vreal &y );
 
	static bool SolveLinear2UnknownByGauss ( const vreal & a1 , const vreal & b1 , const vreal & c1
									 , const vreal & a2 , const vreal & b2 , const vreal & c2
									, vreal &x , vreal &y );

	// Inhomogeneous Linear system Ax = b
	// Modifies A
	//static bool SolveSVD(Matrix &A, const Matrix &b, Matrix &x, bool symA = false);

	// Homogeneous Linear system Ax = 0
	// Modifies A
	//static bool SolveSVD(Matrix &A, Matrix &x);

public:
};

#endif
