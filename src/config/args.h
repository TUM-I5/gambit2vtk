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

#ifndef CONFIG_ARGS_H_
#define CONFIG_ARGS_H_

#include "tools/logger.h"

#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace config
{

class Args
{
private:
	const char* m_inputFile;
	const char* m_outputFile;
	std::vector<const char*> m_simpleFiles;
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
			{"simple", required_argument, 0, 's'},
			{"force", no_argument, 0, 'f'},
			{"help", no_argument, 0, 'h'}
		};

		int c, optionIndex;
		while ((c = getopt_long(argc, argv, "i:o:s:fh",
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
			case 's':
				m_simpleFiles.push_back(optarg);
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

	const char* outputFile()
	{
		return m_outputFile;
	}

	const std::vector<const char*>& simpleFiles()
	{
		return m_simpleFiles;
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
			<< "                                (set to \"-\" for stdout)" << std::endl
			<< "  -s, --simple=FILE             a simple cell data file" << std::endl
			<< "                                each line contains one value for one cell" << std::endl
			<< "  -h, --help                    this help message" << std::endl;
	}
};

} /* namespace config */
#endif /* CONFIG_ARGS_H_ */
