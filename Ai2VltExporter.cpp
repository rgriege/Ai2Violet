#include <ostream>

#include "IllustratorSDK.h"
#include "Ai2VltExporter.h"

void Ai2Vlt::exportSvg(const char * filename)
{
	ofstream file;
	file.open(filename, ios::out);
	if (file.is_open())
	{
		file << "Test!\n";
		file.close();
	}
}
