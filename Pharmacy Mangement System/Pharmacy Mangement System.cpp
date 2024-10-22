//**********Libraries***********//
#include <string_view>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include<vector>
#include <ctime>
#include "SaveData.h"
using namespace std;

//**********Declarations***********//
extern vector<string> paymentMethods;
const int Size = 100;
int medicine_data = 0;
int user_data = 0;
int requestcounter = 0;
int OrderCounter = 0;
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
user newUser;
user currentUser; //Temp to keep the current user's data

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
order orders[Size] = {};
void manage_orders(order orders[Size]);
struct request {
	int userID;
	string medicineName;
	int amountNeeded;
};
request requests[15];

//********Function Declares***********//
void mainInterface();
void dataForTestPurposes();
bool isUsernameTaken(string username);
bool isMedNameTaken(string MedName);
void signUp();
bool isValidEmail(string email);
bool isValidPhone(string phone);
bool validateUser(string username, string password, user& currentUser);
void logInInterface();
void userPermissions();
void adminPermissions();
void warningStock();
void addMedicine();
void removeMedicine();
void editMedicine();
void editUserCredentials(int index);
bool searchForMedicineByName();
void searchForMedicineByCategory();
void makeOrder(string medicineIDS, string quantity, int payment_method);
void showOrderReceipt(order lastOrder,string current_time);
void makeRequest(string _username, string _medicineName, int _amountReq);
void showAllPreviousOrders();
void addUser();
void updateUser();
void removeUser();
void logOut();
void managePaymentMethods();
void showPaymentMehtode(vector<string> x);
void addPayment();
void removePayment();
void manageOrders(order orders[Size]);


//**********Functions***********//

void mainInterface() {
	system("cls");
	cout << "             =======================             \n";
	cout << "             |   Pharmacy System   |              \n";
	cout << "====================================================\n";
	cout << "Choose one of the following option\n";
	cout << "1) Login to the system\t 2) SignUp \n";
	cout << "(Enter 0 if you want to exit the system)\n";
	cout << "Option: ";
	int option;
	do{
		cin>>option;
		if (option > 2 || option < 0)
		{
			cout << "Invalid number. Please enter a number from the numbers above: ";
			continue;
		}
	} while (option > 2 || option < 0);
	if (option == 1)
		logInInterface();
	else if (option == 2)
		signUp();
	else
	{
		saveAllDataLocally();
		exit(0);
	}
}

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
	orders[0].initialize(1, "2024-03-27", medicine1, 500.0, "2024-03-27", 1, false);
	int medicine2[] = { 4, 5, 0 };
	orders[1].initialize(2, "2024-03-28", medicine2, 300.0, "2024-03-28", 2, true);
	int medicine3[] = { 9, 4,5,7 ,0 };
	orders[2].initialize(3, "2024-03-29", medicine3, 3000.0, "2024-03-29", 3, true);


}

bool isUsernameTaken(string username) {
	int i = 0;
	while (users[i].ID != 0) {
		if (users[i].ID == -1)
		{
			++i;
			continue;
		}

		if (users[i].username == username) {
			return true;
		}
		++i;
	}
	return false;
}

bool isMedNameTaken(string MedName) {
	int i = 0;
	while (medicines[i].ID != 0) {
		if (medicines[i].ID == -1)
		{
			++i;
			continue;
		}

		if (medicines[i].name == MedName) {
			return true;
		}
		++i;
	}
	return false;
}

void signUp() {

	int id = user_data + 1; // Next available ID

	newUser.ID = id;

	do {
		cout << "Enter your username: ";
		cin >> newUser.username;

		if (isUsernameTaken(newUser.username))
			cout << "A user with that username already exists. Please enter a different username: ";


	} while (isUsernameTaken(newUser.username)); //Checks if username is already in our database or no.

	cout << "Enter your password: ";
	cin >> newUser.password;

	cout << "Enter your e-mail: ";
	//To check the email if it's valid//
	bool valid=true;
	do {
		cin >> newUser.email;
		valid = isValidEmail(newUser.email);
		if (!valid)
			cout << "Invalid email. Please enter a valid email: ";
	} while (!valid);
	///////////////////////////////////

	cout << "Enter your address: ";
	cin.ignore(1, '\n');
	getline(cin, newUser.address);

	cout << "Enter your phone: ";
	do {
		cin >> newUser.phone;
		valid = isValidPhone(newUser.phone);
		if (!valid)
			cout << "Invalid phone number. Please enter a valid phone number: ";
	} while (!valid);

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
	char again;
	do {
		cout << "Do you want to make another operations? (y/n)\n";
		cin >> again;
		if (again != 'y' && again != 'n')
			cout << "Invalid character. Please Enter a valid character (y for yes/n for no)\n";
		else if (again == 'y')
			mainInterface();
		else
			exit(0);
	} while (again != 'y' && again != 'n');
}

