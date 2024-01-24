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

int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}

int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}

void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
    customerCounter += 1;
}

void WareHouse::addVolunteer(Volunteer *v)
{
    volunteers.push_back(v);
    volunteerCounter += 1;
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
    for(Volunteer* v: volunteers)
    {
        if(v->getId() == volunteerId)
            return *v;
    }
}

Order &WareHouse::getOrder(int orderId) const
{
    for(Order* o: pendingOrders)
    {
            if(o->getId() == orderId)
                return *o;
    }   
    for(Order* o: inProcessOrders)
    {
            if(o->getId() == orderId)
                return *o;
    }  
    for(Order* o: completedOrders)
    {
            if(o->getId() == orderId)
                return *o;
    }  
}


const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::close()
{
    for(BaseAction* action: actionsLog)
    {
        std::cout<< action->toString()<<std::endl;
    }
    isOpen = false;
}   


void WareHouse::open()
{
    isOpen = true;
}

WareHouse::~WareHouse()
{
    deleteActionsLog();
    deleteCustomers();
    deleteVolunteers();
    deleteOrders();
}

WareHouse::WareHouse(const WareHouse &other): isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), ordersCounter(other.ordersCounter) 
{
    
}

WareHouse &WareHouse::operator=(const WareHouse& other)
{
    if(this != &other)
    {
        deleteActionsLog();
        deleteCustomers();
        deleteOrders();
        deleteVolunteers();
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        ordersCounter = other.ordersCounter;
        actionsLog = other.actionsLog;
        customers = other.customers;
        volunteers = other.volunteers;
        pendingOrders = other.pendingOrders;
        inProcessOrders = other.inProcessOrders;
        completedOrders = other.completedOrders;
    }
}



void WareHouse::deleteActionsLog()
{
    while(!actionsLog.empty())
    {
        BaseAction *action = actionsLog.back();
        actionsLog.pop_back();
        delete action;
    }
}

void WareHouse::deleteVolunteers()
{
    while(!volunteers.empty())
    {
        Volunteer *v = volunteers.back();
        volunteers.pop_back();
        delete v;
    }
}

void WareHouse::deleteCustomers()
{
    while(!customers.empty())
    {
        Customer *c = customers.back();
        customers.pop_back();
        delete c;
    }
}

void WareHouse::deleteOrders()
{
    while(!pendingOrders.empty())
    {
        Order *o = pendingOrders.back();
        pendingOrders.pop_back();
        delete o;
    }
    while(!inProcessOrders.empty())
    {
        Order *o = inProcessOrders.back();
        inProcessOrders.pop_back();
        delete o;
    }
    while(!completedOrders.empty())
    {
        Order *o = completedOrders.back();
        completedOrders.pop_back();
        delete o;
    }
}
