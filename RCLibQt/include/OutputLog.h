#pragma once
#include "RCLibQt_Fwd.h"
#include <QListWidget>
#include <QString>

namespace RCLib {
namespace Qt {

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

	OutputLog(QWidget* parent = nullptr);
	virtual ~OutputLog() {}

	void Log(QString, ELogLevel);
	void AddMessage(const QString& message);
	void Clear();
};

} // namespace Qt
} // namespace RCLib
