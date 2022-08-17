#include "texturetextwindow.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		const int width = 600;
		const int height = 600;

		ogl::Initialize(argc, argv, {} /*default context*/);
		ogl::CreateWindow("s3tc load example", width, height, false);

		const string imageFilename = argc == 1 ?
					"./" + string{"../resources/test_s3tsc_dxt3.dds"} : string{argv[1]};

		cout << "imageFilename = " << imageFilename << endl;
		TextureTestWindow window(width, height, imageFilename);
		ogl::RunMainLoop(window);
		ogl::Terminate();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& exc)
	{
		cerr << exc.what() << endl;
		ogl::Terminate();
		return EXIT_FAILURE;
	}
}
