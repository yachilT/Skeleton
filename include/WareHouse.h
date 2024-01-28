#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Order.h"
#include "Customer.h"

class BaseAction; 
#include "Action.h" 
#include "Volunteer.h"

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        int getOrdersCounter() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const; 
        void addCustomer(Customer *customer); 
        void addVolunteer(Volunteer *v); 
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        bool isCustomerExists(int id) const;
        bool isVolunteerExists(int id) const;
        bool isOrderExists(int id) const;
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void assignOrders();
        void advanceTime();
        void pushOrders();
        void fireVolunteers();
        void close();
        void open();
        //Destructor
        ~WareHouse();
        //Copy Constructor
        WareHouse(const WareHouse& other);
        //Copy Assignment Operator
        WareHouse& operator=(const WareHouse &other);
        //Move Constructor
        WareHouse(WareHouse&& other) noexcept;
        //Move Assignment
        WareHouse& operator=(WareHouse&& other) noexcept;




    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int ordersCounter; //For assigning unique orders IDS
        void deleteActionsLog();
        void deleteVolunteers();
        void deleteCustomers();
        void deleteOrders();
        CollectorVolunteer* findAvailableCollector(Order &order);
        DriverVolunteer* findAvailableDriver(Order &order);
        Order *removeCompletedOrder(int id);
        vector<string> splitString(const string& input, char delimiter);
};
