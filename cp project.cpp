#include <iostream>        // input output stream 
#include <fstream>         // for txt file
#include <cstring>        //character string

using namespace std;                 // used for as directory as std like a program is a a std folder

const int MAX_PRODUCTS = 100;      //Maximum number of products the inventory can hold
char productNames[MAX_PRODUCTS][50];  // Array to store the names of all products (each name can be up to 49 characters + null terminator)  
int productIDs[MAX_PRODUCTS];    // Array to store unique IDs for each product
int quantities[MAX_PRODUCTS];    // Array to store the available quantity for each product
float prices[MAX_PRODUCTS];      // Array to store the price of each product
int productCount = 0;      // Keeps track of how many products are currently in the inventory
const int LOW_STOCK_THRESHOLD = 5;    // If quantity is less than this value, it's considered "Low Stock"

// Function Prototypes 
// Means these functions will later be defined in the program
// These help the compiler know which functions exist before they are defined later.

void loadFromFile();   // Loads existing inventory data from a file into memory
void saveToFile();     // Saves current inventory data into a file
void addProduct();     // Adds a new product to the inventory
void sellProduct();    // Sells a product by reducing its quantity
void restockProduct();  // Increases the stock quantity of an existing product
void viewInventory(char names[][50], int* ids, int* qtys, float* prices, int count); // Displays the list of all products in the inventory
/*
int*  is a pointer 

i.e., 

int x = 10;
int* ptr = &x;  // ptr now points to the address of x
*/

void searchProduct();   // Allows user to find a product by ID or name
void sortByPrice();    // Sorts products in the inventory based on their price (ascending)
void sortByQuantity();   // Sorts products based on their quantity (ascending)
float calculateTotalValue();   // Calculates and returns the total value of all products in inventory


void addProduct() {
    // Check if the inventory is full. If the count reaches MAX_PRODUCTS, we cannot add more products.
    if (productCount >= MAX_PRODUCTS) {
        cout << "Inventory Full!\n";
        return;  // Exit the function if inventory is full.
    }

    int id;
    cout << "Enter Product ID: ";  // Prompt user to enter the Product ID.
    cin >> id;  // Store the entered ID.

    // Check if the entered Product ID is already in use.
    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == id) {
            cout << "Product ID must be unique.\n";  // If the ID exists, notify the user.
            return;  // Exit the function if ID is duplicate.
        }
    }

    // Store the new Product ID in the inventory.
    productIDs[productCount] = id;

    cout << "Enter Product Name: ";  // Prompt user to enter the Product Name.
    cin.ignore();  // Clear the input buffer before accepting the name.
    cin.getline(productNames[productCount], 50);  // Store the product name.

    cout << "Enter Quantity: ";  // Prompt user to enter the Product Quantity.
    cin >> quantities[productCount];  // Store the quantity.

    // Check if the quantity is valid (it cannot be negative).
    if (quantities[productCount] < 0) {
        cout << "Quantity cannot be negative.\n";  // Notify user if the quantity is negative.
        return;  // Exit the function if invalid quantity.
    }

    cout << "Enter Price: ";  // Prompt user to enter the Product Price.
    cin >> prices[productCount];  // Store the price.

    // Check if the price is valid (it cannot be negative).
    if (prices[productCount] < 0) {
        cout << "Price cannot be negative.\n";  // Notify user if the price is negative.
        return;  // Exit the function if invalid price.
    }

    productCount++;  // Increment the product count to reflect the new product added.

    cout << "Product added successfully.\n";  // Notify the user that the product was successfully added.
}

void sellProduct() {
    int id, qty;
    cout << "Enter Product ID to sell: ";  // Prompt user to enter the Product ID they want to sell.
    cin >> id;  // Store the entered Product ID.

    // Search for the product with the entered ID in the inventory.
    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == id) {  // If the product is found in the inventory.
            cout << "Enter quantity to sell: ";  // Prompt user to enter the quantity to sell.
            cin >> qty;  // Store the entered quantity.

            // Check if the entered quantity is valid (greater than 0).
            if (qty <= 0) {
                cout << "Invalid quantity.\n";  // Notify the user if the quantity is invalid.
                return;  // Exit the function if quantity is invalid.
            }

            // Check if there is enough stock to sell.
            if (quantities[i] < qty) {
                cout << "Insufficient stock!\n";  // Notify the user if stock is insufficient.
                return;  // Exit the function if there is not enough stock.
            }

            // Update the inventory by reducing the quantity of the sold product.
            quantities[i] -= qty;

            cout << "Product sold successfully.\n";  // Notify the user that the product was sold.
            return;  // Exit the function once the sale is complete.
        }
    }
    cout << "Product not found.\n";  // Notify the user if the product ID was not found in the inventory.
}


