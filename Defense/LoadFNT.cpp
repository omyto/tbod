#include "stdafx.h"
#include "LoadFNT.h"
#include <fstream>
#include <string>

GLuint loadFNT(const char * fileTarget)
{
	GLuint textcoorId;
	std::fstream file;
	file.open(fileTarget);

	if (!file)
	{
		printf(" File font number textcoor not found\n");
	}
	else
	{
		std::string line;
		char id;
		char num;
		int numChar;
		TextCoorData *textcoor = 0;
		if (!file.eof())
		{
			std::getline(file, line);
			sscanf(line.data(), "#Char count: %d", &numChar);
			textcoor = new TextCoorData[numChar];
			(numChar == 11) ? num = 48 : num = 32;
		}
		while (!file.eof())
		{
			std::getline(file, line);
			sscanf(line.data(), "%c", &id);
			id -= num;
			getline(file, line);
			sscanf(line.data(), "vt  %f %f", &textcoor[id].A.x, &textcoor[id].A.y);
			getline(file, line);
			sscanf(line.data(), "vt  %f %f", &textcoor[id].B.x, &textcoor[id].B.y);
			getline(file, line);
			sscanf(line.data(), "vt  %f %f", &textcoor[id].C.x, &textcoor[id].C.y);
			getline(file, line);
			sscanf(line.data(), "vt  %f %f", &textcoor[id].D.x, &textcoor[id].D.y);
		}
		file.close();
		glGenBuffers(1, &textcoorId);
		glBindBuffer(GL_ARRAY_BUFFER, textcoorId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TextCoorData)*numChar, textcoor, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] textcoor;
	}
	return textcoorId;
}

