#include "Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): 
id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId()
{};
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

const vector<int> &Customer::getOrdersIds() const {
    return ordersId;
}
int Customer::addOrder(int orderId)
{
    if (!canMakeOrder()){
        return -1;
    }
    for (size_t i = 0; i < ordersId.size(); i++)
    {
        if (ordersId[i] == orderId)
            return -1;
    }
    ordersId.push_back(orderId);
    return orderId;
}


SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};
SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
};


CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders){};
CivilianCustomer *CivilianCustomer::clone() const{
    return new CivilianCustomer(*this);
}
