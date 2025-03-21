#include "Application.h"

int main(int argc, char* argv[])
{
	TestsQt::Application app(argc, argv);
	
	if (!app.Initialize())
	{
		return 1;
	}
	
	app.Run();
	return 0;
}