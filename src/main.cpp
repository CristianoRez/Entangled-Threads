/**********************************************************************************
 * 
 * FILE:            main.cpp
 * 
 * ********************************************************************************
 *
 * PROJECT:         Project Name (Entangled Threads Structure Algorithm)
 * 
 * ********************************************************************************
 *
 * DESCRIPTION:
 * The Entangled Threads Structure (ETS) comprises multiple dimensions, each 
 * represented as a list, and multiple objects, each modeled as a node, capable of 
 * occupying more than one dimension.
 * 
 * The underlying idea is that modifications to an object in one dimension are
 * reflected across the others in constant time, as they all reference the same 
 * object.
 * 
 * The ETS, was adapted to project-specific requirements. 
 * It optimally addresses the problem outlined below.
 *
 * Problem: Logistics System
 * There are two types of query commands:
 * – Given a package, the system must be able to return all events related to that
 *   package.
 * – Given a customer, the system must be able to return the first and last event of
 *   each package associated with that customer, in chronological order.
 *
 * Solution:
 * The data structure achieves the above by:
 * 
 * Hash tables were used to access a package or a customer (entities) in constant time. 
 * Since the events associated with these entities are only read sequentially, a list
 * was chosen as the storage structure. 
 * At this point, the data structure resembles the diagram below:
 *  _
 * |P|-----------
 * | |
 * |P|--------
 * | |
 * |P|------------
 * |P|--
 * | |
 * |P|------
 * |_|
 *  _
 * | |
 * |C|------------
 * |C|--------
 * | |
 * |C|-----
 * |C|---------
 * | |
 * |C|-
 * |_|
 * 
 * The problem is that, although events are read sequentially, when updating the 
 * last event related to a package (by simply appending it to the end of the 
 * package’s list), it is also necessary to update the corresponding last event in 
 * the lists of the customers associated with that package. 
 * To maintain the correct chronological order in the customer’s list, one would 
 * need to remove the outdated event and append the new one to the end of the list. 
 * However, the outdated event may currently reside at any position within the list, 
 * making constant-time removal difficult — and this is precisely where the 
 * Entangled Threads Structure comes into play.
 * 
 * Each event is, in fact, a N-dimensional node object, simultaneously present 
 * in both the package list and the clients lists. 
 * Thus, when a new node is to be appended to the end of the package list, it is 
 * already possible — via the tail of the list — to access the node that must be 
 * removed from the clients lists. This operation can be efficiently performed 
 * provided that the list is implemented as a doubly linked list.
 * 
 * This implementation requires a simple mechanism that enables the node to move 
 * within each of the defined dimensions. Further details are provided in the file 
 * dimension_node.hpp.
 * 
 * Up to this point, the data structure is expected to resemble a configuration of 
 * entangled threads, reflecting the complex interconnections among its components.
 *  
 * The current file (main.cpp) contains not only the input and output code, but 
 * also the full implementation of the node movement logic within the list update
 * functions.
 * --------------------------------------------------------------------------------
 * Commands:
 * CL - Prints the first and last events related to a given customer
 * PC - Prints all events related to a given package
 * RG - Registers a new package in the system and stores the "Registration" event
 * AR - Stores the "Store" event
 * RM - Stores the "Remove" event
 * UR - Stores the "Restore" event
 * TR - Stores the "Transport" event
 * EN - Stores the "Delivery" event
 * 
 * ********************************************************************************
 *
 * COPYRIGHT NOTICE:
 * Copyright (c) 2025 Cristiano Rezende Sá Miranda Gonçalves
 * 
 * This software is released under the MIT License.
 * You should have received a copy of the MIT License along with this program. 
 * If not, see <https://opensource.org/license/mit>.
 * 
 * SPDX-License-Identifier: MIT
 * 
 * ********************************************************************************
 *
 * REVISION HISTORY:
 * --------------------------------------------------------------------------------
 * | Version |   Date   |    Author     |               Description
 * --------------------------------------------------------------------------------
 * |  1.0.0  | 08/07/25 |   Cristiano   |  - Implementation of the Entangled 
 * |                    |    Rezende    |    Threads Structure Algorithm in a 
 * |                                    |    Logistic System
 * --------------------------------------------------------------------------------
 *
 **********************************************************************************/

#include "array_list.hpp"
#include "hash.hpp"
#include "dimension_node.hpp"
#include "linked_list.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>





struct PackageData {
    std::string sender;
    std::string recipient;
    LinkedList<int> events;
};




