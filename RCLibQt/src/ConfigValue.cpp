#include "ConfigValue.h"
#include "ConfigFile.h"

namespace RCLib::Qt
{

ConfigValue::ConfigValue(std::string_view label, ConfigFile* pFile)
  : m_label(QString::fromStdString(std::string(label)))
  , m_pFile(pFile)
{
	// Connect to config file's loaded signal to update our value when config is loaded
	connect(pFile, &ConfigFile::loaded, [this] { m_value = m_pFile->Object()[m_label]; });
}

ConfigValue::~ConfigValue()
{
	// Save our value back to the config file when destroyed
	m_pFile->Object()[m_label] = m_value;
}

} // namespace RCLib::Qt