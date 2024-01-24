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
SimulateStep::SimulateStep(const SimulateStep &other): numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    //TO DO Figure out what to do
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
    Order order = wareHouse.getOrder(orderId);
    std::cout << "OrderId: " + std::to_string(order.getId()) 
    + "\nOrderStatus: " + order.statusToString()
    + "\nCustomerID: " + std::to_string(order.getCustomerId()) 
    + "\nCollector: " + std::to_string(order.getCollectorId())
    + "\nDriver: " + std::to_string(order.getDriverId())<< std::endl;
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return nullptr;
}

string PrintOrderStatus::toString() const
{
    return string();
}
