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

#ifndef TOOLS_LOGGER_H_
#define TOOLS_LOGGER_H_

#include <cstdlib>
#include <iostream>

namespace tools {

class Logger {
public:
	enum Level { INFO, WARNING, ERROR };

private:
	/** Stream where we print all message */
	std::ostream* m_output;

private:
	Logger()
		: m_output(&std::cout)
	{
	}

public:
	void setOutputStream(std::ostream &output)
	{
		m_output = &output;
	}

	void log(std::string &message, Level level = INFO)
	{
		log(message.c_str(), level);
	}

	void log(const char* message, Level level = INFO)
	{
		switch (level) {
		case INFO:
			info(message);
			break;
		case WARNING:
			warning(message);
			break;
		case ERROR:
			error(message);
			break;
		}
	}

	void info(std::string &message)
	{
		info(message.c_str());
	}

	void info(const char* message)
	{
		*m_output << message << std::endl;
	}

	std::ostream& info()
	{
		return *m_output;
	}

	void warning(std::string &message)
	{
		warning(message.c_str());
	}

	void warning(const char* message)
	{
		*m_output << "Warning: " << message << std::endl;
	}

	std::ostream& warning()
	{
		return *m_output << "Warning: ";
	}

	void error(std::string &message)
	{
		error(message.c_str());
	}

	void error(const char* message)
	{
		// Error messages are always send to std::cerr
		std::cerr << "Error: " << message << std::endl;
		exit(1);
	}

	/**
	 * Can be used to print arbitrary info messages.
	 * Does not append std::endl.
	 */
	template<typename T>
	Logger& operator<<(T value)
	{
		*m_output << value;
		return *this;
	}

	/**
	 * Allow to print std::endl
	 */
	Logger& operator<<(std::ostream& (*func)(std::ostream&))
	{
		*m_output << func;
		return *this;
	}

public:
	static Logger logger;
};

} /* namespace tools */

#endif /* TOOLS_LOGGER_H_ */
