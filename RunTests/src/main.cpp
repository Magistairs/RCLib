#include "RCLib.h"
#include "TestMain.h"

int main(int argc, char* argv[])
{
	RCLib::DefaultStateWrapper<Tests::TestMain> testMain;

	testMain.Initialize();
	bool success = testMain.Update();
	testMain.Release();

	return success ? 0 : 1;
}
