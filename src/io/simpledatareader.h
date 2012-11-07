/**
 * @file
 *  This file is part of gambit2vtk
 *
 *  gambit2vtk is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gambit2vtk is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gambit2vtk.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Diese Datei ist Teil von gambit2vtk.
 *
 *  gambit2vtk ist Freie Software: Sie koennen es unter den Bedingungen
 *  der GNU General Public License, wie von der Free Software Foundation,
 *  Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
 *  veroeffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 *  gambit2vtk wird in der Hoffnung, dass es nuetzlich sein wird, aber
 *  OHNE JEDE GEWAEHELEISTUNG, bereitgestellt; sogar ohne die implizite
 *  Gewaehrleistung der MARKTFAEHIGKEIT oder EIGNUNG FUER EINEN BESTIMMTEN
 *  ZWECK. Siehe die GNU General Public License fuer weitere Details.
 *
 *  Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 *  Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 * 
 * @copyright 2012 Sebastian Rettenberger <rettenbs@in.tum.de>
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
