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

#ifndef CONVERTER_GAMBIT2VTK_H_
#define CONVERTER_GAMBIT2VTK_H_

#include "converter/converter.h"
#include "io/datareader.h"
#include "io/gambitreader.h"
#include "io/xmlwriter.h"

#include <vector>

namespace converter
{

class Gambit2VTK : public Converter
{
private:
	io::GambitReader &m_reader;
	io::XMLWriter &m_writer;

	/** The type of each cell */
	std::vector<Type> m_cellTypes;

	std::vector<io::DataReader*> m_dataReaders;

public:
	Gambit2VTK(io::GambitReader &reader, io::XMLWriter &writer)
		: m_reader(reader),
		  m_writer(writer)
	{
		reader.setConverter(this);

		writer.startElement("VTKFile");
		writer.attribute("type", "UnstructuredGrid");

		writer.startElement("UnstructuredGrid");

		writer.startElement("Piece");
		writer.attribute("NumberOfPoints", reader.nPoints());
		writer.attribute("NumberOfCells", reader.nElems());

		m_dataReaders.push_back(&reader);
	}

	void convert()
	{
		m_reader.parse();

		m_writer.startElement("CellData");

		// Get a list of all data reader names
		std::string names;
		for (size_t i = 0; i < m_dataReaders.size(); i++) {
			if (i)
				names += ' ';
			names += m_dataReaders[i]->dataName();
		}
		m_writer.attribute("Scalars", names);

		for (std::vector<io::DataReader*>::const_iterator i = m_dataReaders.begin();
			i < m_dataReaders.end(); i++) {
			tools::Logger::logger << "Converting element data "
				<< (*i)->dataName() << std::endl;

			m_writer.startElement("DataArray");
			m_writer.attribute("type", "Float64");
			m_writer.attribute("Name", (*i)->dataName());
			m_writer.attribute("Format", "ascii");

			(*i)->parseData(m_reader.nElems());

			m_writer.endElement();
		}
	}

	template<class T>
	void addDataReader(std::vector<T*> &dataReaders)
	{
		std::copy(dataReaders.begin(), dataReaders.end(), std::back_inserter(m_dataReaders));
	}

	void startCoordinates()
	{
		m_writer.startElement("Points");
		m_writer.startElement("DataArray");
		m_writer.attribute("type", "Float64");
		m_writer.attribute("NumberOfComponents", 3);
		m_writer.attribute("Format", "ascii");
	}

	void endCoordinates()
	{
		m_writer.endElement();
		m_writer.endElement();
	}

	void convertCoordinate(std::vector<double> &coordinate)
	{
		for (std::vector<double>::const_iterator i = coordinate.begin();
			i < coordinate.end(); i++) {
			m_writer.content(*i);
			m_writer.content(" ");
		}

		// We always need 3 dimensions in the vtk file ...
		for (size_t i = coordinate.size(); i < 3; i++) {
			m_writer.content(0.0);
			m_writer.content(" ");
		}
	}

	void startElements()
	{
		m_writer.startElement("Cells");
		m_writer.startElement("DataArray");
		m_writer.attribute("type", "Int32");
		m_writer.attribute("Name", "connectivity");
		m_writer.attribute("Format", "ascii");
	}

	void endElements()
	{
		m_writer.endElement();

		m_writer.startElement("DataArray");
		m_writer.attribute("type", "Int32");
		m_writer.attribute("Name", "offsets");
		m_writer.attribute("Format", "ascii");
		unsigned long offset = 0;
		for (std::vector<Type>::const_iterator i = m_cellTypes.begin();
			i < m_cellTypes.end(); i++) {
			offset += nPoints(*i);
			m_writer.content(offset);
			m_writer.content(" ");
		}
		m_writer.endElement();

		m_writer.startElement("DataArray");
		m_writer.attribute("type", "Int32");
		m_writer.attribute("Name", "types");
		m_writer.attribute("Format", "ascii");
		for (std::vector<Type>::const_iterator i = m_cellTypes.begin();
			i < m_cellTypes.end(); i++) {
			m_writer.content(INTERNAL2VTK[*i]);
			m_writer.content(" ");
		}
		m_writer.endElement();
		m_cellTypes.clear(); // Free unused memory

		m_writer.endElement();
	}

	void convertElement(std::vector<unsigned long> &coordinates, Type type)
	{
		for(std::vector<unsigned long>::const_iterator i = coordinates.begin();
			i < coordinates.end(); i++) {
			m_writer.content(*i-1);
			m_writer.content(" ");
		}

		m_cellTypes.push_back(type);
	}

	void convertData(double data)
	{
		m_writer.content(data);
		m_writer.content(" ");
	}
private:
	static const unsigned int INTERNAL2VTK[];
};

} /* namespace converter */
#endif /* CONVERTER_GAMBIT2VTK_H_ */
