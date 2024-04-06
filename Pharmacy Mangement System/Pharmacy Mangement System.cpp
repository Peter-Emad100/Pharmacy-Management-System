//**********Libraries***********//
#include <string_view>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include "SaveData.h"
using namespace std;

//**********Declarations***********//

const int Size = 100;
int medicine_data = 0;
int user_data = 0;
int requestcounter = 0;
int chosenOption; // variable to choose with which order you want to go through after log in

struct medicine {
	int ID;
	string name;
	string description;
	string concentration;
	bool availability;
	string category;
	float price;
	int quantity_in_stock;
	void initialize(int _id, string _name, string _desc, string _conc, bool _avail, string _category, float _price, int _quantity) {
		ID = _id;
		name = _name;
		description = _desc;
		concentration = _conc;
		availability = _avail;
		category = _category;
		price = _price;
		quantity_in_stock = _quantity;
	}
};
medicine medicines[Size];

struct user {
	enum role { User, Admin };
	int ID;
	string username;
	string password;
	string email;
	string address;
	string phone;
	role his_role;
	void initialize(int _id, string _name, string _pass, string _email, string _address, string _phone, role _rol) {
		ID = _id;
		username = _name;
		password = _pass;
		email = _email;
		address = _address;
		phone = _phone;
		his_role = _rol;
	}
};
user users[Size];
user currentUser; //Temp to keep the current user's data

struct order {
	int userID;
	string orderDate;
	int medicine_ID[10] = {};
	float totalPrice;
	string shipDate;
	void initialize(int _id, string _orderDate, int _medicine_ID[], float _Price, string _shipDate) {
		userID = _id;
		orderDate = _orderDate;
		totalPrice = _Price;
		shipDate = _shipDate;
		int i = 0;
		while (_medicine_ID[i] != 0) {
			medicine_ID[i] = _medicine_ID[i];
			i++;
		}

	}
};
order orders[Size] = {};

struct request {
	int userID;
	string medicineName;
	int amountNeeded;
};
request requests[15];

//********Function Declares***********//
void dataForTestPurposes();
bool isUsernameTaken(string username);
void signUp();
bool validateUser(string username, string password, user& currentUser);
void logInInterface();
void userPermissions();
void adminPermissions();
void editUserCredentials(int index);
bool searchForMedicineByName();
void searchForMedicineByCategory();
void makeOrder(int customerID, string orderDate, string shipDate, int orderTime, medicine m[10]);
void showOrderReceipt(order lastOrder);
void makeRequest(string _username, string _medicineName, int _amountReq);
void showAllPreviousOrders();
void logOut();

//**********Functions***********//

void dataForTestPurposes() {

	//*******************Medicine data****************************
	medicines[0].initialize(1, "Paracetamol", "Pain reliever and fever reducer", "500 mg", true, "Analgesic", 5.99, 100);
	medicines[1].initialize(2, "Lisinopril", "Used to treat high blood pressure", "10 mg", true, "Antihypertensive", 10.49, 50);
	medicines[2].initialize(3, "Omeprazole", "Treats heartburn, stomach ulcers, and gastroesophageal reflux disease (GERD)", "20 mg", true, "Gastrointestinal", 7.25, 80);
	medicines[3].initialize(4, "Atorvastatin", "Lowers high cholesterol and triglycerides", "20 mg", true, "Lipid-lowering agent", 15.75, 30);
	medicines[4].initialize(5, "Metformin", "Treats type 2 diabetes", "500 mg", true, "Antidiabetic", 8.99, 60);
	medicines[5].initialize(6, "Amoxicillin", "Antibiotic used to treat bacterial infections", "250 mg", true, "Antibiotic", 6.50, 0);
	medicines[6].initialize(7, "Alprazolam", "Treats anxiety and panic disorders", "0.25 mg", true, "Anxiolytic", 12.99, 40);
	medicines[7].initialize(8, "Ibuprofen", "Nonsteroidal anti-inflammatory drug (NSAID)", "200 mg", true, "Analgesic", 4.75, 200);
	medicines[8].initialize(9, "Cetirizine", "Antihistamine used for allergy relief", "10 mg", true, "Antihistamine", 9.25, 0);
	medicines[9].initialize(10, "Ranitidine", "Reduces stomach acid production to treat heartburn and ulcers", "150 mg", true, "Gastrointestinal", 6.99, 90);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------

	//*******************User data****************************
	users[0].initialize(1, "Naruto", "NotNaruto", "narutouzumaki@example.com", "123 Main St, Cityville", "+1234567890", user::User);
	users[1].initialize(2, "Madara", "password2", "nadarauchiha@example.com", "456 Elm St, Townsville", "+1987654321", user::User);
	users[2].initialize(3, "Cillian", "Cillianpass", "callianmurphy@example.com", "789 Oak St, Villageton", "+1122334455", user::Admin);
	users[3].initialize(4, "Aras", "passBulut", "arasbulut@example.com", "987 Pine St, Hamletville", "+9988776655", user::User);
	users[4].initialize(5, "Sung", "jinwoo", "sungjinwoo@example.com", "654 Birch St, Countryside", "+1122334455", user::User);
	users[5].initialize(6, "Iman", "imangadzhi", "imangadzhi@example.com", "321 Maple St, Suburbia", "+9988776655", user::User);
	users[6].initialize(7, "Ali", "AliAli", "alimuhammadali.smith@example.com", "111 Cedar St, Ruralville", "+1122334455", user::Admin);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	int medicine1[] = { 1, 2, 3, 0 };
	orders[0].initialize(1, "2024-03-27", medicine1, 500.0, "2024-03-27");
	int medicine2[] = { 4, 5, 0 };
	orders[1].initialize(2, "2024-03-28", medicine2, 300.0, "2024-03-28");
	int medicine3[] = { 9, 4,5,7 ,0 };
	orders[2].initialize(3, "2024-03-29", medicine3, 3000.0, "2024-03-29");
}

