#include "Volunteer.h"
//Volunteer:
Volunteer::Volunteer(int id, const string &name): id(id),name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) {}
Volunteer::Volunteer(const Volunteer& other): id(other.id), name(other.name), completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId) {}
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
bool Volunteer::isBusy() const
{
    return activeOrderId == NO_ORDER && completedOrderId == NO_ORDER;
};
//CollectorVolunteer:

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id,name), coolDown(coolDown), timeLeft(0) {}
CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer& other): Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft) {}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
};
void CollectorVolunteer::step()
{

}
int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
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
    // not implemented yet
}
void CollectorVolunteer::acceptOrder(const Order &order)
{
    // not implemented yet
}
string CollectorVolunteer::toString() const
{
    // not implemented yet
    return "";
};
//LimitedCollectorVolunteer
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders): CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}
LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other): CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    return false;
}

//DriverVolunteer

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer::Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}
DriverVolunteer::DriverVolunteer(const DriverVolunteer &other): Volunteer(other), maxDistance(other.maxDistance),distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft) {}
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
    // not implemented yet
    return false;
}

void DriverVolunteer::acceptOrder(const Order &order)
{
    // not implemented yet
}

void DriverVolunteer::step()
{
    // not implemented yet
}

string DriverVolunteer::toString() const
{
    // not implemented yet
    return "";
}