void handleActionCL(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, int i);
void handleActionRG(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionAR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionRM(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionUR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionTR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionEN(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList);
void handleActionPC(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<int, PackageData>& packages, int i);
void updateLists(int i, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int packageID, LinkedList<int>& DNodesList);
void updateCustomerList(std::string customer, DimensionNode<int>* DNode, DimensionNode<int>* newDNode, int eventsSize, LinkedList<int>* customerList);
void updateDeleteList(LinkedList<int>& DNodesList, DimensionNode<int>* newDNode); 
void updatePackageList(PackageData* packageData, DimensionNode<int>* newDNode);




int main(int argc, char* argv[]) {
    ArrayList<std::string> logs(1000);
    Hash<std::string, LinkedList<int>> customers(1000);
    Hash<int, PackageData> packages(1000);
    LinkedList<int> DNodesList;
    DimensionNode<int>* DNode = new DimensionNode<int>;
    DNodesList.head = DNode; 
    DNodesList.tail = DNode; 
    
    int time;
    std::string command;
    std::string action;

    if (argc < 2) {
        std::cerr << "Error: no text file!" << std::endl;
        return 1; 
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open file: '" << argv[1] << "'" << std::endl;
        return 1;
    }
    
    for (int i = 0; inputFile >> time; i++) {
        inputFile >> command;

        if (command == "CL") {
            handleActionCL(time, inputFile, logs, customers, i);
        } 
        else if (command == "EV") {
            inputFile >> action;

            if (action == "RG") {
                handleActionRG(time, inputFile, logs, customers, packages, i, DNodesList);
            } else if (action == "AR") {
                handleActionAR(time, inputFile, logs, customers, packages, i, DNodesList);
            } else if (action == "RM") {
                handleActionRM(time, inputFile, logs, customers, packages, i, DNodesList);
            } else if (action == "UR") {
                handleActionUR(time, inputFile, logs, customers, packages, i, DNodesList);
            } else if (action == "TR") {
                handleActionTR(time, inputFile, logs, customers, packages, i, DNodesList);
            } else if (action == "EN") {
                handleActionEN(time, inputFile, logs, customers, packages, i, DNodesList);
            }
        } 
        else if (command == "PC") {
            handleActionPC(time, inputFile, logs, packages, i);
        }
    }

    DimensionNode<int>* current = DNodesList.head;
    DimensionNode<int>* temp = current;
    while (current != nullptr) {
        temp = current->dimension["delete"].next;
        delete current;
        current = temp;
    }

    return 0;
}




void handleActionRG(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, originWarehouseId, destinationWarehouseId;
    std::string sender, recipient;
    
    inputFile 
        >> packageId 
        >> sender 
        >> recipient 
        >> originWarehouseId 
        >> destinationWarehouseId;

    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV RG " 
        << std::setw(3) << packageId << " " 
        << sender << " " 
        << recipient << " " 
        << std::setw(3) << originWarehouseId << " " 
        << std::setw(3) << destinationWarehouseId;
    logs.insertAtEnd(logStream.str());

    PackageData* packageData = &packages[packageId];
    packageData->sender = sender;
    packageData->recipient = recipient;
    
    DimensionNode<int>* newDNode = new DimensionNode<int>;
    newDNode->item = i;

    updateCustomerList(sender, nullptr, newDNode, packageData->events.getSize(), &customers[sender]);
    
    updateCustomerList(recipient, nullptr, newDNode, packageData->events.getSize(), &customers[recipient]);
    
    updatePackageList(packageData, newDNode);

    updateDeleteList(DNodesList, newDNode);
}




void handleActionAR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, destinationWarehouseId, targetSection;

    inputFile 
        >> packageId 
        >> destinationWarehouseId 
        >> targetSection;
    
    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV AR "
        << std::setw(3) << packageId << " " 
        << std::setw(3) << destinationWarehouseId << " " 
        << std::setw(3) << targetSection;
    logs.insertAtEnd(logStream.str());

    updateLists(i, customers, packages, packageId, DNodesList);
}




void handleActionRM(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, destinationWarehouseId, targetSection;

    inputFile 
        >> packageId 
        >> destinationWarehouseId 
        >> targetSection;
    
    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV RM "
        << std::setw(3) << packageId << " " 
        << std::setw(3) << destinationWarehouseId << " " 
        << std::setw(3) << targetSection;
    logs.insertAtEnd(logStream.str());

    updateLists(i, customers, packages, packageId, DNodesList);
}




void handleActionUR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, destinationWarehouseId, targetSection;

    inputFile 
        >> packageId 
        >> destinationWarehouseId 
        >> targetSection;
    
    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV UR "
        << std::setw(3) << packageId << " " 
        << std::setw(3) << destinationWarehouseId << " " 
        << std::setw(3) << targetSection;
    logs.insertAtEnd(logStream.str());

    updateLists(i, customers, packages, packageId, DNodesList);
}




void handleActionTR(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, originWarehouseId, destinationWarehouseId;

    inputFile 
    >> packageId 
    >> originWarehouseId 
    >> destinationWarehouseId;
    
    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV TR "
        << std::setw(3) << packageId << " " 
        << std::setw(3) << originWarehouseId << " " 
        << std::setw(3) << destinationWarehouseId;
    
    logs.insertAtEnd(logStream.str());
    
    updateLists(i, customers, packages, packageId, DNodesList);
}




