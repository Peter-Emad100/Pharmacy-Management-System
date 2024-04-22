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

struct request {
	int userID;
	string medicineName;
	int amountNeeded;
};
extern request requests[15];

void saveMedicineDataLocally() {
	fstream file;
	file.open("MedicineData.csv", ios::out);
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

void savePayMethodeLocally()
{
	fstream file;
	file.open("payMethode.txt", ios::out);
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
	file.open("UserData.csv", ios::out);
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
	file.open("RequestsData.txt", ios::app);
	if (file.is_open())
	{
		file << "User ID" << " | " << "Medicine Name" << " | " << "Amount Requested\n";
		for (int i = 0; i < requestcounter; i++)
		{
			file << requests[i].userID << "       |";
			file << requests[i].medicineName << "      |";
			file << requests[i].amountNeeded << "        \n";

		}
		file.close();
	}
}

void saveAllDataLocally() {
	saveMedicineDataLocally();
	saveUserDataLocally();
	saveRequestsDataLocally();
	savePayMethodeLocally();
}

void loadMedicineDataToArr() {
	fstream file;
	file.open("MedicineData.csv", ios::in);
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
	file.open("UserData.csv", ios::in);
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

void loadPayMethodeToVec()
{
	fstream file;
	file.open("payMethode.txt", ios::in);
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

void loadAllDataToArr() {
	loadMedicineDataToArr();
	loadUserDataToArr();
	loadPayMethodeToVec();
}

void saveOneUserDataLocally() {
	fstream file;
	file.open("UserData.csv", ios::app);
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
	file.open("MedicineData.csv", ios::app);
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