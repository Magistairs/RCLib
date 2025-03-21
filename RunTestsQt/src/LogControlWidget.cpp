#include "LogControlWidget.h"

#include <RCLib.h>

#include <QFileDialog>
#include <QMessageBox>

namespace TestsQt
{

LogControlWidget::LogControlWidget(QWidget* parent)
  : QWidget(parent)
{
	SetupUI();
	ConnectSignals();
}

void LogControlWidget::SetupUI()
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);

	// Log level control
	m_levelLabel = new QLabel("Log Level:", this);
	m_levelCombo = new QComboBox(this);
	m_levelCombo->addItems({"Debug", "Info", "Warning", "Error"});
	m_levelCombo->setCurrentText("Info");
	layout->addWidget(m_levelLabel);
	layout->addWidget(m_levelCombo);

	// Output controls
	m_consoleOutput = new QCheckBox("Console Output", this);
	m_consoleOutput->setChecked(true);
	layout->addWidget(m_consoleOutput);

	m_fileOutput = new QCheckBox("File Output", this);
	layout->addWidget(m_fileOutput);

	// Action buttons
	m_saveButton = new QPushButton("Save Log", this);
	layout->addWidget(m_saveButton);

	m_clearButton = new QPushButton("Clear Log", this);
	layout->addWidget(m_clearButton);

	// Add stretch to push controls to the left
	layout->addStretch();
}

void LogControlWidget::ConnectSignals()
{
	connect(m_levelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LogControlWidget::OnLevelChanged);
	connect(m_consoleOutput, &QCheckBox::toggled, this, &LogControlWidget::OnConsoleOutputToggled);
	connect(m_fileOutput, &QCheckBox::toggled, this, &LogControlWidget::OnFileOutputToggled);
	connect(m_saveButton, &QPushButton::clicked, this, &LogControlWidget::OnSaveLog);
	connect(m_clearButton, &QPushButton::clicked, this, &LogControlWidget::OnClearLog);
}

void LogControlWidget::OnLevelChanged(int index)
{
	auto& logger = *RCLib::IEngine::Get().GetLogger();
	switch (index)
	{
	case 0: logger.SetLogLevel(RCLib::ELogLevel::eDebug); break;
	case 1: logger.SetLogLevel(RCLib::ELogLevel::eInfo); break;
	case 2: logger.SetLogLevel(RCLib::ELogLevel::eWarning); break;
	case 3: logger.SetLogLevel(RCLib::ELogLevel::eError); break;
	}
}

void LogControlWidget::OnConsoleOutputToggled(bool checked)
{
	auto& logger = *RCLib::IEngine::Get().GetLogger();
	logger.EnableConsoleOutput(checked);
}

void LogControlWidget::OnFileOutputToggled(bool checked)
{
	auto& logger = *RCLib::IEngine::Get().GetLogger();
	logger.EnableFileOutput(checked);
}

void LogControlWidget::OnSaveLog()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Save Log File", "", "Log Files (*.log);;All Files (*)");

	if (!filePath.isEmpty())
	{
		emit SaveLogRequested(filePath);
	}
}

void LogControlWidget::OnClearLog()
{
	emit ClearLogRequested();
}

} // namespace TestsQt