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

#ifndef IO_GAMBITREADER_H_
#define IO_GAMBITREADER_H_

#include "converter/converter.h"
#include "tools/logger.h"
#include "tools/strings.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace io
{

/**
 * Reads a gambit file
 */
class GambitReader
{
private:
	enum Section { NO_SECTION, COORDINATES, ELEMENTS };

	std::istream &m_input;
	/** Number of points in the mesh */
	unsigned long m_nPoints;
	/** Number of elements in the mesh */
	unsigned long m_nElems;
	/** Number of element groups */
	unsigned long m_nGroups;
	/** Number of coordinate directions (should be 2 or 3) */
	unsigned int m_dimensions;

	converter::Converter *m_converter;

	/** The section we are currently working on */
	Section m_section;

public:
	GambitReader(std::istream &input)
		: m_input(input),
		  m_nPoints(0),
		  m_nElems(0),
		  m_nGroups(0),
		  m_dimensions(0),
		  m_converter(0L),
		  m_section(NO_SECTION)
	{
		std::string line;

		// Read header information
		getline(m_input, line);	// First line contains version
								// we ignore this line for know
		line.clear();
		getline(m_input, line);
		trim(line);
		if (line != GAMBIT_FILE_ID)
			tools::Logger::logger.error("Input file is not a gambit file");

		std::string name;
		getline(m_input, name);		// Internal name
		trim(name);

		getline(m_input, line); 	// Skip line:
									// PROGRAM: Gambit VERSION: x.y.z

		getline(m_input, line);		// Date
		trim(line);
		tools::Logger::logger << "Opening gambit file \""
			<< name << "\" from " << line << std::endl;

		getline(m_input, line); 	// Skip problem size names

		int boundarys;
		m_input >> m_nPoints;
		m_input >> m_nElems;
		m_input >> m_nGroups;
		m_input >> boundarys;	// Not used at the moment
		m_input >> m_dimensions;
		getline(m_input, line); 	// Skip rest of the line
		switch (m_dimensions) {
		case 2:
		case 3:
			break;
		default:
			tools::Logger::logger.error("Mesh contains unknown number of dimensions");
			break;
		}

		tools::Logger::logger << "Mesh consists of " << m_nPoints
			<< " points and " << m_nElems << " elements" << std::endl;

		getline(m_input, line);
		trim(line);
		if (line != ENDSECTION)
			tools::Logger::logger.error("Unknown gambit file format");
	}

	unsigned long nPoints()
	{
		return m_nPoints;
	}

	unsigned long nElems()
	{
		return m_nElems;
	}

	unsigned int dimensions()
	{
		return m_dimensions;
	}

	void setConverter(converter::Converter *converter)
	{
		m_converter = converter;
	}

	/**
	 * Do the real work
	 */
	void parse()
	{
		while (!m_input.eof()) {
			m_input >> std::ws; // Skip white spaces

			std::string line;
			getline(m_input, line);

			if (line.find(NODAL_COORDINATES) == 0) {
				// Start coords section
				tools::Logger::logger.info("Start reading coordinates");

				m_converter->startCoordinates();
				m_section = COORDINATES;
			} else if (line.find(ELEMENT_CELLS) == 0) {
				// Start element section
				tools::Logger::logger.info("Start reading elements");

				m_converter->startElements();
				m_section = ELEMENTS;
			} else if (line.find(ENDSECTION) == 0) {
				// End any open section
				switch (m_section) {
				case COORDINATES:
					m_converter->endCoordinates();
					break;
				case ELEMENTS:
					m_converter->endElements();
					break;
				default:
					// nothing to do
					break;
				}
				m_section = NO_SECTION;
			} else {
				std::istringstream ss;
				int n;
				std::vector<double> coords(m_dimensions);
				std::vector<unsigned long> points;

				// Read values
				switch (m_section) {
				case COORDINATES:
					ss.str(line);
					ss >> n; // point number

					// read coordinates
					for (unsigned int i = 0; i < m_dimensions; i++)
						ss >> coords[i];
					m_converter->convertCoordinate(coords);

					break;
				case ELEMENTS:
					ss.str(line);
					ss >> n; // element number

					converter::Converter::Type type;
					ss >> n;
					type = GAMBIT2INTERNAL[n];
					if (type == converter::Converter::UNSUPPORTED)
						tools::Logger::logger.error("Unsupported element type in gambit file");

					ss >> n; // Number of points
					if (converter::Converter::nPoints(type) != n)
						tools::Logger::logger.error("Number of points in does not match element type in gambit file");

					points.resize(n);
					for (int i = 0; i < n; i++)
						ss >> points[i];

					m_converter->convertElement(points, type);
					break;
				default:
					// no known section
					break;
				}
			}
		}
	}

private:
	static const converter::Converter::Type GAMBIT2INTERNAL[];

	static const char* GAMBIT_FILE_ID;
	static const char* ENDSECTION;
	static const char* NODAL_COORDINATES;
	static const char* ELEMENT_CELLS;
};

} /* namespace io */
#endif /* IO_GAMBITREADER_H_ */
