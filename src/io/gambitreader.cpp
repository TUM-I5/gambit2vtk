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
