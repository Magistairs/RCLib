#include "MainWindow.h"

#include <RCLib.h>

#include <QMessageBox>

namespace TestsQt
{

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
	SetupUI();
	ConnectSignals();
}

void MainWindow::SetupUI()
{
	// Create central widget and layout
	m_centralWidget = new QWidget(this);
	setCentralWidget(m_centralWidget);
	auto* mainLayout = new QVBoxLayout(m_centralWidget);

	// Create log widget
	m_logWidget = new LogWidget(this);
	mainLayout->addWidget(m_logWidget);

	// Create control widget
	m_controlWidget = new LogControlWidget(this);
	mainLayout->addWidget(m_controlWidget);

	// Create test button
	m_testButton = new QPushButton("Test Log", this);
	mainLayout->addWidget(m_testButton);

	// Set window properties
	setWindowTitle("RCLib Test Application");
	resize(800, 600);
}

void MainWindow::ConnectSignals()
{
	connect(m_testButton, &QPushButton::clicked, this, &MainWindow::OnTestLog);
	connect(m_controlWidget, &LogControlWidget::ClearLogRequested, this, &MainWindow::OnClearLog);
	connect(m_controlWidget, &LogControlWidget::SaveLogRequested, this, &MainWindow::OnSaveLog);
}

void MainWindow::OnClearLog()
{
	m_logWidget->clear();
}

void MainWindow::OnTestLog()
{
	auto& engine     = RCLib::IEngine::Get();
	auto& testRunner = *engine.GetTestRunner();

	// Run a simple test
	testRunner.RunTest("SimpleTest", []() {
		RCLib::IEngine::Get().GetLogger()->Info("Test passed!");
		return true;
	});
}

void MainWindow::OnSaveLog(const QString& filePath)
{
	if (m_logWidget->SaveToFile(filePath))
	{
		QMessageBox::information(this, "Success", "Log saved successfully");
	}
	else
	{
		QMessageBox::critical(this, "Error", "Failed to save log file");
	}
}

} // namespace TestsQt