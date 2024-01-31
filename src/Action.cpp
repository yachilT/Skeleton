#include "Action.h"

extern WareHouse* backup;

BaseAction::BaseAction() : errorMsg(), status(ActionStatus::COMPLETED) {}

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
    std::cout << "Error: " + errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::getStatusToString() const
{
    return status == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR";
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
SimulateStep::SimulateStep(const SimulateStep &other): numOfSteps(other.numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    for (int i = 0; i < numOfSteps; i++)
    {
        wareHouse.assignOrders();
        wareHouse.advanceTime();
        wareHouse.pushOrders();
        wareHouse.fireVolunteers();
    }
    
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
    if (!wareHouse.isCustomerExists(customerId))
        error("Customer doesn't exist");
    else
    {
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
    wareHouse.addAction(this);
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
    if (customerType == CustomerType::Soldier)
        return new SoldierCustomer(customerId, customerName, distance, maxOrders);
    else
        return new CivilianCustomer(customerId, customerName, distance, maxOrders);
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (!wareHouse.isOrderExists(orderId))
        error("Order doesn't exist");
    else
    {
        Order &order = wareHouse.getOrder(orderId);
        std::cout << order.toString() << std::endl;
        complete();
        
    }
    wareHouse.addAction(this);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId) + " " + getStatusToString();
}

PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (!wareHouse.isCustomerExists(customerId))
        error("Customer doesn't exist");
    else
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
    wareHouse.addAction(this);
} 

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus: " + std::to_string(customerId) + " " + getStatusToString();
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) 
{
    if (!wareHouse.isVolunteerExists(volunteerId))
    {
        error("Volunteer doesn't exist");
    }
    else
    {
        Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
        std::cout << volunteer.toString() << std::endl;
        complete();
    }
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId) + " " + getStatusToString();
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    for (BaseAction *baseAction : wareHouse.getActions()) 
    {
        std::cout << baseAction->toString() << std::endl;
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log " + getStatusToString();
}

Close::Close() {}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
    complete();
    wareHouse.addAction(this);
}

Close *Close::clone() const
{
    return new Close(*this);
}

string Close::toString() const
{
    return "close " + getStatusToString();
}

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
    backup = new WareHouse(wareHouse);
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backup " + getStatusToString();
}

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
        error("No backup available");
    else
    {
        wareHouse = WareHouse(*backup);
        complete();
    }
    wareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restore " + getStatusToString();
}
