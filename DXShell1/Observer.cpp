#include "Observer.h"



Observer::Observer()
{
}


Observer::~Observer()
{
}

Observable::Observable() {

}

Observable::~Observable() {

}

void Observable::Notify() {
	for each (Observer* observer in Observers)
	{
		observer->Update(this);
	}
}

void Observable::Attach(Observer* observer) {
	Observers.push_back(observer);
}