bool isUsernameTaken(string username) {
	int i = 0;
	while (users[i].ID != 0) {
		if (users[i].username == username) {
			return true;
		}
		++i;
	}
	return false;
}

void signUp() {
	user newUser;
	int id = user_data + 1; // Next available ID

	newUser.ID = id;

	cout << "Enter your username: ";
	cin >> newUser.username;
	while (isUsernameTaken(newUser.username))  //Checks if username is already taken in our database or no.
	{
		cout << "A user with that username already exists. Please enter a different username: ";
		cin >> newUser.username;
	}

	cout << "Enter your password: ";
	cin >> newUser.password;
	cout << "Enter your e-mail: ";
	cin >> newUser.email;
	cout << "Enter your address: ";
	cin.ignore(1, '\n');
	getline(cin, newUser.address);
	cout << "Enter your phone: ";
	cin >> newUser.phone;

	int roleChoice;
	do {
		cout << "Pick your role\n1-User\n2-Admin\n";
		cin >> roleChoice;
		if (roleChoice == 1) {
			newUser.his_role = user::User;
		}
		else if (roleChoice == 2) {
			newUser.his_role = user::Admin;
		}
		else {
			cout << "Invalid role choice. Please enter 1 for User or 2 for Admin.\n";
		}
	} while (roleChoice != 1 && roleChoice != 2); // Loop until a valid role choice is made

	users[id - 1] = newUser; // Save the new user data into our users array

	cout << "Congratulations! Your account has been successfully created.\n";
	
	saveOneUserDataLocally();
	
	user_data++;										 // Increment user_data to keep track of the total number of users
}

bool validateUser(string username, string password, user& currentUser)
{
	int userIndex = 0;

	// Loop through the users until a user with userID = 0 is found,
	// indicating that there are no more users in our database
	while (users[userIndex].ID != 0)
	{
		if (users[userIndex].username == username && users[userIndex].password == password)
		{
			currentUser = users[userIndex];     //Avoiding any kind of problem when showing permissions based on the role 
			currentUser.ID = users[userIndex].ID;
			return true;
		}
		userIndex++;
    
	}
	return false;
}

void logInInterface()
{
	bool loggedIn = false;

	while (!loggedIn)
	{


		cout << "Enter your username: ";
		cin >> currentUser.username;
		cout << "Enter your password: ";
		cin >> currentUser.password;

		if (validateUser(currentUser.username, currentUser.password, currentUser)) {

			loggedIn = true;
			cout << "Log in success. Welcome back, " << currentUser.username << " :D\n-------------------------------------------\n";


			if (currentUser.his_role == user::User)
			{
				userPermissions();
			}
			else
			{
				adminPermissions();
			}
			//editUserCredentials(currentUser.ID-1);
			cin >> chosenOption;
		}


		else
		{
			cout << "Invalid credentials. The username or password you entered is incorrect. Please try again.\n";

		}
	}
}

void userPermissions() {
	cout << "1- Search for medicine by name\n";
	cout << "2- Search for medicine by category\n";
	cout << "3- Add order\n";
	cout << "4- Choose payment method\n";
	cout << "5- View order\n";
	cout << "6- Request drug\n";
	cout << "7- View all previous orders\n";
	cout << "8- Log out\n";
}

void adminPermissions() {
	cout << "1- Add new user\n";
	cout << "2- Remove user\n";
	cout << "3- Add new medicine\n";
	cout << "4- Remove medicine\n";
	cout << "5- Manage orders\n";
	cout << "6- Manage payments\n";
	cout << "7- Search for medicine by name\n";
	cout << "8- Search for medicine by category\n";
	cout << "9- Add order\n";
	cout << "10- Choose payment method\n";
	cout << "11- View order\n";
	cout << "12- Request drug\n";
	cout << "13- View all previous orders\n";
}

