#pragma once
#include <QtIncludes.h>

namespace RCLib {

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

} // namespace RCLib
