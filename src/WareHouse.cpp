#include "WareHouse.h"
#include <fstream>

WareHouse::WareHouse(const string &configFilePath): isOpen(false), customerCounter(0), volunteerCounter(0), ordersCounter(0), actionsLog(),volunteers(),
customers(), pendingOrders(), inProcessOrders(), completedOrders()
{
    std::ifstream file(configFilePath,std::ios::in);
    std::string row;
    while(getline(file,row))
    {
        row = WareHouse::subStringByChar(row,'#');
        if(row != "")
        {
            try 
            {
                vector<string> splitBySpace = WareHouse::splitString(row, ' ');
                if(splitBySpace[0] == "customer")
                {
                    BaseAction* action = new AddCustomer(splitBySpace[1], splitBySpace[2], std::stoi(splitBySpace[3]), std::stoi(splitBySpace[4]));
                    action->act(*this);
                }
                else if (splitBySpace[0] == "volunteer")
                {
                    if(splitBySpace[2] == "collector")
                        addVolunteer(new CollectorVolunteer(volunteerCounter,splitBySpace[1], std::stoi(splitBySpace[3])));
                    else if(splitBySpace[2] == "limited_collector")
                        addVolunteer(new LimitedCollectorVolunteer(volunteerCounter,splitBySpace[1], std::stoi(splitBySpace[3]), std::stoi(splitBySpace[4])));
                    else if(splitBySpace[2] == "driver")
                        addVolunteer(new DriverVolunteer(volunteerCounter,splitBySpace[1], std::stoi(splitBySpace[3]), std::stoi(splitBySpace[4])));    
                    else 
                        addVolunteer(new LimitedDriverVolunteer(volunteerCounter,splitBySpace[1], std::stoi(splitBySpace[3]), std::stoi(splitBySpace[4]), std::stoi(splitBySpace[5])));   
                }
            }
            catch(const std::exception &e){
            }
        }
    }
    deleteActionsLog();
    actionsLog = vector<BaseAction*>();
}

