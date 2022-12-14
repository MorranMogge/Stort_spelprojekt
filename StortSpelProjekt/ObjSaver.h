#pragma once
#include <iostream>
#include "Vertex.h"
#include <array>

void saveObj(const std::string& fileName, const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices);