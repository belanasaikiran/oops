#include <iostream>
#include <string>
using namespace std;

// Character class for the adventure game
class Character {
public:
    string name;
    int level;
    int experience;
    int health;

    // Constructor
    Character(string n, int l = 1, int exp = 0, int hp = 100)
        : name(n), level(l), experience(exp), health(hp) {}

    // Display character information
    void displayInfo() const {
        cout << "Character: " << name << " | Level: " << level
             << " | Experience: " << experience << " | Health: " << health << endl;
    }
};

// Node structure for linked list (inventory management)
struct Node {
    string itemName;  // The data
    Node* next;       // The pointer to the next node

    // Constructor for easier node creation
    Node(string item, Node* nextNode = nullptr) : itemName(item), next(nextNode) {}
};

// ========================================
// STEP 1: Function Parameter Passing with References
// ========================================

// TODO: Implement levelUp function
// This function should take a Character reference and:
// - Add 100 to experience
// - Increment level by 1
// - Display a level up message
void levelUp(Character &c) {
  c.level++;
  cout << "Character " << c.name << " has been leveled up to " << c.level << endl;
  // TODO: Implement function body
  // Hint: Use reference parameter to avoid copying the Character object
}

// TODO: Implement healCharacter function
// This function should take a Character reference and healing amount:
// - Add healing amount to health
// - Ensure health doesn't exceed a maximum (e.g., 100)
// - Display healing message
void healCharacter(Character& c, int healingAmount) {
  // TODO: Implement function body
  int oldHealth = c.health;
  if (c.health + healingAmount <= 100) c.health += healingAmount;
  else c.health = 100;

  std::cout << "Character " << c.name << " has been healed for " << c.health - oldHealth << " points! Health: " << c.health << endl;
}

// ========================================
// STEP 2: Data Structure Navigation with Pointers
// ========================================

// TODO: Implement addItem function
// This function should add an item to the front of the linked list
// Parameters: Node*& head (reference to pointer), const string& item
void addItem(Node *&head, const std::string& item) {
  Node *newNode = new Node(item, head);
  head = newNode;
    // TODO: Create new node with the item
    // TODO: Set the new node's next pointer to current head
    // TODO: Update head to point to new node
    // Hint: Node* newNode = new Node(item, head);
}

// TODO: Implement displayInventory function
// This function should traverse and display all items in the linked list
void displayInventory(Node* &head) {
  Node *temp = head;
  if(temp == nullptr) return;
  std::cout << "Items: ";
  while (temp != nullptr) {
    cout << temp->itemName;
    if(temp->next) cout << " -> ";
    temp = temp->next;
  }
  cout << endl;
  // TODO: Check if inventory is empty
    // TODO: Traverse the linked list using a pointer
    // TODO: Display each item name
    // Hint: Use a while loop with current->next
}

// TODO: Implement removeItem function
// This function should remove the first occurrence of an item from the list
// Return true if item was found and removed, false otherwise
bool removeItem(Node *head, const std::string deleteString) {
  if(!head || deleteString.empty()) return false;
  Node *temp = head;
  Node *prev = nullptr;
  while (temp != nullptr) {
    if (temp->itemName == deleteString) {
      prev->next = temp->next;
      delete temp;
      temp = nullptr;
      return true;
    }
    prev = temp;
    temp = temp->next;
  }

  return false;
    // TODO: Handle empty list case
    // TODO: Handle case where first item matches
    // TODO: Traverse list to find item
    // TODO: Update pointers to remove node
    // TODO: Delete the removed node
    // TODO: Return appropriate boolean value
}

// ========================================
// STEP 3: Efficient Memory Usage - Dynamic Allocation
// ========================================

// TODO: Implement createScoreArray function
// This function should dynamically allocate an array of integers
// Parameters: int initialSize
// Return: pointer to the allocated array
int *createScoreArray(int initialSize) {
  int *newArr = new int[initialSize]();
  return newArr;

    // TODO: Allocate array using new
    // TODO: Initialize all elements to 0
    // TODO: Return pointer to array
    // Hint: int* scores = new int[initialSize];
}

// TODO: Implement displayScores function
// This function should display all scores in the array
// Parameters: int* scores, int size
void displayScores(int *scores, int size) {
  int *temp = scores;
  cout << "scores: [";
  for (int i = 0; i < size; i++) {
    cout << *temp;
    if (i < size - 1) {
         cout << ", ";
    }
    temp++;
  }
  cout << "]\n";
    // TODO: Check if scores pointer is valid
    // TODO: Loop through array and display each score
}

