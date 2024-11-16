//
//  main.cpp
//  CS5800-Skiplist-Implimentation
//
//  Created by Logan Gill on 11/10/24.
//

#include <iostream>

using namespace std;

/// Represents Nodes, their fields, and their functions
class Nodes {
protected:
    int value;
public:
    /// A pointer to the Nodes above this Nodes
    Nodes* above;
    /// A pointer to the Nodes below this Nodes
    Nodes* below;
    /// A pointer to the Nodes left of this Nodes
    Nodes* left;
    /// A pointer to the Nodes right of this Nodes
    Nodes* right;
    /// Returns the value of the givin node. If the node is a Head node, throws a domain error
    virtual int getValue() = 0;
    /// Is this node a head node?
    virtual bool isHead() = 0;
    
protected:
    Nodes(int v): value(v), above(nullptr), below(nullptr), left(nullptr), right(nullptr)
    {}
};

/// Represents a (non-Head) node
class Node: public Nodes {
public:
    Node(int v): Nodes(v)
    {}
    
    int getValue() {
        return value;
    }
    
    bool isHead() {
        return false;
    }
};

/// Represents a head Node
class Head: public Nodes {
public:
    Head(): Nodes(0)
    {}
    
    int getValue() {
        throw domain_error("Head does not have a value");
    }
    
    bool isHead() {
        return true;
    }
};

/// A Skiplist data structure
class SkipList {
private:
    /// The head node of the Skiplist
    Head* head;
    
public:
    SkipList() {
        head = new Head();
        srand(time(NULL));
    }
    
    /// Is the given value present in the skiplist
    Node* find(int val) {
        Nodes* currPlace = head;
        while (true) {
            // If the value is already in the list, return true
            if (currPlace->right && currPlace->right->getValue() == val) {
                return static_cast<Node*>(currPlace->right);
            }
            
            // If val is greater than the value of the node right of this one, go right
            if (currPlace->right && currPlace->right->getValue() < val) {
                currPlace = currPlace->right;
                continue;
            }
            
            // If there is a node below, go down
            if (currPlace->below) {
                currPlace = currPlace->below;
                continue;
            }
            
            // If there is no node below, break out (and return null)
            break;
        }

        return nullptr;
    }
    
    /// Inserts the given val into the Skiplist
    void insert(int val) {
        Nodes* currPlace = this->head;
        //Node nn = Node(val);
        Node* newNode = new Node(val);
        
        while (true) {
            // If the value is already in the list, end function
            //if (currPlace->right && currPlace->right->getValue() == val) {
                //return;
            //}
            
            // If val is greater than the value of the node right of this one, go right
            if (currPlace->right && currPlace->right->getValue() < val) {
                currPlace = currPlace->right;
                continue;
            }
            
            // If there is a node below, go down
            if (currPlace->below) {
                currPlace = currPlace->below;
                continue;
            }
            
            // If there is no node below, insert node
            newNode->left = currPlace;
            newNode->right = currPlace->right;
            if (currPlace->right)
                currPlace->right->left = newNode;
            currPlace->right = newNode;
            break;
        }
        while (true) {
            // If the random number is even ("tails")
            if (rand() % 2 == 0) {
                break;
            }
            
            //Node en = Node(val);
            Node* extraNode = new Node(val);
            
            // While currPlace has no node above it and is not a head
            while (!(currPlace->above) && !(currPlace->isHead())) {
                currPlace = currPlace->left;
            }
            
            // If currPlace has a node above it
            if (currPlace->above) {
                currPlace = currPlace->above;
                extraNode->left = currPlace;
                extraNode->right = currPlace->right;
                if (currPlace->right)
                    currPlace->right->left = extraNode;
                currPlace->right = extraNode;
                
                extraNode->below = newNode;
                newNode->above = extraNode;
                newNode = extraNode;
                continue;
            }
            
            // If currPlace is a head (and has no node above it)
            if (currPlace->isHead()) {
                //Head nh = Head();
                Head* newHead = new Head();
                newHead->below = head;
                head->above = newHead;
                extraNode->left = newHead;
                newHead->right = extraNode;
                
                extraNode->below = newNode;
                newNode->above = extraNode;
                newNode = extraNode;
                
                head = newHead;
                currPlace = newHead;
                continue;
            }
        }
    }
    
