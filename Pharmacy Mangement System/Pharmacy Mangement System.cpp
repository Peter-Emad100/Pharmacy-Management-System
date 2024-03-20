//**********Libraries***********//

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
medicine medicines[Size];

struct user {
	int ID;
	string username;
	string password;
	string email;
	string address;
	string phone;
	enum role { User, Admin };
};
user users[Size];

struct order {
	int userID;
	string orderDate;
	int medicine_ID[10];
	float totalPrice;
	string shipDate;
};
order orders[Size];

//**********Functions***********//

void saveMedicineDataLocally() {
	fstream data;
	data.open("MedicineData.txt", ios::out);
	if (data.is_open())
	{
		for (int i = 0; i < 10; i++)
		{
			data << medicines[i].ID << endl;
			data << medicines[i].name << endl;
			data << medicines[i].quantity_in_stock << endl;
			data << medicines[i].availability << endl;
			data << medicines[i].category << endl;
			data << medicines[i].price << endl;
			data << medicines[i].description << endl << endl;
		}
		data.close();
	}
}

void saveMecicineDataToArr() {
	fstream data;
	data.open("MedicineData.txt", ios::in);
	if (data.is_open())
	{
		for (int i = 0; i < 10; i++)
		{
			data >> medicines[i].ID;
			cout << medicines[i].ID << endl;
			data >> medicines[i].name;
			cout << medicines[i].name << endl;
			data >> medicines[i].quantity_in_stock;
			cout << medicines[i].quantity_in_stock << endl;
			data >> medicines[i].availability;
			cout << medicines[i].availability << endl;
			data >> medicines[i].category;
			cout << medicines[i].category << endl;
			data >> medicines[i].price;
			cout << medicines[i].price << endl;
			getline(data, medicines[i].description);
			cout << medicines[i].description << endl;
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

int main()
{
	medicines[0].initialize(1, "Paracetamol", "Pain reliever and fever reducer", true, "Analgesic", 5.99, 100);
	medicines[1].initialize(2, "Lisinopril", "Used to treat high blood pressure", true, "Antihypertensive", 10.49, 50);
	medicines[2].initialize(3, "Omeprazole", "Treats heartburn, stomach ulcers, and gastroesophageal reflux disease (GERD)", true, "Gastrointestinal", 7.25, 80);
	medicines[3].initialize(4, "Atorvastatin", "Lowers high cholesterol and triglycerides", true, "Lipid-lowering agent", 15.75, 30);
	medicines[4].initialize(5, "Metformin", "Treats type 2 diabetes", true, "Antidiabetic", 8.99, 60);
	medicines[5].initialize(6, "Amoxicillin", "Antibiotic used to treat bacterial infections", true, "Antibiotic", 6.50, 120);
	medicines[6].initialize(7, "Alprazolam", "Treats anxiety and panic disorders", true, "Anxiolytic", 12.99, 40);
	medicines[7].initialize(8, "Ibuprofen", "Nonsteroidal anti-inflammatory drug (NSAID)", true, "Analgesic", 4.75, 200);
	medicines[8].initialize(9, "Cetirizine", "Antihistamine used for allergy relief", true, "Antihistamine", 9.25, 70);
	medicines[9].initialize(10, "Ranitidine", "Reduces stomach acid production to treat heartburn and ulcers", true, "Gastrointestinal", 6.99, 90);
	saveMecicineDataToArr();
}