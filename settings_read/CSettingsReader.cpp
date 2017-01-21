/*
 * CSettingsReader.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: mml
 */

#include "CSettingsReader.h"

CSettingsReader::CSettingsReader(char const* path):m_path(path) {
	// TODO Auto-generated constructor stub
	//printf(path);atoi(path);
	CSettingsReader::CValue v;
}

CSettingsReader::~CSettingsReader() {
	// TODO Auto-generated destructor stub
}

CSettingsReader::CValue CSettingsReader::CValue::GetValue(std::string val)
{

	return CValue;
}

void CSettingsReader::OpenAndRead()
{
	m_file.open(m_path.c_str());


}