    /// Remove all nodes with the given value
    void remove(int val) {
        Nodes* currPlace = head;
        while (true) {
            // If the value is already in the list, return true
            if (currPlace->right && currPlace->right->getValue() == val) {
                currPlace = currPlace->right;
                break;
            }
            
            // If val is greater than the value of the node right of this one, go right
            if (currPlace->right && currPlace->right->getValue() < val) {
                currPlace = currPlace->right;
                continue;
            }
            
            // If there is a node below, go down
            if (currPlace->below) {
                currPlace = currPlace->below;
                continue;
            }
            
            // If there is no node below, then value is not present
            return;
        }
        
        if (currPlace->getValue() == val) {
            while (true) {
                // If there will be no other items in the "row" other than the head after removal and there is something below
                //if (dynamic_cast<Head*>(currPlace->left) && !currPlace->right && currPlace->below) {
                    // Delete the row?
                //}
                
                currPlace->left->right = currPlace->right;
                if (currPlace->right)
                    currPlace->right->left = currPlace->left;
                currPlace->left = nullptr;
                currPlace->right = nullptr;
                
                // If there is more with the same value
                if (currPlace->below) {
                    currPlace = currPlace->below;
                    currPlace->above->below = nullptr;
                    currPlace->above = nullptr;
                    continue;
                }
                
                // If there is no more below, then there is no more needed to be removed
                break;
            }
        }
    }
    
    /// Displays the linked list
    void display() {
        Nodes* currPlace = head;
        Nodes* currHead = head;
        int dashLen = getDigitCount();
        cout << "--------";
        for (int i = 0; i < dashLen; ++i) {
            cout << "-";
        }
        cout << endl;
        int lineNum = 0;
        while (currPlace) {
            cout << "Line " << lineNum << ":";
            while (currPlace->right) {
                cout << " " << currPlace->right->getValue();
                currPlace = currPlace->right;
            }
            cout << endl;
            currHead = currHead->below;
            currPlace = currHead;
            ++lineNum;
        }
        cout << "--------";
        for (int i = 0; i < dashLen; ++i) {
            cout << "-";
        }
        cout << endl;
    }
    
    int getDigitCount() {
        Nodes* currPlace = head;
        int length = -1;
        int down = 0;
        while (currPlace->below) {
            ++down;
            currPlace = currPlace->below;
        }
        
        length += to_string(down).length() - 1;
        
        while (currPlace->right) {
            ++length;
            currPlace = currPlace->right;
            int val = currPlace->getValue();
            length += to_string(val).length();
        }
        
        return max(0, length);
    }
};

int main(int argc, const char * argv[]) {
    SkipList sl = SkipList();
    
    while (true) {
        cout << "Enter a command: ";
        string command;
        cin >> command;
        
        if (cin.fail()) {
            cout << "And error occured, please try again" << endl;
            continue;
        }
        
        if (command == "quit") {
            cout << "Prompt closed" << endl;
            break;
        } else if (command == "find") {
            int num;
            cin >> num;
            Node* n = sl.find(num);
            if (n) {
                cout << "Found " << n->getValue() << endl;
            } else {
                cout << num << " not found" << endl;
            }
        } else if (command == "insert") {
            int num;
            cin >> num;
            cout << "Inserting " << num << endl;
            sl.insert(num);
            sl.display();
        } else if (command == "delete") {
            int num;
            cin >> num;
            cout << "Removing " << num << endl;
            sl.remove(num);
            sl.display();
        } else if (command == "display") {
            sl.display();
        } else {
            cout << "Invalid command" << endl;
        }
    }
    return 0;
}