void restockProduct() {
    int id, qty;
    cout << "Enter Product ID to restock: ";  // Prompt user to enter the Product ID they want to restock.
    cin >> id;  // Store the entered Product ID.

    // Search for the product with the entered ID in the inventory.
    for (int i = 0; i < productCount; i++) {
        if (productIDs[i] == id) {  // If the product is found in the inventory.
            cout << "Enter quantity to add: ";  // Prompt user to enter the quantity to restock.
            cin >> qty;  // Store the entered quantity.

            // Check if the entered quantity is valid (greater than 0).
            if (qty <= 0) {
                cout << "Invalid quantity.\n";  // Notify the user if the quantity is invalid.
                return;  // Exit the function if quantity is invalid.
            }

            // Add the restocked quantity to the existing stock of the product.
            quantities[i] += qty;

            cout << "Product restocked successfully.\n";  // Notify the user that the product has been restocked.
            return;  // Exit the function once the restock is complete.
        }
    }
    cout << "Product not found.\n";  // Notify the user if the product ID was not found in the inventory.
}


void viewInventory(char names[][50], int* ids, int* qtys, float* prices, int count) {
    // If there are no products in the inventory, inform the user and exit the function.
    if (count == 0) {
        cout << "No products in inventory.\n";
        return;
    }

    // Display the header for the inventory list.
    cout << "\n--- Inventory List ---\n";
    cout << "ID\tName\t\tQuantity\tPrice\n";

    // Loop through each product and display its details.
    for (int i = 0; i < count; i++) {
        // Display product ID, name, quantity, and price.
        cout << ids[i] << "\t" << names[i] << "\t\t" << qtys[i] << "\t\t" << prices[i];

        // If the product's quantity is below the low stock threshold, mark it as low stock.
        if (qtys[i] < LOW_STOCK_THRESHOLD)
            cout << " <-- Low Stock!";  // Indicate that this product is running low on stock.
        
        cout << "\n";  // Move to the next line after displaying each product's details.
    }

    // Calculate and display the total value of the inventory by calling the calculateTotalValue function.
    cout << "\nTotal Inventory Value: $" << calculateTotalValue() << "\n";
}


void searchProduct() {
    // Variable to store the user's choice for search method.
    int option;
    
    // Prompt the user to choose whether to search by Product ID or Product Name.
    cout << "Search by:\n1. Product ID\n2. Product Name\nEnter choice: ";
    cin >> option;

    // If the user chooses to search by Product ID.
    if (option == 1) {
        int id;
        // Ask the user to enter the Product ID.
        cout << "Enter Product ID: ";
        cin >> id;

        // Loop through the inventory to find a matching Product ID.
        for (int i = 0; i < productCount; i++) {
            if (productIDs[i] == id) {  // If a match is found.
                // Display product details.
                cout << "Product Found:\n";
                cout << "ID: " << productIDs[i] << "\nName: " << productNames[i]
                     << "\nQuantity: " << quantities[i] << "\nPrice: " << prices[i] << "\n";
                return;  // Exit the function after finding the product.
            }
        }
    }
    // If the user chooses to search by Product Name.
    else if (option == 2) {
        char name[50];
        // Ask the user to enter the Product Name.
        cout << "Enter Product Name: ";
        cin.ignore();  // Ignore the newline left in the input buffer by the previous cin.
        cin.getline(name, 50);  // Read the product name into a char array.

        // Loop through the inventory to find a matching Product Name.
        for (int i = 0; i < productCount; i++) {
            // Compare the entered name with the product names in the inventory.
            if (strcmp(productNames[i], name) == 0) {  // If a match is found.
                // Display product details.
                cout << "Product Found:\n";
                cout << "ID: " << productIDs[i] << "\nName: " << productNames[i]
                     << "\nQuantity: " << quantities[i] << "\nPrice: " << prices[i] << "\n";
                return;  // Exit the function after finding the product.
            }
        }
    }
    else {  // If the user enters an invalid choice.
        cout << "Invalid choice.\n";
        return;
    }

    // If no matching product was found, display a message.
    cout << "Product not found.\n";
}


void sortByPrice() {
    // Outer loop iterates through each product in the inventory (excluding the last one).
    for (int i = 0; i < productCount - 1; i++) {
        
        // Inner loop compares the current product with all remaining products.
        for (int j = i + 1; j < productCount; j++) {
            
            // If the price of the current product is greater than the next product, swap them.
            if (prices[i] > prices[j]) {
                // Swap the prices of the products.
                swap(prices[i], prices[j]);
                // Swap the quantities of the products.
                swap(quantities[i], quantities[j]);
                // Swap the product IDs of the products.
                swap(productIDs[i], productIDs[j]);
                
                // Swap the product names using a temporary array for storage.
                char temp[50];
                strcpy(temp, productNames[i]);
                strcpy(productNames[i], productNames[j]);
                strcpy(productNames[j], temp);
            }
        }
    }
    // After sorting, display a message that the inventory has been sorted by price.
    cout << "Inventory sorted by price.\n";
}

