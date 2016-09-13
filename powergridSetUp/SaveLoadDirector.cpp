#include "stdafx.h"
#include "SaveLoadDirector.h"


// Constructor
SaveLoadDirector::SaveLoadDirector()
{
}

// Destructor
SaveLoadDirector::~SaveLoadDirector()
{
}

// Set the type of Builder
void SaveLoadDirector::setBuilder(SaveLoadBuilder* b)
{
	builder = b;
}

// Return Map held in object
Map SaveLoadDirector::getMap()
{
	return builder->getMap();
}

// Build a map, depending on the type of Builder set
void SaveLoadDirector::buildMap(Map m)
{
	builder->createMap(m);
}