bool isValidEmail(string email) {
	if ((email[0] <= 'a' && email[0] >= 'z') || (email[0] <= 'A' && email[0] >= 'Z')) //Checks if the first input a letter or no
		return 0;
	int countAt = 0, countDot = 0; //Counts the number of "at"s and dots
	int at = -1, dot = -1;
	for (int i = 0; i < email.size(); i++)
	{
		if (email[i] == '@')
		{
			countAt++;
			at = i;
		}
		else if (email[i] == '.')
		{
			countDot++;
			dot = i;
		}
	}
		if (at == -1 || dot == -1) //Checks if there's an at and a dot
			return 0;
		if (at > dot) //Checks if there's an at after the dot
			return 0;
		if (countAt > 1 || countDot > 1) //Checks if there are more than one of them
			return 0;
		if (dot >= (email.size() - 1)) // Checks if the Dot is present at the end
			return 0;
		else
			return 1;
}

bool isValidPhone(string phone) {
	for (int i = 0; i < phone.size(); i++)
	{
		if ((phone[i] >= 'a' && phone[i] <= 'z') || (phone[i] >= 'A' && phone[i] <= 'Z')) //Checks if the first input a letter or no
			return 0;
	}
}

bool validateUser(string username, string password, user& currentUser)
{
	int userIndex = 0;

	// Loop through the users until a user with userID = 0 is found,
	// indicating that there are no more users in our database
	while (users[userIndex].ID != 0)
	{
		if (users[userIndex].ID == -1)
		{
			userIndex++;
			continue;
		}

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
			bool adminFirstTime = true;
			char again;
			do {
				system("cls");
				cout << "             ============================             \n";
				cout << "             |   Pharmacy System Menu   |              \n";
				cout << "========================================================\n";
				cout << "Log in success. Welcome back, " << currentUser.username << " :D\n-------------------------------------------\n";
				if (currentUser.his_role == user::User)
				{
					userPermissions();
					cout << "-------------------------------------------\n";
					do{
						cout << "Option: ";
						cin >> chosenOption;
						if (chosenOption == 1 /*Search the medicine by its name*/)
							searchForMedicineByName();
						else if (chosenOption == 2 /*Search the medicine by its category*/)
							searchForMedicineByCategory();
						else if (chosenOption == 3 /* Make an order*/)
						{
							cout << "Enter the ID of different medicines separated by a space: ";
							cin.ignore(1, '\n');
							string MedID;
							getline(cin, MedID);
							cout << "Enter the number of each different medicines separated by a space: ";
							cin.ignore(1, '\n');
							string MedNum;
							getline(cin, MedNum);
							int PayMethod;
							cout << "Enter the number of the payment method: \n";
							showPaymentMehtode(paymentMethods);
							cout << "Payment method no. : ";
							cin >> PayMethod;
							makeOrder(MedID, MedNum, PayMethod);
						}
						else if (chosenOption == 4)
						{
							string medicineName;
							int amountrequested;
							cout << "Enter medicine name:\n";
							cin >> medicineName;
							cout << "Enter the amount you need:\n";
							cin >> amountrequested;
							makeRequest(currentUser.username, medicineName, amountrequested);
						}
						else if (chosenOption == 5)
							showAllPreviousOrders();
						else if (chosenOption == 6 /*Log out*/)
							logOut();
						else
							cout << "Invalid number. Please enter a number from the given numbers above: ";
					}while (chosenOption > 6 || chosenOption < 0);
				}
				else
				{
					adminPermissions();
					cout << "-------------------------------------------\n";
					if (adminFirstTime)
						warningStock();
					cout << "Option: ";
					do{
						cin >> chosenOption;
						if (chosenOption == 1)
							addUser();
						else if (chosenOption == 2)
						{
							cout << "Enter the ID of the user to edit: ";
							int index;
							cin >> index;
							editUserCredentials(index);
						}
						else if (chosenOption == 3)
							removeUser();
						else if (chosenOption == 4)
							addMedicine();
						else if (chosenOption == 5)
							removeMedicine();
						else if (chosenOption == 6)
							editMedicine();
						else if (chosenOption == 7)
							manageOrders(orders);
						else if (chosenOption == 8)
							managePaymentMethods();
						else if (chosenOption == 9 /**/)
							searchForMedicineByName();
						else if (chosenOption == 10)
							searchForMedicineByCategory();
						else if (chosenOption == 11 /*Request drug*/)
						{
							string medicineName;
							int amountrequested;
							cout << "Enter medicine name:\n";
							cin >> medicineName;
							cout << "Enter the amount you need:\n";
							cin >> amountrequested;
							makeRequest(currentUser.username, medicineName, amountrequested);
						}
						else if (chosenOption == 12)
							showAllPreviousOrders();
						else if (chosenOption == 13)
							logOut();
						else
							cout << "Invalid number. Please enter a number from the given numbers above: ";
					}while (chosenOption > 13 || chosenOption < 0);
				}
				adminFirstTime = false;
				do{
					cout << "Do you want to make another operations? (y/n)\n";
					cin >> again;
					if (again != 'y' && again != 'n')
						cout << "Invalid character. Please Enter a valid character (y for yes/n for no)\n";
				} while (again != 'y' && again != 'n');
			} while (again == 'y');
		}


		else
		{
			cout << "Invalid credentials. The username or password you entered is incorrect. Please try again.\n";

		}
	}
}