void sortByQuantity() {
    // Outer loop iterates through each product in the inventory (excluding the last one).
    for (int i = 0; i < productCount - 1; i++) {
        
        // Inner loop compares the current product with all remaining products.
        for (int j = i + 1; j < productCount; j++) {
            
            // If the quantity of the current product is greater than the next product, swap them.
            if (quantities[i] > quantities[j]) {
                // Swap the prices of the products.
                swap(prices[i], prices[j]);
                // Swap the quantities of the products.
                swap(quantities[i], quantities[j]);
                // Swap the product IDs of the products.
                swap(productIDs[i], productIDs[j]);
                
                // Swap the product names using a temporary array for storage.
                char temp[50];
                strcpy(temp, productNames[i]);
                strcpy(productNames[i], productNames[j]);
                strcpy(productNames[j], temp);
            }
        }
    }
    // After sorting, display a message that the inventory has been sorted by quantity.
    cout << "Inventory sorted by quantity.\n";
}


float calculateTotalValue() {
    // Initialize total to 0. This will hold the sum of the total inventory value.
    float total = 0;

    // Loop through each product in the inventory to calculate its total value.
    for (int i = 0; i < productCount; i++) {
        // For each product, add its value (price * quantity) to the total.
        total += prices[i] * quantities[i];
    }

    // Return the total value of the inventory.
    return total;
}



void saveToFile() {
    // Open a file named "inventory.txt" in write mode.
    ofstream file("inventory.txt");

    // Loop through each product in the inventory.
    for (int i = 0; i < productCount; i++) {
        // Write each product's details to the file in the following format:
        // Product ID | Product Name | Quantity | Price
        file << productIDs[i] << " | "
             << productNames[i] << " | "
             << quantities[i] << " | "
             << prices[i] << "\n";
    }

    // Close the file after writing all the product data.
    file.close();
}



void loadFromFile() {
    // Open the file "inventory.txt" for reading.
    ifstream file("inventory.txt");
    string line;

    // Loop through each line in the file until the end.
    while (getline(file, line)) {
        // Skip empty lines in the file.
        if (line.empty()) continue;
        
        // Declare variables to hold product data.
        int id, qty;
        float price;
        char name[50];

        // Parse the line using the '|' separator.
        // The format is: <ID> | <Name> | <Quantity> | <Price>
        sscanf(line.c_str(), "%d | %49[^|] | %d | %f", &id, name, &qty, &price);

        // Assign parsed data to the corresponding arrays.
        productIDs[productCount] = id;             // Set Product ID
        strcpy(productNames[productCount], name);  // Set Product Name
        quantities[productCount] = qty;            // Set Product Quantity
        prices[productCount] = price;              // Set Product Price

        // Increment productCount to keep track of the number of products loaded.
        productCount++;
    }

    // Close the file after reading all data.
    file.close();
}



int main() {
    // Load product data from file at the start of the program
    loadFromFile();
    
    int choice;

    do {
        // Display the menu for the user
        cout << "\n=== Hardware Store Inventory Management ===\n";
        cout << "1. Add New Product\n";        // Option to add a new product
        cout << "2. Sell Product\n";           // Option to sell a product
        cout << "3. Restock Product\n";        // Option to restock a product
        cout << "4. View Inventory\n";         // Option to view all products
        cout << "5. Search Product\n";         // Option to search for a product
        cout << "6. Sort by Price\n";          // Option to sort products by price
        cout << "7. Sort by Quantity\n";       // Option to sort products by quantity
        cout << "8. Save and Exit\n";          // Option to save data and exit
        cout << "Enter your choice: ";
        cin >> choice;  // Get user input for the menu choice

        // Execute the corresponding function based on the user's choice
        switch (choice) {
            case 1: addProduct(); break;         // Add new product to the inventory
            case 2: sellProduct(); break;        // Sell a product
            case 3: restockProduct(); break;     // Restock an existing product
            case 4: 
                // View the inventory, passing necessary arguments
                viewInventory(productNames, productIDs, quantities, prices, productCount); 
                break;
            case 5: searchProduct(); break;      // Search for a product by ID or name
            case 6: sortByPrice(); break;        // Sort products by price
            case 7: sortByQuantity(); break;     // Sort products by quantity
            case 8: 
                // Save data to file and exit the program
                saveToFile(); 
                cout << "Data saved. Exiting...\n"; 
                break;
            default: 
                // Handle invalid input
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);  // Continue the loop until user chooses to exit (choice 8)

    return 0;  // Return 0 to indicate successful program execution
}


