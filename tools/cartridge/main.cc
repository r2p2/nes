#include <iostream>
#include <string>
#include <fstream>

#include "../../cartridge.h"

int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		std::cerr << "pass the path to rom image" << std::endl;
		return 1;
	}

	std::fstream rom;
	rom.open(argv[1], std::fstream::in);
	
	Cartridge cartridge;
	cartridge.load(rom);

	rom.close();
	return 0;
}
