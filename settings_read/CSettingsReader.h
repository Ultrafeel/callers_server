/*
 * CSettingsReader.h
 *
 *  Created on: Jan 21, 2017
 *      Author: mml
 */
#include <fstream>
#include <map>
#ifndef CSETTINGSREADER_H_
#define CSETTINGSREADER_H_

class CSettingsReader {
public:
	CSettingsReader(char const* path);
	bool OpenAndRead();

    static std::string trimWhitesp(std::string const & str);
       static const char  delimiter1 = '=';

	std::string m_path;
	typedef std::map<std::string, std::string > TParsed;
	TParsed m_parsed;
	std::map<std::string, int >  m_parsedInt;
	virtual ~CSettingsReader();
	std::fstream m_file;
};

#endif /* CSETTINGSREADER_H_ */
