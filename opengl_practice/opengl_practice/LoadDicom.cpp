#include "LoadDicom.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


int LoadDicom::openDicom()
{
	int length = 0;
	char* buffer;
	ofstream file;

	ifstream openFile("D://sample//FILE229.dcm",ios::binary);

	openFile.seekg(0, ios::end);
	length = openFile.tellg();
	openFile.seekg(0, ios::beg);

	// allocate memory:
	buffer = new char[length];

	// read data as a block:
	openFile.read(buffer, length);

	openFile.close();

	cout.write(buffer, length);

	delete[] buffer;
	return 0;



	//stream openFile("c://AiOLog.txt",ios::binary);
	/*openFile.s
		while (openFile) 
		{
			char c;
			openFile.get(c);
			if (openFile) 
			{

				::std::cout << int(c) ;
			}
			/ *unsigned short* line;
			while (getline(openFile, *line)) {
				cout << line << endl;
			}* /
		}
		openFile.close();*/
	
	return true;
}

