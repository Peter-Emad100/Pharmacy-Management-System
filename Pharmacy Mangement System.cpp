﻿//**********Libraries***********//
#include <string_view>
#include<iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

//**********Declarations***********//

const int Size = 100;
const int medicine_data = 10;
int user_data = 7;

struct medicine {
	int ID;
	string name;
	string description;
	bool availability;
	string category;
	float price;
	int quantity_in_stock;
	void initialize(int _id, string _name, string _desc, bool _avail, string _category, float _price, int _quantity) {
		ID = _id;
		name = _name;
		description = _desc;
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


//**********Functions***********//
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
			return true;
		}
		userIndex++;

	}
	return false;
}
void logInInterface()
{
	bool loggedIn = false;
	user currentUser; //Temp to keep the current user's data
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
				cout << "1- Search for medicine by name\n";
				cout << "2- Search for medicine by category\n";
				cout << "3- Add order\n";
				cout << "4- Choose payment method\n";
				cout << "5- View order\n";
				cout << "6- Request drug\n";
				cout << "7- View all previous orders\n";
				cout << "8- Log out\n";
			}
			else
			{
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
		}


		else
		{
			cout << "Invalid credentials. The username or password you entered is incorrect. Please try again.\n";

		}
	}
}


void appendingNewUser();
void saveUserDataLocally();

void UsersSignUp()
{
	user tempUser;
	int role;
	cout << "Username     : "; cin >> tempUser.username;
	cout << "Email address: "; cin >> tempUser.email;
	cout << "Phone number : "; cin >> tempUser.phone;
	cout << "Password     : "; cin >> tempUser.password;
	cout << "Address      : "; cin >> tempUser.address;
	cout << "Role         : "; cin >> role;

	if (role)
	{
		tempUser.his_role = user::Admin;
	}
	else if (role == 0)
	{
		tempUser.his_role = user::User;
	}

	user_data++;
	tempUser.ID = (user_data + 1);
	users[user_data] = tempUser;
	appendingNewUser();

}

void editingUserData(user currentUser)
{
	int choice;
	int i = 0;
	for (; i < user_data; i++)
	{
		if (users[i].username == currentUser.username && users[i].password == currentUser.password)
		{
			break;
		}
	}
	cout << "====== choose what to edit =======\n\n ";
	cout << "[1] username\n";
	cout << "[2] password\n";
	cout << "[3] address \n";
	cout << "[4] phone number\n";
	cout << "input choice: "; cin >> choice;

	switch (choice)
	{
	case 1:
		cin >> users[i].username; break;
	case 2:
		cin >> users[i].password; break;
	case 3:
		cin >> users[i].address; break;
	case 4:
		cin >> users[i].phone; break;
	}

	saveUserDataLocally();

}



void appendingNewUser()
{
	fstream file;
	file.open("UserData.csv", ios::app);
	if (file.is_open())
	{
		file << users[user_data].username << "|";
		file << users[user_data].ID << "|";
		file << users[user_data].email << "|";
		file << users[user_data].password << "|";
		file << users[user_data].address << "|";
		file << users[user_data].phone << "|";
		file << users[user_data].his_role;
		file << "\n";
	}
	file.close();
}


void logOut()
{
	logInInterface(); //Basically, just open the log in interface again if you are willing to log out 
}


void saveMedicineDataLocally() {
	fstream file;
	file.open("MedicineData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "Name" << "|" << "Category" << "|" << "Description" << "|" << "ID" << "|" << "Quantity in stock" << "|" << "Availabilty" << "|" << "Price\n";
		for (int i = 0; i < 10; i++)
		{
			file << medicines[i].name << "|";
			file << medicines[i].category << "|";
			file << medicines[i].description << "|";
			file << medicines[i].ID << "|";
			file << medicines[i].quantity_in_stock << "|";

			file << medicines[i].availability << "|";
			file << medicines[i].price;
			file << "\n";
		}
		file.close();
	}
}

void saveUserDataLocally() {
	fstream file;
	file.open("UserData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "Name" << "|" << "ID" << "|" << "Email" << "|" << "Password" << "|" << "Address" << "|" << "Phone" << "|" << "Role\n";
		for (int i = 0; i < user_data; i++)
		{
			file << users[i].username << "|";
			file << users[i].ID << "|";
			file << users[i].email << "|";
			file << users[i].password << "|";
			file << users[i].address << "|";
			file << users[i].phone << "|";
			file << users[i].his_role;
			file << "\n";
		}
		file.close();
	}
}

void saveAllDataLocally() {
	saveMedicineDataLocally();
	saveUserDataLocally();
}