void WareHouse::start()
{
    isOpen = true;
    std::cout << "Warehouse is open!" << std::endl;
    while(isOpen)
    {
        string line;

        try 
        {
            getline(std::cin, line);
            vector<string> splitBySpace = splitString(line,' ');
            if(splitBySpace[0] == "close")
            {
                BaseAction *action = new Close();
                action->act(*this);
            }
            else if(splitBySpace[0] == "backup")
            {
                BaseAction *action = new BackupWareHouse();
                action->act(*this);
            }
            else if(splitBySpace[0] == "restore")
            {
                BaseAction *action = new RestoreWareHouse();
                action->act(*this);
            }
            else if(splitBySpace[0] == "order")
            {
                BaseAction *action =  new AddOrder(std::stoi(splitBySpace[1]));
                action->act(*this);
            }
            else if(splitBySpace[0] == "orderStatus")
            {
                BaseAction *action = new PrintOrderStatus(std::stoi(splitBySpace[1]));
                action->act(*this);
            }
            else if(splitBySpace[0] == "step")
            {
                BaseAction *action = new SimulateStep(std::stoi(splitBySpace[1]));
                action->act(*this);
            }
            else if(splitBySpace[0] == "volunteerStatus")
            {
                BaseAction *action = new PrintVolunteerStatus(std::stoi(splitBySpace[1]));
                action->act(*this);
            }
            else if(splitBySpace[0] == "customerStatus")
            {
                BaseAction *action = new PrintCustomerStatus(std::stoi(splitBySpace[1]));
                action->act(*this);
            }
            else if(splitBySpace[0] == "log")
            {
                BaseAction *action = new PrintActionsLog();
                action->act(*this);
            }
            // handle backup

            
        }
        catch(const std::exception &e){continue;}
    }
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

void WareHouse::assignOrders()
{
    for(auto iter = pendingOrders.begin(); iter < pendingOrders.end(); iter++)
    {
        Order* order = *iter;
        if (order->getStatus() == OrderStatus::PENDING)
        {
            CollectorVolunteer *collector = findAvailableCollector(*order);
            if (collector != nullptr)
            {
                order->setCollectorId(collector->getId());
                collector->acceptOrder(*order);
                pendingOrders.erase(iter--);
                inProcessOrders.push_back(order);
                order->setStatus(OrderStatus::COLLECTING);
            }
        }
        else if (order->getStatus() == OrderStatus::COLLECTING)
        {
            DriverVolunteer *driver = findAvailableDriver(*order);
            if (driver != nullptr)
            {
                order->setDriverId(driver->getId());
                driver->acceptOrder(*order);
                pendingOrders.erase(iter--);
                inProcessOrders.push_back(order);
                order->setStatus(OrderStatus::DELIVERING);
            }
        }
        

    }
}

void WareHouse::advanceTime()
{
    for (Volunteer* volunteer : volunteers)
    {
        volunteer->step();
    }
}

void WareHouse::pushOrders()
{
    for (auto iter = volunteers.begin(); iter < volunteers.end(); iter++)
    {

        Volunteer *volunteer = *iter;
        if (volunteer->hasCompleted())
        {
            Order *completedOrder = removeCompletedOrder(volunteer->getCompletedOrderId());
            volunteer->passOrder();
            if (completedOrder->getStatus() == OrderStatus::COLLECTING)
                pendingOrders.push_back(completedOrder);
            else if(completedOrder->getStatus() == OrderStatus::DELIVERING)
            {
                completedOrders.push_back(completedOrder);
                completedOrder->setStatus(OrderStatus::COMPLETED);
            }
        }
    }
}

void WareHouse::fireVolunteers()
{
    for (auto iter = volunteers.begin(); iter < volunteers.end(); iter++) 
    {
        Volunteer *volunteer = *iter;
        if (!volunteer->hasOrdersLeft())
        {
            volunteers.erase(iter--);
            delete volunteer;
        }
    }
}

void WareHouse::close()
{
    for(Order* order: pendingOrders)
    {
        std::cout << "OrderID: " + std::to_string(order->getId())
        + ", CustomerID: " + std::to_string(order->getCustomerId()) 
        + ", OrderStatus: " + order->statusToString()<< std::endl;
    }

    for (Order *order : inProcessOrders)
    {
        std::cout << "OrderID: " + std::to_string(order->getId())
        + ", CustomerID: " + std::to_string(order->getCustomerId()) 
        + ", OrderStatus: " + order->statusToString()<< std::endl;
    }

    for (Order *order : completedOrders)
    {
        std::cout << "OrderID: " + std::to_string(order->getId())
        + ", CustomerID: " + std::to_string(order->getCustomerId()) 
        + ", OrderStatus: " + order->statusToString()<< std::endl;
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

CollectorVolunteer* WareHouse::findAvailableCollector(Order &order)
{
    for(Volunteer *volunteer : volunteers)
    {
        if (volunteer->canTakeOrder(order) & dynamic_cast<CollectorVolunteer*>(volunteer) != nullptr)
        {
            return dynamic_cast<CollectorVolunteer*>(volunteer);
        }
        
    }
    return nullptr;
}

DriverVolunteer *WareHouse::findAvailableDriver(Order &order)
{
    for(Volunteer *volunteer : volunteers)
    {
        if (volunteer->canTakeOrder(order)& dynamic_cast<DriverVolunteer*>(volunteer) != nullptr)
            return dynamic_cast<DriverVolunteer*>(volunteer);
    }
    return nullptr;
}

Order *WareHouse::removeCompletedOrder(int id)
{
    auto iter = inProcessOrders.begin();
    Order *order = *iter;
    while (iter < inProcessOrders.end() && order->getId() != id)
    {
        iter++;
        order = *iter;
    }
    order = nullptr;
    if (iter != inProcessOrders.end()) 
    {
        order = *iter;
        inProcessOrders.erase(iter);
    }
    return order;

}

vector<string> WareHouse::splitString(const string& input, char delimiter) {
    vector<string> result;
    string token;

    for (char ch : input) {
        if (ch != delimiter) {
            token += ch;
        } else {
            result.push_back(token);
            token.clear();
        }
    }
    if (!token.empty()) {
        result.push_back(token);
    }

    return result;
}
string WareHouse::subStringByChar(const string &input, char c){
    string result;
    for(char ch: input)
    {
        if(ch == c)
            break;
        result += ch;
    }
    return result;
}