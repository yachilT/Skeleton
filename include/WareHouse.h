#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Order.h"
#include "Customer.h"

class Action; 
class Volunteer; 
#include "Action.h" // doifndef
#include "Volunteer.h"

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        int getOrdersCounter() const;
        int getCustomerCounter() const;//
        int getVolunteerCounter() const; // 
        void addCustomer(Customer *customer); // 
        void addVolunteer(Volunteer *v); //
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        //Destructor
        ~WareHouse();
        //Copy Constructor
        WareHouse(const WareHouse& other);
        // Copy Assignment Operator
        WareHouse& operator=(const WareHouse &other);



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
};