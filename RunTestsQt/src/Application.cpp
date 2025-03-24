#include "Application.h"
#include "MainWindow.h"
#include "QtFactory.h"
#include "RCLib.h"

namespace TestsQt
{
using IEngine = RCLib::IEngine;

Application::Application(int& argc, char* argv[])
  : QApplication(argc, argv)
{
}

bool Application::Initialize()
{
	// Create Qt factory first
	auto                    pFactory = MakeShared<QtFactory>();
	RCLib::IStateWrapperPtr pEngine  = MakeShared<RCLib::Impl::DefaultStateWrapper<IEngine>>("Engine", pFactory->Create<IEngine>());
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

	return true;
}

void Application::Run()
{
	exec();
}

} // namespace TestsQt