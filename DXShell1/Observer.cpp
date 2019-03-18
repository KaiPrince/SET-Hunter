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
	/*for each (Observer* observer in Observers)
	{
		observer->Notify(this);
	}*/
	for (Observer* observer : Observers) {
		observer->Notify(this);
	}
}

void Observable::AddObserver(Observer* observer) {
	Observers.push_back(observer);
}