void editUserCredentials(int index)
{
	cout << "What are you willing to change ?\n";
	cout << "1- Phone Number\n";
	cout << "2- Address\n";
	int choice;
	do {

		cin >> choice;
		if (choice == 1) {
			cout << "Enter your new phone number: ";
			cin >> users[index].phone;

		}
		else if (choice == 2) {
			cout << "Enter your new adress: ";
			cin.ignore(1, '\n');
			getline(cin, users[index].address);
		}
		else {
			cout << "Invalid choice. Please enter 1 for Phone Number or 2 for Address.\n";
		}
	} while (choice != 1 && choice != 2); // Loop until a valid choice is made

	saveUserDataLocally();


}

bool searchForMedicineByName() {
	string name;
	cout << "Enter the medicine name\n";
	cin >> name;
	int x = name.size();
	if (name[0] >= 'a' && name[0] <= 'z') {
		name[0] -= 32;
	}
	int i = 0;
	bool found = 0;
	bool instock = 1;
	while (medicines[i].ID != 0)
	{
		string_view sv(medicines[i].name.c_str(), x);
		if (name == sv) {
			cout << medicines[i].ID << " -- " << medicines[i].name << " -- " << medicines[i].availability << " -- " << medicines[i].category << " -- " << medicines[i].price << " -- " << medicines[i].quantity_in_stock << '\n';
			if (medicines[i].quantity_in_stock <= 0) {
				instock = 0;
			}
			found = 1;
		}
		i++;
	}
	if (instock && found) {
		return 1;
	}
	else {
		int amountrequested;
		cout << "Enter the amount you need:\n";
		cin >> amountrequested;
		makeRequest(currentUser.username, name, amountrequested);
	}
}

void searchForMedicineByCategory() {
	string category;
	cin >> category;
	bool found = false;
	for (int i = 0; i < Size; i++) {
		if (category == medicines[i].category) {
			cout << medicines[i].name << " -- " << medicines[i].ID << " -- " << medicines[i].category << '\n';
			found = true;
		}
	}
	if (found == false) {
		cout << "there is no medicine meet this category\n";
		int ifwant;
		string medicineName;
		int amountrequested;
		cout << "Want to make a request?choose(any number for yes/ 0 for no)" << endl;
		cin >> ifwant;
		if (ifwant != 0)
		{
			cout << "Enter medicine name:\n";
			cin >> medicineName;
			cout << "Enter the amount you need:\n";
			cin >> amountrequested;
			makeRequest(currentUser.username, medicineName, amountrequested);
		}
	}
}

// Convert date string to integers
void parseDateString(const std::string& dateString, int& year, int& month, int& day) {
	std::stringstream ss(dateString);
	char dash;
	ss >> year >> dash >> month >> dash >> day;
}

// calculate the difference in days between two dates
int dateDifference(const std::string& date1, const std::string& date2) {
	int year1, month1, day1;
	int year2, month2, day2;

	parseDateString(date1, year1, month1, day1);
	parseDateString(date2, year2, month2, day2);

	// Calculate the total number of days for each date
	int days1 = year1 * 365 + month1 * 30 + day1;
	int days2 = year2 * 365 + month2 * 30 + day2;

	// Calculate the difference in days
	int difference = days2 - days1;

	return difference;
}

void makeOrder(int customerID, string orderDate, string shipDate, int orderTime, medicine m[10]) {
	char choice = 'a';
	int medicine1[10]{};
	int quantity1[10]{};
	float sum = 0;

	//take medecine id and quantity
	for (int i = 0; i < 10; i++)
	{
		cout << "select medicine id\n";
		int medicineid = -1;
		cin >> medicineid;
		medicineid = medicineid - 1;
		medicine1[i] = medicineid + 1;

		cout << "select quantity\n";
		int quantity = 0;
		cin >> quantity;
		quantity1[i] = quantity;

		while (quantity > m[medicineid].quantity_in_stock)
		{
			cout << "quantity in stock is : " << m[medicineid].quantity_in_stock << '\n';
			cout << "select quantity\n";
			cin >> quantity;
		}
		//calculate total price of each medicine
		sum = sum + (quantity * m[medicineid].price);
		cout << "Total Price : " << sum << '\n';

		cout << "Do you want to add medicine?" << "y" << "/" << "n\n";
		cin >> choice;
		if (choice != 'y')
		{
			break;
		}
	}

	int Payment_method = -1;
	//choose payment method
	while (true)
	{
		cout << "Choose payment method (Cash, pick at pharmacy, add visa)(1,2,3)\n";
		cin >> Payment_method;

		if (Payment_method == 1)
		{
			cout << "payment with cash\n";

		}
		else if (Payment_method == 2)
		{
			cout << "pick at pharmacy\n";
		}
		else if (Payment_method == 3)
		{
			cout << "payment with visa\n";
		}

		cout << "are you sure? (y / n)\n";
		char choice2;
		cin >> choice2;

		if (choice2 == 'y')
		{
			break;
		}
	}
	cout << "ordertime : " << orderTime << '\n';

	//create an order
	orders[0].initialize(customerID, orderDate, medicine1, sum, shipDate);



}