// TODO: Implement updateScore function
// This function should safely update a score at given index
// Parameters: int* scores, int size, int index, int newScore
// Return: true if update successful, false if invalid index
bool updateScore(int *scores, int size, int index, int newScore) {
  if (!scores)
    return false;
  int *temp = scores;
  if (index < size) {
      temp += index;
      cout << "updating " << "scores[" << index << "] from " << *temp << " to " << newScore << endl;
      *temp = newScore;
      return true;
  }

  return false;
    // TODO: Validate parameters (scores not null, valid index)
    // TODO: Update score at index
    // TODO: Return success/failure status
}

// ========================================
// STEP 4: Safety and Error Prevention
// ========================================

// TODO: Implement safeDeleteArray function
// This function should safely delete a dynamically allocated array
void safeDeleteArray(int* &arr) {
  cout << "deleting array " << std::endl;
  if (!arr)
    return;
  delete[] arr;
  arr = nullptr;
  // TODO: Check if pointer is not null
    // TODO: Delete the array using delete[]
    // TODO: Set pointer to nullptr (passed by reference)
}

// TODO: Implement clearInventory function
// This function should safely delete all nodes in the linked list
void clearInventory(Node *&head) {
  std::cout << "Inventory has been cleaned!" << endl;
  Node *temp = head;
  if (!temp)
    return;
  while (temp) {
    Node *node = temp;
    temp = temp->next;
    delete node;
    node = nullptr;
  }

  head = nullptr;
  // TODO: Create temporary pointer
    // TODO: Loop through list
    // TODO: Store current node in temp
    // TODO: Move to next node
    // TODO: Delete temp node
    // TODO: Set head to nullptr when done
}

// ========================================
// MAIN FUNCTION - Testing Ground
// ========================================

int main() {
    cout << "=== Adventure Game Memory Management System ===" << endl;

    // Test Step 1: Function Parameter Passing with References
    cout << "\n--- Step 1: Character Management with References ---" << endl;
    Character hero("Aragorn", 1, 0, 80);
    hero.displayInfo();

    // TODO: Test levelUp function
    cout << "Leveling up character..." << endl;
    levelUp(hero);
    hero.displayInfo();

    // TODO: Test healCharacter function
    cout << "Healing character..." << endl;
    healCharacter(hero, 20);
    hero.displayInfo();

    // Test Step 2: Inventory Management with Linked List
    cout << "\n--- Step 2: Inventory Management with Pointers ---" << endl;
    Node* inventory = nullptr;  // Empty inventory initially

    // TODO: Test addItem function
    cout << "Adding items to inventory..." << endl;
    addItem(inventory, "Health Potion");
    addItem(inventory, "Magic Sword");
    addItem(inventory, "Shield");

    // TODO: Test displayInventory function
    cout << "Current inventory:" << endl;
    displayInventory(inventory);

    // TODO: Test removeItem function
    cout << "Removing 'Health Potion' from inventory..." << endl;
    if (removeItem(inventory, "Health Potion")) {
        cout << "Item removed successfully!" << endl;
    } else {
        cout << "Item not found!" << endl;
    }
    displayInventory(inventory);

    // Test Step 3: Dynamic Memory for Game Stats
    cout << "\n--- Step 3: Dynamic Score Management ---" << endl;
    const int numScores = 5;

    // TODO: Test createScoreArray function
    int* gameScores = createScoreArray(numScores);

    // TODO: Test updateScore function
    updateScore(gameScores, numScores, 0, 1500);
    updateScore(gameScores, numScores, 2, 2300);
    updateScore(gameScores, numScores, 4, 1800);

    // TODO: Test displayScores function
    cout << "Game scores:" << endl;
    displayScores(gameScores, numScores);

    // Test Step 4: Cleanup and Safety
    cout << "\n--- Step 4: Safe Memory Cleanup ---" << endl;

    // TODO: Test safeDeleteArray function
    safeDeleteArray(gameScores);

    // TODO: Test clearInventory function
    clearInventory(inventory);

    cout << "All memory cleaned up safely!" << endl;
    cout << "\n=== Adventure Game Demo Complete ===" << endl;

    return 0;
}

/*
IMPLEMENTATION CHECKLIST:
□ Step 1: levelUp function implemented
□ Step 1: healCharacter function implemented
□ Step 2: addItem function implemented
□ Step 2: displayInventory function implemented
□ Step 2: removeItem function implemented
□ Step 3: createScoreArray function implemented
□ Step 3: displayScores function implemented
□ Step 3: updateScore function implemented
□ Step 4: safeDeleteArray function implemented
□ Step 4: clearInventory function implemented
□ All test calls in main() uncommented
□ Program compiles and runs without errors
□ No memory leaks (all new matched with delete)

SAFETY REMINDERS:
- Always check pointers for nullptr before dereferencing
- Match every 'new' with 'delete', every 'new[]' with 'delete[]'
- Set pointers to nullptr after deletion
- Use references for function parameters to avoid unnecessary copying
- Validate array indices before access
*/