void checkContinue() {
	cout << "Do you want to continue? (y for Yes / n for No)\n";
	char letter;
	cin >> letter;
	if (letter == 'y');
		
}

void userPermissions() {
	cout << "1- Search for medicine by name\n";
	cout << "2- Search for medicine by category\n";
	cout << "3- Add order\n";
	cout << "4- Request drug\n";
	cout << "5- View all previous orders\n";
	cout << "6- Log out\n";
}

void adminPermissions() {
	cout << "1- Add new user\n";
	cout << "2- Update user information\n";
	cout << "3- Remove user\n";
	cout << "4- Add new medicine\n";
	cout << "5- Remove medicine\n";
	cout << "6- Edit medicine\n";
	cout << "7- Manage orders\n";
	cout << "8- Manage payments\n";
	cout << "9- Search for medicine by name\n";
	cout << "10- Search for medicine by category\n";
	cout << "11- Request drug\n";
	cout << "12- View all previous orders\n";
	cout << "13- Log out\n";
}

void warningStock() {
	for (int i = 0; i < medicine_data; i++)
	{
		if (medicines[i].quantity_in_stock <= 0)
			cout << "Warning! We have shortage of " << medicines[i].name << " as we have " << medicines[i].quantity_in_stock << " From it!\n";
	}
}

void addMedicine() {
	medicine newMedicine;
	newMedicine.ID = medicine_data + 1; //medicine_data is the number of the amount of medicines aka the last ID minus one since the arr is zero based :P
	newMedicine.availability = true;
	
	cout << "Enter the name of the medicine: ";
	do {
		cin >> newMedicine.name;
		if (isMedNameTaken(newMedicine.name))
			cout << "A medicine with that name already exists. Please enter a different name: ";
	} while (isMedNameTaken(newMedicine.name));
	
	cout << "Enter the description of the medicine: ";
	cin >> newMedicine.description;
	
	cout << "Enter the concentration of the medicine: ";
	cin >> newMedicine.concentration;
	
	cout << "Enter the category of the medicine: ";
	cin >> newMedicine.category;
	
	cout << "Enter the price of the medicine: ";
	do {
		cin >> newMedicine.price;
		if (newMedicine.price <= 0)
			cout << "The price of the medicine can't be equal or less than 0. Please enter a positive number: ";
	} while (newMedicine.price <= 0);

	cout << "Enter the quantity in stock of the medicine: ";
	do {
		cin >> newMedicine.quantity_in_stock;
		if (newMedicine.quantity_in_stock < 0)
			cout << "The quantity is stock of the medicine can't be less than 0. Please enter a positive number: ";
		else if (newMedicine.quantity_in_stock == 0)
			newMedicine.availability = false;
	} while (newMedicine.quantity_in_stock < 0);

	medicines[medicine_data] = newMedicine;
	saveOneMedDataLocally();
	medicine_data++;
}

