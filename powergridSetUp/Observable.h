#ifndef OBSERVEABLE_H
#define OBSERVEABLE_H
#include <vector>
#include "Observer.h"

// This abstract class is the superclass of all Observeable objects
// It holds a list of observers, and populates the list using the attach and detach functions
// Upon calling notify, all Observers in the list call update(), which updates appropriately
// Notify takes an integer, which is used to call the desired type of Observer update

class Observable
{
protected:
	std::vector<Observer*> observers;
public:
	Observable();
	virtual ~Observable();
	virtual void attach(Observer *a);
	virtual void detach(Observer *a);
	virtual void notify(int j);
};

#endif
