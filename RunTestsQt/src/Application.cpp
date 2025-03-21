#include "Application.h"
#include "MainWindow.h"
#include "QtFactory.h"
#include "RCLib.h"

namespace TestsQt
{
using IEngine = RCLib::IEngine;
using namespace RCLibQt;

Application::Application(int& argc, char* argv[])
  : QApplication(argc, argv)
{
}

bool Application::Initialize()
{
	// Create Qt factory first
	auto               pFactory = MakeShared<QtFactory>();
	RCLib::IManagerPtr pEngine  = pFactory->Create<RCLib::IStateWrapper<IEngine>>();
	// Create engine using the factory
	if (!pEngine)
	{
		return false;
	}

	// Initialize the engine
	pEngine->Initialize();

	// Create main window
	m_pMainWindow = std::make_unique<MainWindow>();
	m_pMainWindow->show();

	// Run all tests
	auto& testRunner = pEngine->GetTestRunner();
	testRunner.RunAllTests();

	return true;
}

void Application::Run()
{
	exec();
}

} // namespace TestsQt