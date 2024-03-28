//**********Libraries***********//
#include <string_view>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

//**********Declarations***********//

const int Size = 100;
const int medicine_data = 7;

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
medicine medicines[Size] = {};

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
user users[Size] = {};

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

void saveMedicineDataLocally() {
	fstream data;
	data.open("MedicineData.csv", ios::out);
	if (data.is_open())
	{
		data << "sep=|" << endl;
		data << "Name" << "|" << "Category" << "|" << "Description" << "|" << "ID" << "|" << "Quantity in stock" << "|" << "Availabilty" << "|" << "Price" << endl;
		for (int i = 0; i < 10; i++)
		{
			data << medicines[i].name << "|";
			data << medicines[i].category << "|";
			data << medicines[i].description << "|";
			data << medicines[i].ID << "|";
			data << medicines[i].quantity_in_stock << "|";
			data << medicines[i].availability << "|";
			data << medicines[i].price;
			data << endl;
		}
		data.close();
	}
}

void saveMedicineDataToArr() {
	fstream data;
	data.open("MedicineData.csv", ios::in);
	if (data.is_open())
	{
		string line;
		getline(data, line);                  //Skips a line
		getline(data, line);                  //Skips a line
		for (int i = 0; i < 10; i++)
		{
			getline(data, line, '|');
			medicines[i].name = line;
			//cout << medicines[i].name << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line, '|');
			medicines[i].category = line;
			//cout << medicines[i].category << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line, '|');
			medicines[i].description = line;
			//cout << medicines[i].description << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line, '|');
			medicines[i].ID = stoi(line);
			//cout << medicines[i].ID << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line, '|');
			/*if (medicines[i].quantity_in_stock <= 1)
			{
				warningOfShortage();
			}*/
			medicines[i].quantity_in_stock = stoi(line);
			//cout << medicines[i].quantity_in_stock << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line, '|');
			medicines[i].availability = stoi(line);
			//cout << medicines[i].availability << endl;             //Testing the outcomes. Best to keep here, so don't delete//
			getline(data, line);
			medicines[i].price = stof(line);
			//cout << medicines[i].price << endl << endl;             //Testing the outcomes. Best to keep here, so don't delete//
		}
		data.close();
	}
}

void searchForMedicineByCategory() {
	string category;
	cin >> category;
	bool found = false;
	for (int i = 0; i < Size; i++) {
		if (category == medicines[i].category) {
			cout << medicines[i].name << " -- " << medicines[i].ID << " -- " << medicines[i].category << endl;
			found = true;
		}
	}
	if (found == false) {
		cout << "there is no medicine meet this category" << endl;
	}
}

void showOrderReciept(order lastOrder) {
	cout << "order date : " << lastOrder.orderDate << endl;
	int i = 0;
	while (lastOrder.medicine_ID[i] != 0) {
		cout << "medicine id : " << lastOrder.medicine_ID[i] << endl;
		i++;
	}
	cout << "user id : " << lastOrder.userID << endl;
	cout << "total price : " << lastOrder.totalPrice << endl;
	cout << "ship date : " << lastOrder.shipDate << endl;
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
bool searchForMedicineByName() {
	string name;
	cout << "Enter the medicine name" << endl;
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
			cout << medicines[i].ID<<" -- " << medicines[i].name<<" -- " <<medicines[i].availability<<" -- "<<medicines[i].category<<" -- " << medicines[i].price <<" -- "<< medicines[i].quantity_in_stock << endl;
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

int main()
{
	dataForTestPurposes();
	saveMedicineDataLocally();
	saveMedicineDataToArr();
}