void saveMedicineDataToArr() {
	fstream file;
	file.open("MedicineData.csv", ios::in);
	if (file.is_open())
	{
		string data;
		getline(file, data);                  //Skips a data
		getline(file, data);                  //Skips a data
		for (int i = 0; i < medicine_data; i++)
		{
			getline(file, data, '|');
			medicines[i].name = data;
			cout << medicines[i].name << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].category = data;
			cout << medicines[i].category << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].description = data;
			cout << medicines[i].description << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].ID = stoi(data);
			cout << medicines[i].ID << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			/*if (medicines[i].quantity_in_stock <= 1)
			{
				warningOfShortage();                                       //A funtion to make later. For admins, in order to get an alert when they are low in sth >:3
			}*/
			medicines[i].quantity_in_stock = stoi(data);
			cout << medicines[i].quantity_in_stock << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].availability = stoi(data);
			cout << medicines[i].availability << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data);
			medicines[i].price = stof(data);
			cout << medicines[i].price << "\n" << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
		}
		file.close();
	}
}

void saveUserDataToArr() {
	fstream file;
	file.open("UserData.csv", ios::in);
	if (file.is_open())
	{
		string data;
		getline(file, data);                  //Skips a data
		getline(file, data);                  //Skips a data
		for (int i = 0; i < user_data; i++)
		{
			getline(file, data, '|');
			users[i].username = data;
			cout << users[i].username << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].ID = stoi(data);
			cout << users[i].ID << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].email = data;
			cout << users[i].email << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].password = data;
			cout << users[i].password << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].address = data;
			cout << users[i].address << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].phone = data;
			cout << users[i].phone << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data);
			if (stoi(data) == 1)
				users[i].his_role = user::Admin;
			else
				users[i].his_role = user::User;
			cout << users[i].his_role << "\n" << "\n";             //Testing the outcomes. Best to keep here, so don't delete//
		}
		file.close();
	}
}

void saveAllDataToArr() {
	saveMedicineDataToArr();
	saveUserDataToArr();
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
			cout << medicines[i].ID << " -- " << medicines[i].name << " -- " << medicines[i].availability << " -- " << medicines[i].category << " -- " << medicines[i].price << " -- " << medicines[i].quantity_in_stock << "\n";
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
		return 0;
	}
}

void searchForMedicineByCategory() {
	string category;
	cin >> category;
	bool found = false;
	for (int i = 0; i < Size; i++) {
		if (category == medicines[i].category) {
			cout << medicines[i].name << " -- " << medicines[i].ID << " -- " << medicines[i].category << "\n";
			found = true;
		}
	}
	if (found == false) {
		cout << "there is no medicine meet this category\n";
	}
}

void showOrderReceipt(order lastOrder) {
	cout << "order date : " << lastOrder.orderDate << "\n";
	int i = 0;
	while (lastOrder.medicine_ID[i] != 0) {
		cout << "medicine id : " << lastOrder.medicine_ID[i] << "\n";
		i++;
	}
	cout << "user id : " << lastOrder.userID << "\n";
	cout << "total price : " << lastOrder.totalPrice << "\n";
	cout << "ship date : " << lastOrder.shipDate << "\n";
}

void dataForTestPurposes() {

	//*******************Medicine data****************************
	medicines[0].initialize(1, "Paracetamol", "Pain reliever and fever reducer", true, "Analgesic", 5.99, 100);
	medicines[1].initialize(2, "Lisinopril", "Used to treat high blood pressure", true, "Antihypertensive", 10.49, 50);
	medicines[2].initialize(3, "Omeprazole", "Treats heartburn, stomach ulcers, and gastroesophageal reflux disease (GERD)", true, "Gastrointestinal", 7.25, 80);
	medicines[3].initialize(4, "Atorvastatin", "Lowers high cholesterol and triglycerides", true, "Lipid-lowering agent", 15.75, 30);
	medicines[4].initialize(5, "Metformin", "Treats type 2 diabetes", true, "Antidiabetic", 8.99, 60);
	medicines[5].initialize(6, "Amoxicillin", "Antibiotic used to treat bacterial infections", true, "Antibiotic", 6.50, 0);
	medicines[6].initialize(7, "Alprazolam", "Treats anxiety and panic disorders", true, "Anxiolytic", 12.99, 40);
	medicines[7].initialize(8, "Ibuprofen", "Nonsteroidal anti-inflammatory drug (NSAID)", true, "Analgesic", 4.75, 200);
	medicines[8].initialize(9, "Cetirizine", "Antihistamine used for allergy relief", true, "Antihistamine", 9.25, 0);
	medicines[9].initialize(10, "Ranitidine", "Reduces stomach acid production to treat heartburn and ulcers", true, "Gastrointestinal", 6.99, 90);

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
			cout << "quantity in stock is : " << m[medicineid].quantity_in_stock << "\n";
			cout << "select quantity\n";
			cin >> quantity;
		}
		//calculate total price of each medicine
		sum = sum + (quantity * m[medicineid].price);
		cout << "Total Price : " << sum << "\n";

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

	cout << "ordertime : " << orderTime << "\n";

	//create an order
	orders[0].initialize(customerID, orderDate, medicine1, sum, shipDate);



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

int main()
{
	//	dataForTestPurposes();
		//saveAllDataLocally();
		//saveAllDataToArr();
		//logInInterface();
		//int orderTime = dateDifference("2024-03-27", "2024-05-27");
		//makeOrder(users[1].ID, "2024-03-27", "2024-05-27", orderTime, medicines);
		//showOrderReceipt(orders[0]);
	UsersSignUp();
}