#pragma once
#include "MainWindow.h"

#include <QApplication>

#include <memory>

namespace TestsQt
{
/**
 * @brief Main application class that handles initialization
 */
class Application : public QApplication
{
public:
	Application(int& argc, char* argv[]);
	~Application() override = default;

	bool Initialize();
	void Run();

private:
	std::unique_ptr<MainWindow> m_pMainWindow;
};

} // namespace TestsQt