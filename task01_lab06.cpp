#include <iostream>
#include <string>

using namespace std;

int N, targetSum;
string targetCode;

int totalCodes = 0;
bool isFound = false;

void findCodes(int pos, int digit, string code, int sum){
    if(pos == N){
        if(sum == targetSum){
            cout<<code<<endl;
            totalCodes++;
            if(code == targetCode){
                isFound = true;
            }
        }
        return;
    }

    if(digit > 9){
        return;
    }
    char ch = '0' + digit;

    if(!(pos == 0 && digit == 0) && code.find(ch) == string::npos && sum + digit <= targetSum){
        findCodes(pos + 1, 0, code + ch, sum + digit);
    }

    findCodes(pos, digit + 1, code, sum);
}

int main() {
    cout<<"Enter N: ";
    cin>>N;
    cout<<"Enter target sum: ";
    cin>>targetSum;
    cout<<"Enter target code: ";
    cin>>targetCode;

    cout<<"Valid codes: "<<endl;
    findCodes(0, 0, "", 0);

    if(totalCodes == 0){
        cout<<"None "<<endl;
    }
    cout<<"Target code found: "<<((isFound) ? "Yes" : "No")<<endl;

    return 0;
}