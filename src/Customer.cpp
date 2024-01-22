#include "Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): 
id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId()
{};

Customer::Customer(const Customer &other): id(other.id), name(other.name), locationDistance(other.locationDistance), maxOrders(other.maxOrders){
    
};
const string& Customer::getName() const {
    return name;
};

int Customer::getId() const {
    return id;
};

int Customer::getCustomerDistance() const {
    return locationDistance;
};

int Customer::getMaxOrders() const {
    return maxOrders;
};

int Customer::getNumOrders() const {
    return ordersId.size();
}

bool Customer::canMakeOrder() const
{
    return maxOrders > getNumOrders();
}

const vector<int> &Customer::getOrders() const {
    return ordersId;
}
int Customer::addOrder(int orderId)
{
    if (!canMakeOrder()){
        return -1;
    }
    for (int i = 0; i < ordersId.size(); i++)
    {
        if (ordersId[i] == orderId)
            return -1;
    }
    ordersId.push_back(orderId);
    return orderId;
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};
SoldierCustomer::SoldierCustomer(const SoldierCustomer &other): Customer(other){};
SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
};


CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};
CivilianCustomer::CivilianCustomer(const CivilianCustomer &other): Customer(other){};
CivilianCustomer *CivilianCustomer::clone() const{
    return new CivilianCustomer(*this);
}
