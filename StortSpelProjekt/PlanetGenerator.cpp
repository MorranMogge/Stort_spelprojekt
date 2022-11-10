#include "stdafx.h"
#include "PlanetGenerator.h"
#include "GPU.h"
#include "Input.h"

PlanetGenerator::PlanetGenerator()
{
    meshes.emplace_back(new Mesh());
    planet = new Planet(meshes[0]);
}

PlanetGenerator::~PlanetGenerator()
{
    delete planet;
    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
}

GAMESTATE PlanetGenerator::Update()
{
    return NOCHANGE;
}

void PlanetGenerator::Render()
{
}
