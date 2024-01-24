#include "Action.h"

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::getStatusToString() const
{
    switch (status)
    {
    case ActionStatus::COMPLETED:
        return "COMPLETED";
    case ActionStatus::ERROR:
        return "ERROR";
    }
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
SimulateStep::SimulateStep(const SimulateStep &other): numOfSteps(other.numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    //TO DO Figure out what to do
    complete();
}

string SimulateStep::toString() const
{
    return "simulateStep " + std::to_string(numOfSteps) + " " + getStatusToString();
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : customerId(id) {}

AddOrder::AddOrder(const AddOrder &other) : customerId(other.customerId) {}

void AddOrder::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    Customer &customer = wareHouse.getCustomer(customerId);
    if(customer.canMakeOrder())
    {
        int orderId = wareHouse.getOrdersCounter();
        customer.addOrder(orderId);
        Order *order = new Order(orderId, customerId, customer.getCustomerDistance());
        wareHouse.addOrder(order);
        complete();
    }
    else
    {
        this->error("Cannot place this order");
    }
}

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId) + " " + getStatusToString();
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : 
customerName(customerName),
 customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian),
  distance(distance),
   maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    Customer *customer = createCustomer(wareHouse.getCustomerCounter());
    wareHouse.addCustomer(customer);
    complete();
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
    return "addCustomer " + customerName + (customerType == CustomerType::Soldier ? "soldier" : "civilian") 
    + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " " + getStatusToString();
}

Customer *AddCustomer::createCustomer(int customerId)
{
    switch(customerType)
    {
        case CustomerType::Soldier:
            return new SoldierCustomer(customerId, customerName, distance, maxOrders);
        case CustomerType::Civilian:
            return new CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    Order &order = wareHouse.getOrder(orderId);
    std::cout << "OrderId: " + std::to_string(order.getId()) 
    + "\nOrderStatus: " + order.statusToString()
    + "\nCustomerID: " + std::to_string(order.getCustomerId()) 
    + "\nCollector: " + std::to_string(order.getCollectorId())
    + "\nDriver: " + std::to_string(order.getDriverId())<< std::endl;
    complete();
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "printOrderStatus " + std::to_string(orderId) + getStatusToString();
}

PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);
    std::cout << "CustomerID: " + std::to_string(customer.getId()) << std::endl;
    vector<int> orderIds = customer.getOrdersIds();
    for (int id : orderIds)
    {
        Order order = wareHouse.getOrder(id);
        std::cout << "OrderID: " + std::to_string(id)
        + "\nOrderStatus: " + order.statusToString() << std::endl;
    }
    std::cout << "numOrdersLeft: " + std::to_string(customer.getMaxOrders() - customer.getNumOrders()) << std::endl; 
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "printCustomerStatus: " + std::to_string(customerId) + " " + getStatusToString();
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) 
{
    
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return nullptr;
}

string PrintVolunteerStatus::toString() const
{
    return string();
}
