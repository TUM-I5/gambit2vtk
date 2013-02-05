/**
 * @file
 *  This file is part of gambit2vtk
 *
 *  For conditions of distribution and use, please see the copyright
 *  notice in the file 'COPYING' at the root directory of this package
 *  and the copyright notice at https://github.com/TUM-I5/gambit2vtk
 *
 * @copyright 2012-2013 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

#ifndef CONVERTER_CONVERTER_H_
#define CONVERTER_CONVERTER_H_

#include <vector>

namespace converter
{

class Converter
{
public:
	enum Type { TRIANGLE = 0, TETRA, UNSUPPORTED };

public:
	virtual ~Converter() {}

	virtual void startCoordinates() {};
	virtual void endCoordinates() {};
	virtual void startElements() {}
	virtual void endElements() {};
	virtual void convertCoordinate(std::vector<double> &coordinate) {};
	virtual void convertElement(std::vector<unsigned long> &coordinates, Type type) {};
	virtual void convertData(double value) {};
	virtual void convertData(long value) {};

	/**
	 * @return The number of points for a specific type
	 */
	static int nPoints(Type t)
	{
		return TYPE_NPOINTS[t];
	}

private:
	static const int TYPE_NPOINTS[];
};

} /* namespace converter */
#endif /* CONVERTER_CONVERTER_H_ */
