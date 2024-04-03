# Simple Inventory App

# The Program Flow
 - Display Title
 - Menu
  - 1. Daily Log Inventory
  - 2. Products
  - 3. Exit

# 1. Daily Log Inventory
 - Display Last Inventory
 - Menu
  - 1. Log New Inventory
    - Display Current Date
    - manual loop: 
     - list of all products
      - inventory_id (unique)
      - product_id
      - quantity
      - date(hide)
      - save record
  - 2. Edit Inventory
    - Display all dates from daily-log-inventory folder
      - enter the correct date: (2024-04-03)
       - list of all batches
        - batch 1
        - batch 2
        - Enter batch number: 
          - display all daily logs under specific batch. (sample batch 1)
          - Enter Product id: 
          - display product details
           - Enter quantity: 
       
  - 3. Delete Inventory
  - 4. Back To Main Menu

# 2. Products
 - Display List of Products
 - Menu
  - 1. Add New Product
   - Enter Product Id
   - Enter Product Name
  - 2. Edit Product
   - call search product
   - Edit Product Id
   - Edit Product Name
   - Edit Product Category
  - 3. Delete Product
  - 4. Search Product
    - Display Products
    - Enter Product ID
    - Display Result
  - 5. Back To Main Menu

# Technical/Specs
 - Database: csv
 - Code Pattern: OOP
 - Objects
  - Product
   - Variables: product_id, product_name
  - ProductInventory
   - Variables: inventory_id, product_id, quantiy, date

# App Commands
 - Compiling and Creating an Executable file: g++ simple-inventory-app.cpp -o simple-inventory-app
 - Run the executable file: ./simple-inventory-app
