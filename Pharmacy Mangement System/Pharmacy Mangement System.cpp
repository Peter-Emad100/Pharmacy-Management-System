#include <iostream>
using namespace std;
struct medicine {
	int ID;
	string name;
	string description;
	bool availability;
	string category;
	float price;
	int quantity_in_stock;
};
medicine medicines[100];
struct user {
	int ID;
	string username;
	string password;
	string email;
	string address;
	string phone;
	enum role{ User, Admin };
};
user users[100];
struct order {
	int userID;
	string orderDate;
	int medicine_ID[10];
	float totalPrice;
	string shipDate;
};
order orders[100];
int main()
{

}
