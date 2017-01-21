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

	struct CValue
	{
		virtual ~CValue()
		{

		}
		static CValue GetValue(std::string val);
		/*
		 CSettingsReader::CValue CSettingsReader::CValue::GetValue(std::string val)
		 {


		 }
		 */
	};

	bool OpenAndRead();

	std::string m_path;
	typedef std::map<std::string, std::string > TParsed;
	TParsed m_parsed;
	std::map<std::string, int >  m_parsedInt;
	virtual ~CSettingsReader();
	std::fstream m_file;
};

#endif /* CSETTINGSREADER_H_ */
