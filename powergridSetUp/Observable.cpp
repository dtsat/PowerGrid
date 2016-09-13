#include "stdafx.h"
#include "Observable.h"

// Constructor
Observable::Observable()
{
}

// Destructor
Observable::~Observable()
{
}

// Attach takes a pointer to observer object and adds it to vector of observers
void Observable::attach(Observer *a)
{
	observers.push_back(a);
}

// detach takes a pointer to observer
// iterates the observers vector from start to end, looks for incidents of observer a and removes a from vector
void Observable::detach(Observer *a)
{
	for (auto it = this->observers.begin(); it < this->observers.end(); it++) {

		if (*it == a) {
			this->observers.erase(it);
			break;
		}
	}
}

// notify interates the observers vector and calls update() function for each observer, passing pointer to itself as parameters
void Observable::notify(int j)
{
	for (int i = 0; i < observers.size(); i++)
		observers[i]->update(this, j);
}
