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

	virtual void Notify(Observable* subject) = 0;
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
	virtual void AddObserver(Observer* observer);
	virtual void RemoveObserver(Observer* observer);

};

