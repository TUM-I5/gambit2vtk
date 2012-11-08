/**
 * @file
 *  This file is part of gambit2vtk
 *
 *  For conditions of distribution and use, please see the copyright
 *  notice in the file 'COPYING' at the root directory of this package
 *  and the copyright notice at https://github.com/TUM-I5/gambit2vtk
 *
 * @copyright 2012 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

#ifndef IO_SIMPLEDATAREADER_H_
#define IO_SIMPLEDATAREADER_H_

#include "converter/converter.h"
#include "io/datareader.h"

#include <iostream>
#include <string>

namespace io
{

class SimpleDataReader: public io::DataReader
{
private:
	converter::Converter &m_converter;
	std::istream &m_input;
	const std::string m_name;

public:
	SimpleDataReader(converter::Converter &converter, std::istream &input, const char* filename)
		: m_converter(converter),
		  m_input(input),
		  m_name(extractFilename(filename))
	{
	}

	/**
	 * @param size Number of values that should be extracted
	 *  from the input stream
	 */
	void parseData(unsigned long size)
	{
		for (unsigned long i = 0; i < size; i++) {
			double value;
			m_input >> value;
			m_converter.convertData(value);
		}
	}

	const char* dataName()
	{
		return m_name.c_str();
	}

private:
	/**
	 * Removes any path information form the filename
	 */
	static std::string extractFilename(const char* filename)
	{
		size_t found = std::string(filename).find_last_of("/\\");
		return std::string(&filename[found+1]);
	}
};

} /* namespace io */
#endif /* SIMPLEDATAREADER_H_ */
