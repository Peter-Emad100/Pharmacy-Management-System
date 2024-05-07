#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "SaveData.h"

using namespace std;
vector<string> paymentMethods;
extern const int Size = 100;
extern int medicine_data;
extern int user_data;
extern int requestcounter;
extern int OrderCounter;
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
extern medicine medicines[Size];

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
extern user users[Size];

struct order {
	int userID;
	string orderDate;
	int orderID;
	bool orderState = false;
	int medicine_ID[10] = {};
	float totalPrice;

	string shipDate;
	void initialize(int _id, string _orderDate, int _medicine_ID[], float _Price, string _shipDate, int order_id, bool order_state) {
		userID = _id;
		orderDate = _orderDate;
		totalPrice = _Price;
		shipDate = _shipDate;
		orderID = order_id;
		orderState = order_state;
		int i = 0;
		while (_medicine_ID[i] != 0) {
			medicine_ID[i] = _medicine_ID[i];
			i++;
		}

	}
};
extern order orders[Size];

struct request {
	int userID;
	string medicineName;
	int amountNeeded;
};
extern request requests[15];

void saveMedicineDataLocally() {
	fstream file;
	file.open("Data/MedicineData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "ID" << '|' << "Name" << '|' << "Category" << '|' << "Description" << '|' << "Concentration" << '|' << "Quantity in stock" << '|' << "Availabilty" << '|' << "Price\n";
		for (int i = 0; i < medicine_data; i++)
		{
			file << medicines[i].ID << '|';
			file << medicines[i].name << '|';
			file << medicines[i].category << '|';
			file << medicines[i].description << '|';
			file << medicines[i].concentration << '|';
			file << medicines[i].quantity_in_stock << '|';
			file << medicines[i].availability << '|';
			file << medicines[i].price;
			file << '\n';
		}
		file.close();
	}
}
void savePayMethodLocally()
{
	fstream file;
	file.open("Data/payMethod.csv", ios::out);
	if (file.is_open())
	{
		for (auto it = paymentMethods.begin(); it != paymentMethods.end(); ++it)
		{
			if (paymentMethods.back() == *it)
			{
				file << *it;
			}
			else
				file << *it << endl;
		}
		file.close();
	}
}
void saveUserDataLocally() {
	fstream file;
	file.open("Data/UserData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "Name" << '|' << "ID" << '|' << "Email" << '|' << "Password" << '|' << "Address" << '|' << "Phone" << '|' << "Role\n";
		for (int i = 0; i < user_data; i++)
		{
			file << users[i].username << '|';
			file << users[i].ID << '|';
			file << users[i].email << '|';
			file << users[i].password << '|';
			file << users[i].address << '|';
			file << users[i].phone << '|';
			file << users[i].his_role;
			file << '\n';
		}
		file.close();
	}
}
void saveRequestsDataLocally()
{
	ofstream file;
	file.open("Data/RequestsData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "User ID" << "|" << "Medicine Name" << "|" << "Amount Requested\n";
		for (int i = 0; i < requestcounter; i++)
		{
			file << requests[i].userID << '|';
			file << requests[i].medicineName << '|';
			file << requests[i].amountNeeded;
			file << '\n';

		}
		file.close();
	}
}
void saveOrderDataLocally() {
	ofstream file;
	file.open("Data/OrdersData.csv", ios::out);
	if (file.is_open())
	{
		file << "sep=|\n";
		file << "Order ID" << "|" << "User ID" << "|" << "Order Date"<<'|'<<"Medicine IDs"<<'|'<<"Total Price" << '|' << "Ship Date" << '|'<<"Order State\n";
		for (int i = 0; i < OrderCounter; i++)
		{
			file << orders[i].orderID << '|';
			file << orders[i].userID << '|';
			file << orders[i].orderDate << '|';
			for (int j = 0; j < 9; j++)
			{
				file << orders[i].medicine_ID[j]<<',';
				if (orders[i].medicine_ID[j] == 0)
				{
					break;
				}
			}
			file << '|';
			file << orders[i].totalPrice << '|';
			file << orders[i].shipDate << '|';
			file << orders[i].orderState;
			file << '\n';

		}
		file.close();
	}
}
void saveAllDataLocally() {
	saveMedicineDataLocally();
	saveUserDataLocally();
	saveRequestsDataLocally();
	savePayMethodLocally();
	saveOrderDataLocally();
}

