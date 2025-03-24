#pragma once
#include "RCLibQt_Fwd.h"

#include <QJsonValue>
#include <QObject>
#include <QString>

#include <string_view>

#define DECLARE_CONFIG_VALUE(name) \
	ConfigValue name                 \
	{                                \
		#name, &m_config               \
	}

namespace RCLib::Qt
{

/**
 * @brief Configuration value that automatically saves to a ConfigFile
 * 
 * Provides a way to automatically persist QJsonValue data to a ConfigFile.
 * Values are automatically loaded when the config file is loaded and saved when the ConfigValue is destroyed.
 */
class RCLIB_QT_API ConfigValue : public QObject
{
public:
	/**
	 * @brief Constructs a new ConfigValue with the given label and file
	 * @param label The label to use as the JSON key
	 * @param pFile The config file to save to/load from
	 */
	ConfigValue(std::string_view label, ConfigFile* pFile);
	virtual ~ConfigValue();

	void operator=(const QJsonValue& value) { m_value = value; }
	operator QJsonValue&() { return m_value; }
	operator QString() const { return m_value.toString(); }

protected:
	QJsonValue  m_value;
	QString     m_label;
	ConfigFile* m_pFile{nullptr};
};

} // namespace RCLib::Qt
