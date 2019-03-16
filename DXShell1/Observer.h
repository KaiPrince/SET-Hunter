#pragma once
#include <vector>

class Observable;

/*
Class Name: Observer
Purpose: This class is used to implement the Observer Pattern.
*/
class Observer
{
public:
	Observer();
	virtual ~Observer();

	virtual void Update(Observable* subject) = 0;
};

/*
Class Name: Observable
Purpose: This class is used to implement the Observer Pattern.
*/
class Observable
{
	std::vector<Observer*> Observers;
public:
	Observable();
	virtual ~Observable();

	virtual void Notify();
	virtual void Attach(Observer* observer);

};

