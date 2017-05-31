#include "ParticleType.h"

std::string getLine(std::istringstream &ifs)
{
	int maxchars = 8192;				// Alloc enough size.
	std::vector<char> buf(maxchars);	// Alloc enough size.
	if(ifs.peek() != -1)
	{
		ifs.getline(&buf[0], maxchars);
		
		std::string linebuf(&buf[0]);
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\n') linebuf.erase(linebuf.size()-1);
		}
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\r') linebuf.erase(linebuf.size()-1);
		}

		return linebuf;
	}

	return "";
}

std::string readString (std::string &line)
{
	return line.substr(line.find(":") + 2, line.size() - (line.find(":") + 2));
}

bool readBool (std::string &line)
{
	return readString(line).find("false") == std::string::npos?true:false;
}

int readInt (std::string &line) 
{
	return atoi(readString(line).c_str());
}

float readFloat (std::string &line)
{
	return atof(readString(line).c_str());
}
