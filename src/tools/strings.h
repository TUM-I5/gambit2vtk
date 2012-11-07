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

#ifndef TOOLS_STRINGS_H_
#define TOOLS_STRINGS_H_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

/**
 * Trims from start
 *
 * Taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

/**
 * Trims from end
 *
 * Taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

/**
 * Trims from both ends
 *
 * Taken from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


#endif /* TOOLS_STRINGS_H_ */
