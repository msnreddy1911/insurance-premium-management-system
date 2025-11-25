Operations
📄 Abstract

The Insurance Premium Management System is a C-based console application designed to manage insurance products and their premium details. It demonstrates the use of file handling, structures, and menu-driven programming.
The program allows users to add, display, search, update, and delete insurance products stored in a binary data file.

🚀 Features
✔ Core Functionalities

Add new insurance product

Display all products

Search product by ID

Update product information

Delete product by ID

Persistent storage using products.dat

✔ Program Concepts Used

File handling (binary files)

Structures in C

CRUD operations

Menu-driven interface

Input validation

🛠 Technical Requirements

Programming Language: C

Compiler: GCC / MinGW / Clang / Turbo C (Not recommended)

OS Support: Windows / Linux / macOS

Storage: Uses binary file: products.dat

📌 Functional Requirements
1. Add Product

User enters:

Product ID

Product Name

Premium Amount

Duration (Years)

2. Display Products

Displays all product details stored in the file.

3. Search Product

Search by Product ID.

4. Update Product

Update existing product details by ID.

5. Delete Product

Removes a product record from file storage.

6. Exit

Terminates the program.

📂 Data Storage File

The program stores all product records in:

products.dat


Each product record contains:

int id

char name[100]

float premium

int duration_years

▶️ How to Compile & Run
Step 1: Save the C File

Save program code in:

insurance_premium.c

Step 2: Compile
gcc insurance_premium.c -o insurance_premium

Step 3: Run Program
./insurance_premium


(Windows)

insurance_premium.exe

📸 Screenshots (Text-Based Simulation)
1. Main Menu
==== Insurance Premium Management System ====
1. Add Product
2. Display All Products
3. Search Product by ID
4. Update Product by ID
5. Delete Product by ID
6. Exit
Enter your choice:

2. Add Product Screen
Enter Product ID: 101
Enter Product Name: LifeSecure
Enter Premium Amount: 2500
Enter Duration (years): 15
Product added successfully.

3. Display Products Output
===== All Products =====

Product ID       : 101
Product Name     : LifeSecure
Premium Amount   : 2500.00
Duration (years) : 15
-------------------------------

4. Search Product Output
Enter Product ID to search: 101

Product found:
Product ID       : 101
Product Name     : LifeSecure
Premium Amount   : 2500.00
Duration (years) : 15

5. Update Product Output
Enter Product ID to update: 101

Existing record:
Product ID       : 101
Product Name     : LifeSecure
Premium Amount   : 2500.00
Duration (years) : 15

Enter new values:
New Product Name: LifeSecurePlus
New Premium Amount: 3000
New Duration (years): 20

Product updated successfully.

6. Delete Product Output
Enter Product ID to delete: 101
Product with ID 101 deleted successfully.

🧾 File Structure
project-folder/
│
├── insurance_premium.c
├── products.dat          (auto-created)
└── README.md

📘 Project Learning Outcomes

Working with binary file storage in C

Implementing CRUD operations

Using structures effectively

Handling user input safely

Building a real-world console application

🎯 Future Enhancements (Optional)

Add policyholder management

Add insurance claim processing

Convert binary file to CSV export

Add admin login

GUI version using C++ or Python
