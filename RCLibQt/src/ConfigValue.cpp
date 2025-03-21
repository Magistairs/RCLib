#include "RCLibQt.h"
#include "ConfigValue.h"
#include "RCLib.h"

using namespace RCLib::Qt;

ConfigValue::ConfigValue(const char* label, ConfigFile* pFile)
  : m_label(label)
  , m_pFile(pFile)
{
	connect(pFile, &ConfigFile::loaded, [this] { m_value = m_pFile->Object()[m_label]; });
}

ConfigValue::~ConfigValue()
{
	m_pFile->Object()[m_label] = m_value;
}
