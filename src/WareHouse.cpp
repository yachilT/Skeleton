#include "WareHouse.h"

WareHouse::WareHouse(const string &configFilePath)
{
}

void WareHouse::start()
{
}

int WareHouse::getOrdersCounter() const
{
    return ordersCounter;
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
    ordersCounter +=1;
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for(Customer* c : customers)
    {
        if(c->getId() == customerId)
            return *c;
    }
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for(int i = 0; i < volunteers.size(); i++)
    {
        if(*volunteers[i]->getId() == volunteerId)
            return *volunteers[i];
    }
}

Order &WareHouse::getOrder(int orderId) const
{
    // TODO: insert return statement here
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    // TODO: insert return statement here
}

void WareHouse::close()
{
}

void WareHouse::open()
{
}
