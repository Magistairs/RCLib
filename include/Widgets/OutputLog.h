#pragma once
#include <QtIncludes.h>

class OutputLog : public QListWidget
{
public:
	enum ELogLevel
	{
		Debug,
		Info,
		Warning,
		Error
	};

	OutputLog(QWidget* pParent);
	virtual ~OutputLog() {}

	void Log(QString, ELogLevel);
};
