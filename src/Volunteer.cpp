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
     // not implemented yet
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
    return "volunteer " + getName() + " collector " + std::to_string(getCoolDown());
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
    return ordersLeft != 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
     // not implemented yet
    return false;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    // not implemented yet
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
    return "volunteer " + getName() + " limited_collector " + std::to_string(getCoolDown()) + " " + std::to_string(maxOrders);
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
    return "volunteer " + getName() + " driver " + std::to_string(getMaxDistance()) + " " + std::to_string(getDistancePerStep());
}

//LimitedDriverVolunteer

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders): DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other): DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft) {}

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
     // not implemented yet
    return false;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
     // not implemented yet
}

string LimitedDriverVolunteer::toString() const
{
     return "volunteer " + getName() + " driver " + std::to_string(getMaxDistance()) + " " + std::to_string(getDistancePerStep()) + " " +
     std::to_string(maxOrders);
} 
