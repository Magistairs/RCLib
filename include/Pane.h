#pragma once
#include <QtIncludes.h>

#include "OutputLog.h"

class Pane : public QMainWindow
{
	Q_OBJECT

public:
	Pane(const QString& name, QWidget* pParent);
	virtual ~Pane() {}

	void SetCurrentProgress(const int current);
	void Log(QString str, OutputLog::ELogLevel level) const;

private:
	void OnStartNewProgress(QString label);
	void OnSetMaximumProgress(const int maximum);

Q_SIGNALS:
	void log(QString, OutputLog::ELogLevel) const;
	void setInfo(const QString& text) const;

	void startNewProgress(QString label);
	void setMaximumProgress(const int maximum);

private:
	QProgressBar*    m_pProgressBar = nullptr;
	QTimer           m_progressRefreshTimer;
	std::atomic<int> m_currentProgress;
};
