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

#ifndef IO_DATAREADER_H_
#define IO_DATAREADER_H_

namespace io
{

class DataReader
{
public:
	enum Type { INT64, FLOAT64 };

public:
	virtual ~DataReader() {}

	virtual void parseData(unsigned long size) = 0;

	/** The name of this data */
	virtual const char* dataName() = 0;

	/** The type of data */
	virtual Type dataType() = 0;
};

} /* namespace io */
#endif /* IO_DATAREADER_H_ */
