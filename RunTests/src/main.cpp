#include <DefaultStateWrapper.h>
#include <RCLibTests.h>

#include "TestMain.h"

int main(int argc, char* argv[])
{
	RCLib::Impl::DefaultStateWrapper<Tests::TestMain> testMain("TestMain", RCLib::IFactory::Get()->Create<Tests::TestMain>());

	testMain.Initialize();
	testMain.Update();
	testMain.Release();

	return 1;
}
