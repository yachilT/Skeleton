#include "Order.h"

Order::Order(int id, int customerId, int distance) :
 id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
{
}

int Order::getId() const
{
    return id;
}

int Order::getCustomerId() const
{
    return customerId;
}

void Order::setStatus(OrderStatus status)
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    this->driverId = driverId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

const string Order::toString() const
{
    return "OrderId: " + std::to_string(id)
        + "\nOrderStatus: " + statusToString()
        + "\nCustomerID: " + std::to_string(customerId) 
        + "\nCollector: " + (collectorId == NO_VOLUNTEER ? "None" : std::to_string(collectorId))
        + "\nDriver: " + (driverId == NO_VOLUNTEER ? "None" : std::to_string(driverId));
}

int Order::getDistance() const
{
    return distance;
}

const string Order::statusToString() const
{
    switch(status){
        case OrderStatus::PENDING:
            return "Pending";
        case OrderStatus::COLLECTING:
            return "Collecting";
        case OrderStatus::DELIVERING:
            return "Delivering";
        case OrderStatus::COMPLETED:
            return "Completed";
        default:
            return "";
    }
}

Order *Order::clone()
{
    return new Order(*this);
}