void loadMedicineDataToArr() {
	fstream file;
	file.open("Data/MedicineData.csv", ios::in);
	if (file.is_open())
	{
		string data;
		getline(file, data);                  //Skips a data
		getline(file, data);                  //Skips a data
		for (int i = 0; getline(file, data, '|'); i++)
		{
			medicines[i].ID = stoi(data);
			//cout << medicines[i].ID << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].name = data;
			//cout << medicines[i].name << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].category = data;
			//cout << medicines[i].category << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].description = data;
			//cout << medicines[i].description << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].concentration = data;
			//cout << medicines[i].concentration << '\n';         //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			/*if (medicines[i].quantity_in_stock <= 1)
			{
				warningOfShortage();                                       //A funtion to make later. For admins, in order to get an alert when they are low in sth >:3
			}*/
			medicines[i].quantity_in_stock = stoi(data);
			//cout << medicines[i].quantity_in_stock << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			medicines[i].availability = stoi(data);
			//cout << medicines[i].availability << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data);
			medicines[i].price = stof(data);
			//cout << medicines[i].price << '\n' << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			medicine_data++;
		}
		//cout << endl << medicine_data;
		file.close();
	}
}
void loadUserDataToArr() {
	fstream file;
	file.open("Data/UserData.csv", ios::in);
	if (file.is_open())
	{
		string data;
		getline(file, data);                  //Skips a data
		getline(file, data);                  //Skips a data
		for (int i = 0; getline(file, data, '|'); i++)
		{
			users[i].username = data;
			//cout << users[i].username << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].ID = stoi(data);
			//cout << users[i].ID << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].email = data;
			//cout << users[i].email << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].password = data;
			//cout << users[i].password << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].address = data;
			//cout << users[i].address << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			users[i].phone = data;
			//cout << users[i].phone << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data);
			if (stoi(data) == 1)
				users[i].his_role = user::Admin;
			else
				users[i].his_role = user::User;
			//cout << users[i].his_role << '\n' << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			user_data++;
		}

		file.close();
	}
}
void loadPayMethodToVec()
{
	fstream file;
	file.open("Data/payMethod.csv", ios::in);
	if (file.is_open())
	{
		string line;
		while (!file.eof())
		{
			getline(file, line);
			paymentMethods.push_back(line);
		}
		file.close();
	}
}
void loadOrderDataToArr() {
	fstream file;
	file.open("Data/OrdersData.csv", ios::in);
	if (file.is_open())
	{
		string data;
		getline(file, data);                  //Skips a data
		getline(file, data);                  //Skips a data
		for (int i = 0; getline(file, data, '|'); i++)
		{
			orders[i].orderID = stoi(data);
			//cout << medicines[i].ID << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			orders[i].userID = stoi(data);
			//cout << medicines[i].name << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			orders[i].orderDate = data;
			//cout << medicines[i].category << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			for (int j = 0; getline(file, data, ','); j++)
			{
				if (!stoi(data))
					break;
				orders[i].medicine_ID[j] = stoi(data);
			}
			getline(file, data, '|');                               //Skips a |
			//cout << medicines[i].description << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			orders[i].totalPrice = stoi(data);
			//cout << medicines[i].concentration << '\n';         //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data, '|');
			orders[i].shipDate = data;
			//cout << medicines[i].quantity_in_stock << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			getline(file, data);
			orders[i].orderState = stoi(data);
			//cout << medicines[i].availability << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			//cout << medicines[i].price << '\n' << '\n';             //Testing the outcomes. Best to keep here, so don't delete//
			OrderCounter++;
		}
	//cout << endl << medicine_data;
	file.close();
	}
}
void loadAllDataToArr() {
	loadMedicineDataToArr();
	loadUserDataToArr();
	loadPayMethodToVec();
	loadOrderDataToArr();
}

void saveOneUserDataLocally() {
	fstream file;
	file.open("Data/UserData.csv", ios::app);
	if (file.is_open())
	{
		file << users[user_data].username << '|';
		file << users[user_data].ID << '|';
		file << users[user_data].email << '|';
		file << users[user_data].password << '|';
		file << users[user_data].address << '|';
		file << users[user_data].phone << '|';
		file << users[user_data].his_role;
		file << '\n';
	}
	file.close();
}
void saveOneMedDataLocally() {
	fstream file;
	file.open("Data/MedicineData.csv", ios::app);
	if (file.is_open())
	{
		file << medicines[medicine_data].ID << '|';
		file << medicines[medicine_data].name << '|';
		file << medicines[medicine_data].category << '|';
		file << medicines[medicine_data].description << '|';
		file << medicines[medicine_data].concentration << '|';
		file << medicines[medicine_data].quantity_in_stock << '|';
		file << medicines[medicine_data].availability << '|';
		file << medicines[medicine_data].price;
		file << '\n';
	}
	file.close();
}