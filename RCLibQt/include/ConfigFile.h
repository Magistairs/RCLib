#pragma once
#include "RCLibQt_Fwd.h"
#include <QFile>
#include <QJsonObject>
#include <QString>
#include <QObject>

namespace RCLib {
namespace Qt {

class ConfigFile : public QFile
{
	Q_OBJECT

public:
	ConfigFile();
	virtual ~ConfigFile() { Save(); }

	void SetName(const QString& name);
	void Load();
	void Save();

	QJsonObject& Object() { return m_object; }

signals:
	void loaded();

protected:
	QJsonObject m_object;
};

} // namespace Qt
} // namespace RCLib
