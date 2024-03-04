#pragma once
#include "vpoint.h"
#include "sVector.h"
#include <vector>

class SerializeVector
{
public:
	SerializeVector(void);
    void write (const std::vector<sVector<double,3> > &points,
                    const	std::vector<sVector<double, 3> > &normals,
                    const	std::vector<sVector<int, 3> > &indices,
					VofStream &f);
	
    void read	(std::vector<sVector<double,3> > &points,
                  std::vector<sVector<double,3> > &normals,
                  std::vector<sVector<int,3> > &indices,
				  const VStr &filename);
	~SerializeVector(void);
};