void handleActionEN(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int i, LinkedList<int>& DNodesList) {
    int packageId, destinationWarehouseId;

    inputFile 
        >> packageId 
        >> destinationWarehouseId;

    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(7) << time << " EV EN " 
        << std::setw(3) << packageId << " " 
        << std::setw(3) << destinationWarehouseId;
    logs.insertAtEnd(logStream.str());

    updateLists(i, customers, packages, packageId, DNodesList);
}




void handleActionCL(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<std::string, LinkedList<int>>& customers, int i) {
    std::string customerName;
    inputFile >> customerName;

    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(6) << time << " CL " 
        << customerName;
    logs.insertAtEnd(logStream.str());

    LinkedList<int>* customerPackages = &customers[customerName];
    
    std::cout 
    << logs[i] << std::endl
    << customerPackages->getSize() << std::endl;
    
    if ( customerPackages->getSize() < 1) return;
    else {
        DimensionNode<int>* DNode = customerPackages->head;
        do {
            std::cout << logs[(*DNode).item] << std::endl;
            DNode = DNode->dimension[customerName].next;
        } while (DNode != nullptr); 
    }
};




void handleActionPC(int time, std::ifstream& inputFile, ArrayList<std::string>& logs, Hash<int, PackageData>& packages, int i) {
    int packageId;

    inputFile >> packageId;

    std::stringstream logStream;
    logStream 
        << std::setfill('0')
        << std::setw(6) << time << " PC " 
        << std::setw(3)<< packageId;
    logs.insertAtEnd(logStream.str());

    LinkedList<int>* packageEvents = &packages[packageId].events;

    std::cout 
        << logs[i] << std::endl
        << packageEvents->getSize() << std::endl;

    if ( packageEvents->getSize() < 1) return;
    else {
        DimensionNode<int>* DNode = packageEvents->head;
        do {
            std::cout << logs[DNode->item] << std::endl;
            DNode = DNode->dimension["package"].next;
        } while (DNode != nullptr); 
    }
};




void updateLists(int i, Hash<std::string, LinkedList<int>>& customers, Hash<int, PackageData>& packages, int packageId, LinkedList<int>& DNodesList) {
    // Package List
    PackageData* packageData = &packages[packageId];
    // Last event
    DimensionNode<int>* DNode = packageData->events.tail;
    // New event
    DimensionNode<int>* newDNode = new DimensionNode<int>;
    newDNode->item = i;
    
    updateDeleteList(DNodesList, newDNode);

    updateCustomerList(packageData->sender, DNode, newDNode, packageData->events.getSize(), &customers[packageData->sender]);
    
    updateCustomerList(packageData->recipient, DNode, newDNode, packageData->events.getSize(), &customers[packageData->recipient]);
    
    updatePackageList(packageData, newDNode);
};




void updateCustomerList(std::string customer, DimensionNode<int>* DNode, DimensionNode<int>* newDNode, int packageListEventPosition, LinkedList<int>* customerList) {    
    if (customerList->tail == nullptr) { // Customer list is empty
        // Add newDNode as the 1st element of the list
        customerList->head = newDNode;
        customerList->size = 1;
    } else if (packageListEventPosition <= 1) { // 1st or 2nd event of a package
        // As the 1st event cannot be deleted, in both conditions the new event is added to the end of the list
        customerList->tail->dimension[customer].next = newDNode;
        newDNode->dimension[customer].prev = customerList->tail;
        customerList->size++;
    } else {
        if (DNode->dimension[customer].next == nullptr) {
            // The new DNode replaces the old one
            customerList->tail->dimension[customer].prev->dimension[customer].next = newDNode;
            newDNode->dimension[customer].prev = customerList->tail->dimension[customer].prev;
        } else { // Has a subsequent DNode
            // It is necessary to delete the old DNode from the list and add the new one at the end;
            DNode->dimension[customer].prev->dimension[customer].next = DNode->dimension[customer].next;
            DNode->dimension[customer].next->dimension[customer].prev = DNode->dimension[customer].prev;
            customerList->tail->dimension[customer].next = newDNode;
            newDNode->dimension[customer].prev = customerList->tail; 
        }
    }
    customerList->tail = newDNode;
};




void updateDeleteList(LinkedList<int>& DNodesList, DimensionNode<int>* newDNode) {
    // Add as last element
    DNodesList.tail->dimension["delete"].next = newDNode;
    newDNode->dimension["delete"].prev = DNodesList.tail;
    DNodesList.tail = newDNode;
    DNodesList.size++;
};




void updatePackageList(PackageData* packageData, DimensionNode<int>* newDNode) {
    if (packageData->events.getSize() == 0) {
        // Add as 1st element
        packageData->events.head = newDNode;
        packageData->events.tail = packageData->events.head;
        packageData->events.size = 1;
    } else {
        // Add as last element
        packageData->events.tail->dimension["package"].next = newDNode;
        newDNode->dimension["package"].prev = packageData->events.tail;
        packageData->events.tail = newDNode;
        packageData->events.size++;
    }
};