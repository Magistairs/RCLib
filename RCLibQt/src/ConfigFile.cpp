#include "RCLibQt.h"
#include "ConfigFile.h"
#include "RCLib.h"

#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QFileInfo>

using namespace RCLib::Qt;

ConfigFile::ConfigFile()
  : QFile()
{
}

void ConfigFile::SetName(const QString& name)
{
	QDir path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	path      = path.filePath(name);
	setFileName(path.path() + "_config.json");
}

void ConfigFile::Load()
{
	if (exists())
	{
		if (open(QIODeviceBase::ReadOnly))
		{
			m_object = QJsonDocument::fromJson(readAll()).object();
			close();
			// PDXModdingAssistant::Log(fileName() + " loaded.", OutputLog::Info);
			Q_EMIT loaded();
		}
		else
		{
			// PDXModdingAssistant::Log(fileName() + " : " + errorString(), OutputLog::Error);
		}
	}
}

void ConfigFile::Save()
{
	QFileInfo fi(*this);
	if (!fi.absoluteDir().exists())
		fi.absoluteDir().mkpath(fi.absolutePath());

	if (open(QIODeviceBase::WriteOnly))
	{
		write(QJsonDocument(m_object).toJson());
		// PDXModdingAssistant::Log(fileName() + " saved.", OutputLog::Info);
		close();
	}
	else
	{
		// PDXModdingAssistant::Log(fileName() + " : " + errorString(), OutputLog::Error);
	}
}
