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

#ifndef IO_GAMBITREADER_H_
#define IO_GAMBITREADER_H_

#include "converter/converter.h"
#include "io/datareader.h"
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
class GambitReader : public DataReader
{
private:
	enum Section { NO_SECTION, COORDINATES, ELEMENTS, GROUP };

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

	/** Group for each element */
	std::vector<unsigned int> m_group;
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

		// We need to store a group for each element
		m_group.resize(m_nElems);
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
				tools::Logger::logger.info("Reading coordinates");

				m_converter->startCoordinates();
				m_section = COORDINATES;
			} else if (line.find(ELEMENT_CELLS) == 0) {
				// Start element section
				tools::Logger::logger.info("Reading elements");

				m_converter->startElements();
				m_section = ELEMENTS;
			} else if (line.find(ELEMENT_GROUP) == 0) {
				// A matrial group
				m_section = GROUP;
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
				std::string str;
				unsigned long element, size;

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
				case GROUP:
					if (line.find("GROUP:") != 0)
						// Not the beginning of a group??
						break;

					ss.str(line);
					ss >> str; // "GROUP:"
					ss >> n; // group number
					ss >> str; // "ELEMENTS:"
					ss >> size; // Number of elements

					m_input >> std::ws; // read white spaces
					m_input >> str; // material name
					getline(m_input, line); // Read rest of the line
					tools::Logger::logger << "Reading material " << str
						<< std::endl;

					getline(m_input, line); // This line always contains a "0"

					for (unsigned int i = 0; i < size; i++) {
						m_input >> element;
						m_group[element-1] = n;
					}

					break;
				default:
					// no known section
					break;
				}
			}
		}
	}

	void parseData(unsigned long size)
	{
		if (size != m_group.size())
			tools::Logger::logger.error("Could not convert group information in gambit file");

		for (std::vector<unsigned int>::const_iterator i = m_group.begin();
			i < m_group.end(); i++)
			m_converter->convertData(*i);
	}

	const char* dataName()
	{
		return "group";
	}

private:
	static const converter::Converter::Type GAMBIT2INTERNAL[];

	static const char* GAMBIT_FILE_ID;
	static const char* ENDSECTION;
	static const char* NODAL_COORDINATES;
	static const char* ELEMENT_CELLS;
	static const char* ELEMENT_GROUP;
};

} /* namespace io */
#endif /* IO_GAMBITREADER_H_ */
