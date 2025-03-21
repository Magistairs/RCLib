#pragma once
#include "RCLibQt_Fwd.h"

#include <QJsonValue>
#include <QObject>
#include <QString>

#define DECLARE_CONFIG_VALUE(name) \
	ConfigValue name                 \
	{                                \
		#name, &m_config               \
	}

namespace RCLib
{
namespace Qt
{

class RCLIB_QT_API ConfigValue : public QObject
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

} // namespace Qt
} // namespace RCLib
