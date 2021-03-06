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

#ifndef IO_XMLWRITER_H_
#define IO_XMLWRITER_H_

#include "tools/logger.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlwriter.h>

namespace io
{

/**
 * The writer only supports the ascii charset at the moment
 */
class XMLWriter
{
private:
	xmlOutputBufferPtr m_outputBuffer;
	xmlTextWriterPtr m_xmlWriter;
	/** Output stream where the xml file should be written to */
	std::ostream &m_output;

public:
	XMLWriter(std::ostream &output)
		: m_output(output)
	{
		m_outputBuffer = xmlAllocOutputBuffer(0L);
		if (!m_outputBuffer)
			tools::Logger::logger.error("Could not allocate xml output buffer");

		m_output << std::setprecision(12);

		m_outputBuffer->context = this;
		m_outputBuffer->writecallback = outputWriteCallback;
		m_outputBuffer->closecallback = 0L;
		m_xmlWriter = xmlNewTextWriter(m_outputBuffer);

		xmlTextWriterStartDocument(m_xmlWriter, 0L, 0L, 0L);
	}

	virtual ~XMLWriter()
	{
		xmlTextWriterEndDocument(m_xmlWriter);

		xmlFreeTextWriter(m_xmlWriter);
	}

	void startElement(const char* name)
	{
		xmlTextWriterStartElement(m_xmlWriter, reinterpret_cast<const xmlChar*>(name));
	}

	void endElement()
	{
		xmlTextWriterEndElement(m_xmlWriter);
	}

	template<typename T>
	void attribute(const char* name, T value)
	{
		// Convert value to const char*
		std::string str;
		std::ostringstream ss(str);
		ss << value;

		// This calls the specialized function, so we don't get any
		// recursion
		attribute(name, ss.str().c_str());
	}

	template<typename T>
	void content(T c)
	{
		// Convert value to const char*
		std::string str;
		std::ostringstream ss(str);
		ss << c;

		// This calls the specialized function, so we don't get any
		// recursion
		content(ss.str().c_str());
	}
private:
	static int outputWriteCallback(void* context, const char* buffer, int len)
	{
		static_cast<XMLWriter*>(context)->m_output.write(buffer, len);

		return len;
	}
};


template<> inline
void XMLWriter::attribute(const char* name, const char* value)
{
	xmlTextWriterWriteAttribute(m_xmlWriter,
		reinterpret_cast<const xmlChar*>(name),
		reinterpret_cast<const xmlChar*>(value));
}

template<> inline
void XMLWriter::content(const char* content)
{
	xmlTextWriterWriteString(m_xmlWriter, reinterpret_cast<const xmlChar*>(content));
}

} /* namespace io */
#endif /* IO_XMLWRITER_H_ */
