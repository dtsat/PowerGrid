#include "stdafx.h"
#include "SaveLoadBuilder.h"


// Abstract class, cannot instantiate
SaveLoadBuilder::SaveLoadBuilder()
{
}

// Destructor
SaveLoadBuilder::~SaveLoadBuilder()
{
}

// Return the Map created by the object
Map SaveLoadBuilder::getMap()
{
	return map;
}
