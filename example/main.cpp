#include <iostream>
#include <string>
using namespace std;

double calculate_tax(double income);
string get_tax_bracket(double income);

int main() {
    double income = 50000;

    double tax = calculate_tax(income);
    string bracket = get_tax_bracket(income);

    cout << "Your tax bracket is: " << bracket << endl;
    cout << "Your estimated tax liability is: $" << tax << endl;
    return 0;
}

double calculate_tax(double income) {
    double tax = 0;
    if (income <= 10000) {
        tax = 0;
    } else if (income <= 50000) {
        tax = 0.1 * (income - 10000);
    } else if (income <= 100000) {
        tax = 4000 + 0.2 * (income - 50000);
    } else {
        tax = 14000 + 0.3 * (income - 100000);
    }
    return tax;
}

string get_tax_bracket(double income) {
    if (income <= 10000) {
        return "0%";
    } else if (income <= 50000) {
        return "10%";
    } else if (income <= 100000) {
        return "20%";
    } else {
        return "30%";
    }
}
