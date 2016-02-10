/*
 * worldMeta.h
 *
 *  Created on: Jan 30, 2016
 *      Author: felix
 */

#ifndef SRC_UTIL_WORLDMETA_H_
#define SRC_UTIL_WORLDMETA_H_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../config/data.h"

// Maybe rename to file util

namespace world_meta
{

void addName(std::string name)
{
	std::vector<std::string> names{name};
	std::ifstream inStream(config::worldMetaFile);

	std::string n;
	while (getline(inStream, n))
		names.push_back(n);

	inStream.close();
	std::sort(names.begin(), names.end(),
			[](std::string a, std::string b) -> bool { return a < b; });

	std::ofstream outStream(config::worldMetaFile);

	for (auto nn : names)
		outStream << nn << "\n";

	outStream.close();
}

bool worldNameExists(std::string name)
{
	std::ifstream inStream(config::worldMetaFile);

	std::string n;
	while (getline(inStream, n)) {
		if (n == name)
			return true;
	}
	return false;
}

void deleteName(std::string name)
{
	std::vector<std::string> files;



	std::remove("");
}

std::vector<std::string> getAllWorldNames()
{
	std::vector<std::string> names;
	std::ifstream inStream(config::worldMetaFile);

	std::string n;
	while (getline(inStream, n))
		names.push_back(n);

	inStream.close();

	return names;
}

}

#endif /* SRC_UTIL_WORLDMETA_H_ */
