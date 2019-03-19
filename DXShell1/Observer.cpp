#include "Observer.h"
#include <algorithm>



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

void Observable::RemoveObserver(Observer * observer)
{
	if (observer != nullptr) {
		Observers.erase(std::remove(Observers.begin(), Observers.end(), observer), Observers.end());
	}
}
