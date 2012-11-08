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
