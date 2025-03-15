#include "Pane.h"

Pane::Pane(const QString& name, QWidget* pParent)
  : QMainWindow(pParent)
{
	setWindowTitle(name);
	setCentralWidget(new QWidget(this));
	setAttribute(Qt::WA_DeleteOnClose, false);

	QDockWidget* pLogDockWidget = new QDockWidget("Output", this);
	pLogDockWidget->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
	addDockWidget(Qt::BottomDockWidgetArea, pLogDockWidget);

	OutputLog* pLogWidget = new OutputLog(this);
	pLogDockWidget->setWidget(pLogWidget);
	connect(this, &Pane::log, pLogWidget, &OutputLog::Log);

	QDockWidget* pInfoDockWidget = new QDockWidget("Info", this);
	pInfoDockWidget->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
	tabifyDockWidget(pLogDockWidget, pInfoDockWidget);

	QLabel* pInfoLabel = new QLabel(this);
	pInfoDockWidget->setWidget(pInfoLabel);
	pInfoLabel->setWordWrap(true);
	connect(this, &Pane::setInfo, pInfoLabel, &QLabel::setText);

	m_pProgressBar = new QProgressBar(this);
	statusBar()->addPermanentWidget(m_pProgressBar, 1);
	m_pProgressBar->hide();
	m_pProgressBar->setAlignment(Qt::AlignCenter);
	connect(this, &Pane::startNewProgress, this, &Pane ::OnStartNewProgress);
	connect(this, &Pane::setMaximumProgress, this, &Pane ::OnSetMaximumProgress);

	show();
}

void Pane::OnStartNewProgress(QString label)
{
	m_pProgressBar->setFormat(QString("%1: %v/%m").arg(label));
	m_pProgressBar->show();
}

void Pane::OnSetMaximumProgress(const int maximum)
{
	m_pProgressBar->setMaximum(maximum);
	m_progressRefreshTimer.setInterval(500);
	m_progressRefreshTimer.start();
	connect(&m_progressRefreshTimer, &QTimer::timeout, [this] {
		m_pProgressBar->setValue(m_currentProgress);
		if (m_currentProgress >= m_pProgressBar->maximum())
		{
			m_progressRefreshTimer.singleShot(2000, [this] { m_pProgressBar->hide(); });
		}
	});
}

void Pane::SetCurrentProgress(const int current)
{
	m_currentProgress = current;
}

void Pane::Log(QString str, OutputLog::ELogLevel level) const
{
	if (level == OutputLog::Debug)
		qDebug() << str;
	else if (level == OutputLog::Warning)
		qWarning() << str;
	else if (level == OutputLog::Error)
		qCritical() << str;
	else
		qInfo() << str;

	if (level != OutputLog::Debug)
		Q_EMIT log(str, level);
}