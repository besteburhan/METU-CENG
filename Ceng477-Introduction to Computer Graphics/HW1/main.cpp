#include "defs.h"
#include "Scene.h"
#include <iostream>
Scene *pScene; // definition of the global scene variable (declared in defs.h)

using namespace std;
int main(int argc, char *argv[])
{
	const char *xmlPath = argv[1];

    pScene = new Scene(xmlPath);

    pScene->renderScene();

	return 0;
}
