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

#ifndef CONFIG_ARGS_H_
#define CONFIG_ARGS_H_

#include "tools/logger.h"

#include <getopt.h>
#include <cstdlib>
#include <iostream>

namespace config
{

class Args
{
private:
	const char* m_inputFile;
	const char* m_outputFile;
	/** Set to 1 if any existing output file should be overwritten */
	bool m_overwrite;

public:
	Args(int argc, char** argv)
		: m_inputFile(0L),
		  m_outputFile(0L),
		  m_overwrite(false)
	{
		const struct option longOptions[] = {
			{"input", required_argument, 0, 'i'},
			{"output", required_argument, 0, 'o'},
			{"force", no_argument, 0, 'f'},
			{"help", no_argument, 0, 'h'}
		};

		int c, optionIndex;
		while ((c = getopt_long(argc, argv, "i:o:fh",
			longOptions, &optionIndex)) >= 0) {
			switch (c) {
			case 0:
				tools::Logger::logger.error("Could not parse command line arguments");
				break;
			case 'i':
				m_inputFile = optarg;
				break;
			case 'o':
				m_outputFile = optarg;
				break;
			case 'f':
				m_overwrite = true;
				break;
			case 'h':
				printHelpMessage();
				exit(0);
				break;
			case '?':
				printHelpMessage(std::cerr);
				abort();
				break;
			default:
				tools::Logger::logger.error("Could not parse command line arguments");
				break;
			}
		}
	}

	const char* inputFile()
	{
		return m_inputFile;
	}

	const char* outpuFile()
	{
		return m_outputFile;
	}

	bool overwrite()
	{
		return m_overwrite;
	}

private:
	/**
	 * Prints the help message, showing all available options
	 *
	 * @param out The output stream the should be used for
	 *  printing
	 */
	void printHelpMessage(std::ostream &out = std::cout)
	{
		out << "Usage: gambit2vtk [OPTIONS...]" << std::endl
			<< "  -i, --input=FILE              name of the gambit input file" << std::endl
			<< "                                (if not set, stdin is used)" << std::endl
			<< "  -o, --output=FILE.vtu         name of the vtk output file" << std::endl
			<< "                                (if not set, stdout is used)" << std::endl
			<< "  -h, --help                    this help list" << std::endl;
	}
};

} /* namespace config */
#endif /* CONFIG_ARGS_H_ */
