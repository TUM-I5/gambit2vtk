/**
 * @file
 *  This file is part of gambit2vtk
 *
 *  For conditions of distribution and use, please see the copyright
 *  notice in the file 'COPYING' at the root directory of this package
 *  and the copyright notice at https://github.com/TUM-I5/gambit2vtk
 *
 * @copyright 2013 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

#ifndef IO_INDEXDATAREADER_H_
#define IO_INDEXDATAREADER_H_

#include "converter/converter.h"
#include "io/datareader.h"

namespace io
{

class IndexDataReader: public io::DataReader
{
private:
	converter::Converter &m_converter;

public:
	IndexDataReader(converter::Converter &converter)
		: m_converter(converter)
	{
	}

	/**
	 * @param size Number of values that should be extracted
	 *  from the input stream
	 */
	void parseData(unsigned long size)
	{
		for (unsigned long i = 0; i < size; i++) {
			m_converter.convertData(static_cast<long>(i));
		}
	}

	const char* dataName()
	{
		return "index";
	}

	Type dataType()
	{
		return INT64;
	}
};

} /* namespace io */
#endif /* INDEXDATAREADER_H_ */
