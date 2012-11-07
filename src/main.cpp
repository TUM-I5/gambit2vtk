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

#include "config/args.h"
#include "converter/gambit2vtk.h"
#include "io/gambitreader.h"
#include "io/xmlwriter.h"
#include "tools/logger.h"

#include <iostream>
#include <fstream>
#include <string>

/**
 * Check if file already exists
 *
 * Taken from http://www.cplusplus.com/forum/general/1796/
 */
bool fexists(const char* filename)
{
	std::ifstream ifile(filename);
	return ifile;
}

int main(int argc, char** argv)
{
	config::Args args(argc, argv);

	if (!args.outpuFile())
		// XML file is printed to std::cout
		// -> use std::cerr for all log messages
		tools::Logger::logger.setOutputStream(std::cerr);

	// Get gambit input stream
	std::istream *gambitStream;
	std::ifstream gambitFile;
	if (args.inputFile() == 0L)
		gambitStream = &std::cin;
	else {
		gambitFile.open(args.inputFile(), std::fstream::in);
		if (!gambitFile.good()) {
			std::string error("Could not open gambit file ");
			error += args.inputFile();
			tools::Logger::logger.error(error);
		}

		gambitStream = &gambitFile;
	}

	// Get vtk output stream
	std::ostream *vtkStream;
	std::ofstream vtkFile;
	if (args.outpuFile() == 0L)
		vtkStream = &std::cout;
	else {
		if (!args.overwrite() && fexists(args.outpuFile()))
			tools::Logger::logger.error("Output file already exists, use -f to overwrite");

		std::string filename(args.outpuFile());
		if (filename.rfind(".vtu") != filename.size() - 4)
			tools::Logger::logger.warning("Output file extension should be .vtu");

		vtkFile.open(args.outpuFile(), std::fstream::out);
		if (!vtkFile.good())
			tools::Logger::logger.error("Could not open vtk file");

		vtkStream = &vtkFile;
	}

	io::GambitReader gambitReader(*gambitStream);
	io::XMLWriter xmlWriter(*vtkStream);

	converter::Gambit2VTK(gambitReader, xmlWriter);

	return 0;
}

