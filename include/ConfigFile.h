#pragma once
#include <QtIncludes.h>

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
