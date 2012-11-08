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

#include "gambitreader.h"

const converter::Converter::Type io::GambitReader::GAMBIT2INTERNAL[] = {
		converter::Converter::UNSUPPORTED,
		converter::Converter::UNSUPPORTED,
		converter::Converter::UNSUPPORTED,
		converter::Converter::TRIANGLE,
		converter::Converter::UNSUPPORTED,
		converter::Converter::UNSUPPORTED,
		converter::Converter::TETRA,
		converter::Converter::UNSUPPORTED};

const char* io::GambitReader::GAMBIT_FILE_ID = "** GAMBIT NEUTRAL FILE";
const char* io::GambitReader::ENDSECTION = "ENDOFSECTION";
const char* io::GambitReader::NODAL_COORDINATES = "NODAL COORDINATES";
const char* io::GambitReader::ELEMENT_CELLS = "ELEMENTS/CELLS";
const char* io::GambitReader::ELEMENT_GROUP = "ELEMENT GROUP";
