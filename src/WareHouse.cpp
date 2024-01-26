#include "WareHouse.h"

WareHouse::WareHouse(const string &configFilePath)
{
    
}

void WareHouse::start()
{
    isOpen = true;
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

bool WareHouse::isCustomerExists(int id) const
{
    for(Customer* customer: customers)
        if(customer->getId() == id)
            return true;
    return false;
}

bool WareHouse::isVolunteerExists(int id) const
{
    for(Volunteer* volunteer: volunteers)
        if(volunteer->getId() == id)
            return true;
    return false;
}

bool WareHouse::isOrderExists(int id) const
{
    for(Order* order: pendingOrders)
        if(order->getId() == id)
            return true;
    for(Order* order: inProcessOrders)
        if(order->getId() == id)
            return true;
    for(Order* order: completedOrders)
        if(order->getId() == id)
            return true;
    
    return false;
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
    for(BaseAction *action: other.actionsLog)
        actionsLog.push_back(action->clone());
    
    for(Volunteer *volunteer: other.volunteers)
        volunteers.push_back(volunteer->clone());
    
    for(Customer *customer: other.customers)    
        customers.push_back(customer->clone());
    
    for(Order *order: other.pendingOrders)
        pendingOrders.push_back(order->clone());
    
    for(Order *order: other.inProcessOrders)
        inProcessOrders.push_back(order->clone());
    
    for(Order *order: other.completedOrders)
        completedOrders.push_back(order->clone());
    
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
        for(BaseAction *action: other.actionsLog)
            actionsLog.push_back(action->clone());
        
        for(Volunteer *volunteer: other.volunteers)
            volunteers.push_back(volunteer->clone());
        
        for(Customer *customer: other.customers)    
            customers.push_back(customer->clone());
        
        for(Order *order: other.pendingOrders)
            pendingOrders.push_back(order->clone());
        
        for(Order *order: other.inProcessOrders)
            inProcessOrders.push_back(order->clone());
        
        for(Order *order: other.completedOrders)
            completedOrders.push_back(order->clone());
    }
}

WareHouse::WareHouse(WareHouse&& other) noexcept: isOpen(other.isOpen), actionsLog(other.actionsLog), volunteers(other.volunteers),
pendingOrders(other.pendingOrders), inProcessOrders(other.inProcessOrders), completedOrders(other.completedOrders), customers(other.customers),
customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), ordersCounter(other.ordersCounter)
{
    for (int i = 0; i < other.actionsLog.size(); i++)
        other.actionsLog[i] = nullptr;

    for (int i = 0; i < other.volunteers.size(); i++)
        other.volunteers[i] = nullptr;

    for (int i = 0; i < other.customers.size(); i++)
        other.customers[i] = nullptr;

    for (int i = 0; i < other.pendingOrders.size(); i++)
        other.pendingOrders[i] = nullptr;

    for (int i = 0; i < other.inProcessOrders.size(); i++)
        other.inProcessOrders[i] = nullptr;

    for (int i = 0; i < other.completedOrders.size(); i++)
        other.completedOrders[i] = nullptr;
}

WareHouse &WareHouse::operator=(WareHouse &&other) noexcept
{
    if(this != &other)
    {
        deleteActionsLog();
        deleteCustomers();
        deleteVolunteers();
        deleteOrders();
        for (int i = 0; i < other.actionsLog.size(); i++)
        {   
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
        }
        for (int i = 0; i < other.customers.size(); i++)
        {   
            customers.push_back(other.customers[i]);
            other.customers[i] = nullptr;
        }
        for (int i = 0; i < other.volunteers.size(); i++)
        {   
            volunteers.push_back(other.volunteers[i]);
            other.volunteers[i] = nullptr;
        }
        for (int i = 0; i < other.pendingOrders.size(); i++)
        {   
            pendingOrders.push_back(other.pendingOrders[i]);
            other.pendingOrders[i] = nullptr;
        }
        for (int i = 0; i < other.inProcessOrders.size(); i++)
        {   
            inProcessOrders.push_back(other.inProcessOrders[i]);
            other.inProcessOrders[i] = nullptr;
        }
        for (int i = 0; i < other.completedOrders.size(); i++)
        {   
            completedOrders.push_back(other.completedOrders[i]);
            other.completedOrders[i] = nullptr;
        }
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        ordersCounter = other.ordersCounter;
    }
    return *this;
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

std::vector<std::string> WareHouse::splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::string token;

    for (char ch : input) {
        if (ch != delimiter) {
            token += ch;
        } else {
            result.push_back(token);
            token.clear();
        }
    }

    // Add the last token if the input doesn't end with the delimiter
    if (!token.empty()) {
        result.push_back(token);
    }

    return result;
}
