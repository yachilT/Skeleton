#include "Volunteer.h"
//Volunteer:
Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name)  {}
Volunteer::Volunteer(const Volunteer& other): completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId), id(other.id), name(other.name) {}
int Volunteer::getId() const
{
    return id;
};
const string& Volunteer::getName() const
{
    return name;
}
int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}
int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

void Volunteer::passOrder()
{
    completedOrderId = NO_ORDER;
}

bool Volunteer::isBusy() const
{
    return !(activeOrderId == NO_ORDER && completedOrderId == NO_ORDER);
};
bool Volunteer::hasCompleted() const
{
    return activeOrderId == NO_ORDER && completedOrderId != NO_ORDER;
}

//CollectorVolunteer:

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id,name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
};
void CollectorVolunteer::step()
{
    timeLeft -= 1;
    if(timeLeft == 0)
    {
        completedOrderId = activeOrderId; 
        activeOrderId = NO_ORDER;
    } 
}


int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}
bool CollectorVolunteer::decreaseCoolDown()
{
    timeLeft--;
    return timeLeft == 0;   
}
bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}


bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy();
}
void CollectorVolunteer::acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    timeLeft = coolDown;
}
string CollectorVolunteer::toString() const
{
    return "VolunteerID: " + std::to_string(this->getId())
    + "\nisBusy: " + (this->isBusy() ? "True" : "False")
    + "\nOrderID: " + (activeOrderId == -1 ? "None" : std::to_string(activeOrderId))
    + "\nTimeLeft: " + (timeLeft == 0 ? "None" : std::to_string(timeLeft))
    + "\nOrdersLeft: " + "No Limit";
};
//LimitedCollectorVolunteer

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders): CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return ordersLeft != 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
     return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft -= 1;

}

int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
    return "VolunteerID: " + std::to_string(this->getId())
    + "\nisBusy: " + (this->isBusy() ? "True" : "False")
    + "\nOrderID: " + (activeOrderId == -1 ? "None" : std::to_string(activeOrderId))
    + "\nTimeLeft: " + (this->getTimeLeft() == 0 ? "None" : std::to_string(this->getTimeLeft()))
    + "\nOrdersLeft: " + std::to_string(ordersLeft);
}

//DriverVolunteer

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer::Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}



int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    distanceLeft -= distancePerStep;
    return distanceLeft <= 0;
}

bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy() && order.getDistance() <= maxDistance;
}

void DriverVolunteer:: acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step()
{
   distanceLeft -= distancePerStep;
   if(distanceLeft <= 0)
   {
    completedOrderId = activeOrderId;
    activeOrderId = NO_ORDER;
   }
}

string DriverVolunteer::toString() const
{
    return "VolunteerID: " + std::to_string(this->getId())
    + "\nisBusy: " + (this->isBusy() ? "True" : "False")
    + "\nOrderID: " + (activeOrderId == -1 ? "None" : std::to_string(activeOrderId))
    + "\nTimeLeft: " + (distanceLeft == 0 ? "None" : std::to_string(distanceLeft))
    + "\nOrdersLeft: " + "No Limit";
}

//LimitedDriverVolunteer

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders): DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders), ordersLeft(maxOrders) {}


LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return ordersLeft != 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return DriverVolunteer::canTakeOrder(order) && ordersLeft != 0;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    DriverVolunteer::acceptOrder(order);
    ordersLeft -= 1; 
}

string LimitedDriverVolunteer::toString() const
{
    return "VolunteerID: " + std::to_string(this->getId())
    + "\nisBusy: " + (this->isBusy() ? "True" : "False")
    + "\nOrderID: " + (activeOrderId == -1 ? "None" : std::to_string(activeOrderId))
    + "\nTimeLeft: " + (this->getDistanceLeft() == 0 ? "None" : std::to_string(this->getDistanceLeft()))
    + "\nOrdersLeft: " + std::to_string(ordersLeft);
}