void removeMedicine() {
	int medID;
	bool medFound = false;

	while (!medFound) {
		cout << "Enter the ID of the medicine you are willing to remove: ";
		cin >> medID;

		if (medID <= 0) {
			cout << "Invalid ID.\n";
			continue;
		}


		int i = 0;
		while (medicines[i].ID != 0) {
			if (medicines[i].ID == medID) {
				medFound = true;
				medicines[i].ID = -1;
				cout << "Medicine with ID: " << medID << " has been removed.\n";
				saveMedicineDataLocally();
				break;
			}
			++i;


		}

		if (!medFound) {
			cout << "Medicine not found.\n";
		}
	}
}

void editMedicine() {
	int index;
	cout << "Enter the ID of the medicine that you'd love to change: ";
	do {
		cin >> index;
		if (index > medicine_data)
			cout << "There's no Medicine with such an ID\nRe-enter the ID of the medicine that you'd love to change: ";
	} while (index > medicine_data);
	cout << "What are you willing to change ?\n";
	cout << "1- Price\n";
	cout << "2- Name\n";
	cout << "3- Description\n";
	cout << "4- category\n";
	cout << "5- concentration\n";
	cout << "6- Quantity in stock\n";
	int choice;
	do {
		cin >> choice;
		if (choice == 1)
		{
			cout << "Enter the new price: ";
			cin >> medicines[index-1].price;
		}
		else if (choice == 2)
		{
			cout << "Enter the new name: ";
			cin >> medicines[index-1].name;
		}
		else if (choice == 3)
		{
			cout << "Enter the new description: ";
			cin.ignore(1, '\n');
			getline(cin, medicines[index-1].description);
		}
		else if (choice == 4)
		{
			cout << "Enter the new category: ";
			cin.ignore(1, '\n');
			getline(cin, medicines[index-1].category);
		}
		else if (choice == 5)
		{
			cout << "Enter the new concentration: ";
			cin.ignore(1, '\n');
			getline(cin, medicines[index-1].concentration);
		}
		else if (choice == 6)
		{
			cout << "Enter the new quantity in stock: ";
			cin >> medicines[index-1].quantity_in_stock;
			if (medicines[index - 1].quantity_in_stock == 0)
				medicines[index - 1].availability = false;
			else
				medicines[index - 1].availability = true;
		}
		else
		{
			cout << "Invalid choice. Please enter a number from the numbers above (1-6).\n";
		}
	} while (choice > 6);
	saveMedicineDataLocally();
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
		cout << "there is no medicine meet this name\n";
		int ifwant;
		cout << "Want to make a request?\nType the number of required medicines (Enter 0 to cancel)\n" << endl;
		cin >> ifwant;
		if (ifwant != 0)
		{
			string& medicineName = name; //Simply gets the entered name of the medicine above and we couldn't find it
			int& amountrequested = ifwant; //Simply gets the entered number of medicine above
			makeRequest(currentUser.username, name, amountrequested);
		}
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
		cout << "Want to make a request?choose(any number for yes/ 0 for no)" << endl;
		cin >> ifwant;
		if (ifwant != 0)
		{
			string medicineName;
			int amountrequested;
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

void makeOrder(string medicineIDS, string quantity, int payment_method) {
	// this function might take some time to be understood ... 
	// i tried my best to make it more understandable with comments
	//Good Luck
	order lastyorder = {};
	int length_medicines = medicineIDS.size();
	int length_quantity = quantity.size();
	int first_space_pos = -1, second_space_pos = -1;
	int first_space_pos_quan = -1, second_space_pos_quan = -1;
	int quantity_arr[11] = {};
	int pay_num = 2000;
	int j = 0;
	bool error_format = false;
	bool error_id = false;
	bool quantity_problem = false;
	bool pay_in_range = false;
	string current_time;
	// simple general try and catch code because this can execute some exceptions dependent on user input format
	try {
		// a loop to split string input into medicine id array
		for (int i = 0; i < length_medicines + 1; i++) {
			if (medicineIDS[i] == ' ' || medicineIDS[i] == '\0') {
				first_space_pos = second_space_pos + 1;
				second_space_pos = i;
				lastyorder.medicine_ID[j] = stoi(medicineIDS.substr(first_space_pos, (second_space_pos - first_space_pos)));
				j++;
			}
		}
	}
	catch (...) {
		cout << "medicine ids is in a wrong format ... this order will not be excuted" << endl;
		error_format = true;
	}
	j = 0;
	// simple general try and catch code because this can execute some exception dependent on user input format
	try {
		// a loop to split string input into quantity array
		for (int i = 0; i < length_quantity + 1; i++) {
			if (quantity[i] == ' ' || quantity[i] == '\0') {
				first_space_pos_quan = second_space_pos_quan + 1;
				second_space_pos_quan = i;
				quantity_arr[j] = stoi(quantity.substr(first_space_pos_quan, (second_space_pos_quan - first_space_pos_quan)));
				j++;
			}
		}
	}
	catch (...) {
		cout << "quantities is in a wrong format ... this order will not be excuted" << endl;
		error_format = true;
	}
	try {
		pay_num = payment_method;
	}
	catch (...) {
		cout << "payment method is in a wrong format ... this order will not be excuted" << endl;
		error_format = true;
	}
	// the order will be done only if the input was in the right format
	if (error_format == false) {
		int x = 0;
		bool found = false;
		while (x != 10 && lastyorder.medicine_ID[x] != 0) {
			found = false;
			for (int k = 0; medicines[k].ID != 0; k++) {
				if (medicines[k].ID == lastyorder.medicine_ID[x]) {
					if (medicines[k].quantity_in_stock >= quantity_arr[x]) {
						lastyorder.totalPrice += medicines[k].price * quantity_arr[x];
						found = true;
					}
					else {
						quantity_problem = true;
					}
				}
			}
			if (found == false) {
				error_id = true;
			}
			x++;
		}
		//check if payment method in range
		if (pay_num <= paymentMethods.size() && pay_num > 0) {
			pay_in_range = true;
		}
		//quantity will decrease only if there is no error detected
		if (quantity_problem == false && error_format == false && quantity_problem == false && pay_in_range == true) {
			x = 0;
			while (x != 10 && lastyorder.medicine_ID[x] != 0) {
				for (int k = 0; medicines[k].ID != 0; k++) {
					if (medicines[k].ID == lastyorder.medicine_ID[x]) {
						medicines[k].quantity_in_stock -= quantity_arr[x];
					}
				}
				x++;
			}
			lastyorder.userID = currentUser.ID;
			time_t t = time(0); // Get time now
			tm now;
			localtime_s(&now, &t);
			lastyorder.orderDate = to_string(now.tm_year + 1900) + '-' + to_string(now.tm_mon + 1) + '-' + to_string(now.tm_mday);
			lastyorder.orderState = 0;
			lastyorder.shipDate = to_string(now.tm_year + 1900) + '-' + to_string(now.tm_mon + 1) + '-' + to_string(now.tm_mday);
			current_time = to_string(now.tm_hour) + ':' + to_string(now.tm_min) + ':' + to_string(now.tm_sec);

		}
		//order will be cleared if a medicine id is wrong
		if (error_id == true) {
			cout << "you entered a wrong id , your order will not be excuted" << endl;
			lastyorder = {};
			current_time = "";
		}
		//order will be cleared if we don't have the quantity needed
		if (quantity_problem) {
			cout << "you entered unavailable quantity , your order will not be excuted" << endl;
			lastyorder = {};
			current_time = "";
		}
		if (pay_in_range == false) {
			cout << "wrong payment method, your order will not be excuted" << endl;
			lastyorder = {};
			current_time = "";
		}
		//order will be saved if there isn't a single error
		if (error_id == false && error_format == false && quantity_problem == false) {
			int i = 0;
			for (i; orders[i].orderID != 0; i++);
			orders[i] = lastyorder;
		}
		showOrderReceipt(lastyorder, current_time);
	}
}

void showOrderReceipt(order lastOrder , string current_time) {
	cout << "order date : " << lastOrder.orderDate << '\n';
	int i = 0;
	while (lastOrder.medicine_ID[i] != 0) {
		cout << "medicine id : " << lastOrder.medicine_ID[i] << '\n';
		i++;
	}
	cout << "user id : " << lastOrder.userID << '\n';
	cout << "total price : " << lastOrder.totalPrice << '\n';
	cout << "ship date : " << lastOrder.shipDate << '\n';
	cout << "current time :" << current_time << '\n';
}

void makeRequest(string _username, string _medicineName, int _amountReq)
{
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
				break;
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
		saveRequestsDataLocally();
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
void showPaymentMehtode(vector<string> x)
{
	int c = 1;
	for (auto it = x.begin(); it != x.end(); ++it)
	{
		cout << "[" << c << "] " << *it << endl;
		c++;
	}
	cout << '\n';
}
void managePaymentMethods()
{
	int chooseP;

	int m = 0;
	while (true)
	{
		if (m > 0)
		{
			cout << "invalid input try again from the list given : \n";
		}
		cout << "Edit payment methods : " << "\n";
		cout << "[1] show existed payment methods\n";
		cout << "[2] add new payment method      \n";
		cout << "[3] delete payment method       \n";
		cout << "choose from the above : "; cin >> chooseP;
		m++;
		if (chooseP >= 1 && chooseP <= 3)
		{
			break;
		}
		system("cls");
	}


	switch (chooseP)
	{
		case 1:
		{
			system("cls");
			cout << "=== payment methods available now are === \n";
			showPaymentMehtode(paymentMethods);
			cout << "Do you want to add or delete an option?  \n";
			cout << "1) Add a new paymethod\t 2)Delete a paymethod\n (Enter 0 if the answer is no)\n";
			cout << "Option no. :";
			do {
				cin >> chooseP;
				if (chooseP == 1)
					addPayment();
				else if (chooseP == 2)
					removePayment();
				else if (chooseP == 0)
					return;
				else
				{
					cout << "Ivalid number. Please enter a number from the options above :";
				}
			} while (chooseP < 0 || chooseP>2);
			break;
		}
		case 2:
		{
			addPayment();
			break;
		}
		case 3:
		{
			removePayment();
			break;
		}
	}
}

void addPayment() {
	system("cls");
	string newMethod;
	cout << "=== Adding new Payment method ===\n";
	cout << "insert method's name: ";
	cin.ignore(1, '\n');
	getline(cin, newMethod, '\n');
	cout << "method now available are: \n";
	paymentMethods.push_back(newMethod);
	showPaymentMehtode(paymentMethods);
	savePayMethodLocally();
	int chooseP;
	cout << "Do you want to add or delete an option?  \n";
	cout << "1) Add a new paymethod\t 2)Delete a paymethod\n (Enter 0 if the answer is no)\n";
	cout << "Option no. :";
	do {
		cin >> chooseP;
		if (chooseP == 1)
			addPayment();
		else if (chooseP == 2)
			removePayment();
		else if (chooseP == 0)
			return;
		else
		{
			cout << "Ivalid character. Please enter a number from the options above :";
		}
	} while (chooseP < 0 || chooseP>2);
}

void removePayment() {
	system("cls");
	vector<string>::iterator it = paymentMethods.begin();
	int chooseD;
	cout << "=== delete payment method ===\n";
	showPaymentMehtode(paymentMethods);
	cout << "\choose method to delete from the given list: "; cin >> chooseD;
	it = paymentMethods.begin() + chooseD - 1;
	paymentMethods.erase(it, it + 1);
	cout << "\nmethods available now is : \n";
	showPaymentMehtode(paymentMethods);
	savePayMethodLocally();
	int chooseP;
	cout << "Do you want to add or delete an option?  \n";
	cout << "1) Add a new paymethod\t 2)Delete a paymethod\n (Enter 0 if the answer is no)\n";
	cout << "Option no. :";
	do {
		cin >> chooseP;
		if (chooseP == 1)
			addPayment();
		else if (chooseP == 2)
			removePayment();
		else if (chooseP == 0)
			return;
		else
		{
			cout << "Ivalid character. Please enter a number from the options above :";
		}
	} while (chooseP < 0 || chooseP>2);
	
}

void addUser()
{
	int id = user_data + 1;
	newUser.ID = id;

	cout << "Please provide the following information for the new user:\n";
	do {
		cout << "Username: ";
		cin >> newUser.username;

		if (isUsernameTaken(newUser.username))

			cout << "A user with that username already exists. Please enter a different username: ";


	} while (isUsernameTaken(newUser.username));


	cout << "Password: ";
	cin >> newUser.password;
	cout << "Enter your e-mail: ";
	//To check the email if it's valid//
	bool valid = true;
	do {
		cin >> newUser.email;
		valid = isValidEmail(newUser.email);
		if (!valid)
			cout << "Invalid email. Please enter a valid email: ";
	} while (!valid);
	///////////////////////////////////

	cout << "Enter your address: ";
	cin.ignore(1, '\n');
	getline(cin, newUser.address);

	cout << "Enter your phone: ";
	do {
		cin >> newUser.phone;
		valid = isValidPhone(newUser.phone);
		if (!valid)
			cout << "Invalid phone number. Please enter a valid phone number: ";
	} while (!valid);

	int roleChoice;

	do {
		cout << "Pick the new role\n1-User\n2-Admin\n";
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
	} while (roleChoice != 1 && roleChoice != 2);

	users[id - 1] = newUser;

	cout << "Congratulations! A new user has been successfully created.\n";

	saveOneUserDataLocally();

	user_data++;										 // Increment user_data to keep track of the total number of users

}


void updateUser() {
	int userID;
	bool userFound = false;
	int index = -1;

	while (!userFound) {
		cout << "Enter the ID of the user you are willing to update: ";
		cin >> userID;

		if (userID <= 0) {
			cout << "Invalid ID.\n";
			continue;
		}

		int i = 0;
		while (users[i].ID != 0) {
			if (users[i].ID == userID) {
				userFound = true;
				index = i;
				break;
			}

			++i;
		}

		if (!userFound) {
			cout << "User not found.\n";
		}
	}


	cout << "---------------------------------\n";
	cout << "Current username: " << users[index].username << "\n";
	cout << "Current password: " << users[index].password << "\n";
	cout << "Current email: " << users[index].email << "\n";
	cout << "Current address: " << users[index].address << "\n";
	cout << "Current phone number: " << users[index].phone << "\n";


	if (users[index].his_role == user::User)
		cout << "Current role: User\n";
	else if (users[index].his_role == user::Admin)
		cout << "Current role: Admin\n";



	char choice;
	int roleChoice;
	do {
		cout << "What do you want to edit?\n";
		cout << "1. Username\n";
		cout << "2. Password\n";
		cout << "3. Email\n";
		cout << "4. Address\n";
		cout << "5. Phone Number\n";
		cout << "6. Role\n\n";
		cout << "Enter your choice: ";

		int option;
		cin >> option;
		string newUsername;
		bool userNameChanged = false;
		switch (option) {
		case 1:
			
			
			while (!userNameChanged) {
				cout << "Enter new username: ";
				cin >> newUsername;

				if (newUsername == users[index].username) {
					cout << "The new username is the same as the current one. Please enter a different username.\n";
					
				}
				else if (isUsernameTaken(newUsername)) {
					cout << "A user with that username already exists. Please enter a different username.\n";
					
				}
				else {
					users[index].username = newUsername;
					cout << "Username updated successfully!\n";
					userNameChanged = true;
					
				}
			}
			break;
		case 2:
			cout << "Enter new password: ";
			cin >> users[index].password;
			break;
		case 3:
			cout << "Enter new email: ";
			cin >> users[index].email;
			break;
		case 4:
			cout << "Enter new address: ";
			cin.ignore(1, '\n');
			getline(cin, users[index].address);
			break;
		case 5:
			cout << "Enter new phone number: ";
			cin >> users[index].phone;
			break;
		case 6:
			if (users[index].his_role == user::User) {
				users[index].his_role = user::Admin;
				cout << "Role updated successfully! Your new role is Admin.\n";
				
			}
			else {
				users[index].his_role = user::User;
				cout << "Role updated successfully! Your new role is User.\n";
			}
			
			break;

		default:
			cout << "Invalid option!\n";
		}

		cout << "Would you like to make any additional edits? Choose between y/n : ";
		cin >> choice;

	} while (choice == 'y' || choice == 'Y');

	cout << "User updated successfully!\n";

	saveOneUserDataLocally();
}

void removeUser()
{
	int userID;
    bool userFound = false;

	while (!userFound) {
		cout << "Enter the ID of the user you are willing to remove: ";
		cin >> userID;

		if (userID <= 0) {
			cout << "Invalid ID.\n";
			continue;
		}


		int i = 0;
		while (users[i].ID != 0) {
			if (users[i].ID == userID) {
				userFound = true;
				users[i].ID = -1;
				cout << "User with ID: " << userID << " has been removed.\n";
				saveUserDataLocally();
				break;
			}
			++i;


		}

		if (!userFound) {
			cout << "User not found.\n";
		}
	}
}

void trackorder(order orders[])
{
	bool orderfound = false;
	int orderid = -1;
	cout << "enter id : ";
	cin >> orderid;
	for (size_t i = 0; i < 3; i++)
	{
		if (orderid == orders[i].orderID)
		{

			if (orders[i].orderState == 0)
			{
				cout << "OrderNotDelivered" << endl;

			}
			else
			{
				cout << "OrderDelivered" << endl;
			}
			orderfound = true;
		}

	}
	if (orderfound != true)
	{
		cout << "order not found enter another id " << endl;
		trackorder(orders);
	}

}
void manageOrders(order orders[Size]) {
	int ID, indx;
	bool found = 0;
	cout << "Enter the orders's ID: ";
	cin >> ID;
	cout << '\n';
	for (int i = 0; i < Size; i++) {
		if (ID == orders[i].orderID) {
			found = 1;
			indx = i;
			break;
		}
	}
	
	if (found) { // display the order first
		for (int j = 0; j < 10; j++) {
			if (orders[indx].medicine_ID[j] == 0) {
				break;
			}
			for (int i = 0; i < Size; i++) {

				if (medicines[i].ID == orders[indx].medicine_ID[j]) {
					cout << "Name of the medicine: " << medicines[i].name << "\n";
					cout << "concentraion of the medicine" << medicines[i].concentration << "\n";
					cout << "-----------------------------------------------\n";
				}
			}
		}
		cout << "Order's date: " << orders[indx].orderDate << '\n';
		cout << "Order's state: " << orders[indx].orderState << '\n';
		cout << "Ship date: " << orders[indx].shipDate << '\n';
		cout << "Total price: " << orders[indx].totalPrice << '\n';

	}
	else {
		cout << "There is no order with this ID\n";
		cout << "Please try again\n";
	}
	char answer = 'y';
	do{
		int choice;
	cout << "What do you want to edit?\n";
	cout << "1. Order's state \n";
	cout << "2. Total price \n";
	cout << "Enter your choice: ";
	cin >> choice;
	switch (choice) {
	case 1:
		if (orders[indx].orderState == 0) {
			cout << "Order' state is: ";
			if (orders[indx].orderState == 0) {
				cout << "Not Delivered \n";

			}

			else {
				cout << "Delivered \n";
			}
		}
			cout << "Do you want to change the order's state ?   choose y/n \n";
			char change;
				cin >> change;
				if (change == 'y' or change == 'Y') {
					if (orders[indx].orderState == 0) {
						orders[indx].orderState = 1;
						cout << "Orders's state is now 'Delivered' ";
					}

					else {
						if (orders[indx].orderState == 1) {
							orders[indx].orderState = 0;
							cout << "Orders's state is now 'Not Delivered' ";
						}
					}
				}
		else {
			break;
		}
			break;
		
	case 2:
		int new_price;
		cout << "Total price: " << orders[indx].totalPrice << '\n';
		cout << "Enter new price: ";
		cin >> new_price;
		cout << "\n";
		orders[indx].totalPrice = new_price;
		cout << " Total price is now: " << orders[indx].totalPrice << '\n';
		break;

	}
	
		cout << "Would you like to make any additional edits? Choose between y/n : ";
		cin >> answer;

		} while (answer == 'y' || answer == 'Y');
		cout << "Order updated successfully!\n";
}

void logOut()
{
	system("cls");
	mainInterface(); //Basically, just open the main interface again if you are willing to log out 
}

int main()
{
	loadAllDataToArr();
	//dataForTestPurposes();
	//saveOrderDataLocally();
	//loadOrderDataToArr();
	mainInterface();
	return 0;
	//addMedicine();
	//int orderTime = dateDifference("2024-03-27", "2024-05-27");
	//makeOrder(users[1].ID, "2024-03-27", "2024-05-27", orderTime, medicines);
	//showOrderReceipt(orders[0]);
	/*if (currentUser.his_role == user::User) {
		if (chosenOption == 8)
			showAllPreviousOrders();
		else if (chosenOption == 7)
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
		if (chosenOption == 14)
			showAllPreviousOrders();
		else if (chosenOption == 13)
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
		else if (chosenOption == 1)
			addUser();
		else if (chosenOption == 2)
			updateUser();
		else if (chosenOption == 3)
			removeUser();
	}
	*/
	//manage_orders(orders);
	//string orr;
	//getline(cin , orr);
	//makeOrder(orr);
	
}