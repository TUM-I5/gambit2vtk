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

#include "config/args.h"
#include "converter/gambit2vtk.h"
#include "io/gambitreader.h"
#include "io/simpledatareader.h"
#include "io/xmlwriter.h"
#include "tools/logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

	converter::Gambit2VTK  gambit2vtk(gambitReader, xmlWriter);

	// Get simple files input streams
	std::vector<std::ifstream*> simpleStreams;
	std::vector<io::SimpleDataReader*> simpleReaders;
	std::vector<const char*> simpleFiles = args.simpleFiles();
	for (std::vector<const char*>::const_iterator i = simpleFiles.begin();
		i < simpleFiles.end(); i++) {
		std::ifstream* stream = new std::ifstream(*i);
		if (stream->good()) {
			simpleStreams.push_back(stream);
			simpleReaders.push_back(new io::SimpleDataReader(gambit2vtk, *stream, *i));
		} else
			tools::Logger::logger.warning() << "Could not open "
				<< *i << ", ignoring file" << std::endl;
	}
	gambit2vtk.addDataReader(simpleReaders);

	// Start the convertion
	gambit2vtk.convert();

	// Delete all pointers
	for (std::vector<std::ifstream*>::iterator i = simpleStreams.begin();
		i < simpleStreams.end(); i++)
		delete *i;

	return 0;
}

