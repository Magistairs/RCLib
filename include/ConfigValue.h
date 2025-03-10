#pragma once
#include "ConfigFile.h"
#include <QtIncludes.h>

#define DECLARE_CONFIG_VALUE(name) \
	ConfigValue name                 \
	{                                \
		#name, &m_config               \
	}

class ConfigValue : public QObject
{
public:
	ConfigValue(const char* label, ConfigFile* pFile);
	virtual ~ConfigValue();

	void operator=(const QJsonValue& value) { m_value = value; }
	operator QJsonValue&() { return m_value; }
	operator QString() const { return m_value.toString(); }

protected:
	QJsonValue  m_value;
	QString     m_label;
	ConfigFile* m_pFile{nullptr};
};