void showOrderReceipt(order lastOrder) {
	cout << "order date : " << lastOrder.orderDate << '\n';
	int i = 0;
	while (lastOrder.medicine_ID[i] != 0) {
		cout << "medicine id : " << lastOrder.medicine_ID[i] << '\n';
		i++;
	}
	cout << "user id : " << lastOrder.userID << '\n';
	cout << "total price : " << lastOrder.totalPrice << '\n';
	cout << "ship date : " << lastOrder.shipDate << '\n';
}

void makeRequest(string _username, string _medicineName, int _amountReq)
{
	_username = currentUser.username;
	int response;
	cout << "Confirm the request? choose (1 for yes / 0 for no): " << endl;
	cin >> response;
	//no cases because it will be handeled in gui
	requestcounter++;
	if (response == 1) {
		for (int i = 0; i < Size; i++)
		{
			if (_username == users[i].username)
			{
				currentUser.ID = users[i].ID;
			}
		}
		for (int j = 0; j < 15; j++)
		{
			if (requests[j].userID == 0)
			{
				requests[j].userID = currentUser.ID;
				requests[j].medicineName = _medicineName;
				requests[j].amountNeeded = _amountReq;
				break;
			}
		}
	}
}

void showAllPreviousOrders() {
	for (int i = 0; i < Size; i++) {   // checking for the current user ID to be able to get his/her orders using ID
		if (currentUser.username == users[i].username) {
			currentUser.ID == users[i].ID;
			break;
		}
	}

	cout << "Your previous orders: \n";
	cout << "---------------------\n";
	int num_order = 1;   // to display list of numbered orders
	bool found_orders = false;  // to check if there were no orders regesitered for this user
	for (int i = 0; i < Size; i++) {
		if (orders[i].userID == currentUser.ID) {
			found_orders = true;
			cout << "Order number (" << num_order << ") : \n";
			cout << "-------------------\n";
			cout << "Date of order: " << orders[i].orderDate << "\n";
			cout << "Ship date: " << orders[i].shipDate << "\n";

			// printing out medicine id 

			cout << "Medicine ID: ";
			int j = 0;
			int currentID = 0;

			while (orders[i].medicine_ID[j] != 0) {
				currentID *= 10;
				cout << orders[i].medicine_ID[j];
				j++;
				currentID += orders[i].medicine_ID[j];
			}
			cout << "\n";

			//getting medicine name from medicine ID
			for (int i = 0; i < Size; i++) {
				if (medicines[i].ID == currentID) {
					cout << "Name of the medicine: " << medicines[i].name << "\n";
					cout << "concentraion of the medicine" << medicines[i].concentration << "\n";
					break;
				}
			}


			cout << "Total price: " << orders[i].totalPrice << "\n";
			num_order++;

		}
	}
	if (!found_orders) {
		cout << "You have no previous orders \n";
	}
	cout << "------------------------------------------ \n ";


}

void logOut()
{
	logInInterface(); //Basically, just open the log in interface again if you are willing to log out 
}

int main()
{
	//dataForTestPurposes();
	//saveAllDataLocally();
	saveAllDataToArr();
	signUp();
	logInInterface();
	//int orderTime = dateDifference("2024-03-27", "2024-05-27");
	//makeOrder(users[1].ID, "2024-03-27", "2024-05-27", orderTime, medicines);
	//showOrderReceipt(orders[0]);
	if (currentUser.his_role == user::User) {
		if (chosenOption == 7)
			showAllPreviousOrders();
		else if (chosenOption == 6)
		{
			string medicineName;
			int amountrequested;
			bool found = false;
			cout << "Enter medicine name:\n";
			cin >> medicineName;
			cout << "Enter the amount you need:\n";
			cin >> amountrequested;
			makeRequest(currentUser.username, medicineName, amountrequested);
		}
	}
	else {
		if (chosenOption == 13)
			showAllPreviousOrders();
		else if (chosenOption == 12)
		{
			string medicineName;
			int amountrequested;
			cout << "Enter medicine name:\n";
			cin >> medicineName;
			cout << "Enter the amount you need:\n";
			cin >> amountrequested;
			makeRequest(currentUser.username, medicineName, amountrequested);
			saveRequestsDataLocally();
		}
	}
	
}