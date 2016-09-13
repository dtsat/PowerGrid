#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>

// This abstract class implements the Observer Pattern by acting as the superclass Observer
// All observers hold and define an update function that takes an observable object and an integer
// The parameters decide the type of update to perform

// forward declaration
class Observable;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void update(Observable* ob, int i) = 0;
};

#endif