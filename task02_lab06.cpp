#include <iostream>
#include <vector>
using namespace std;

int N;
long long totalStamina = 0;
vector<int> stamina, speed;
vector<int> order;
vector<bool> used;
int validCount = 0;

bool canHandover(int a, int b){
    return (stamina[a] + speed[a]) >= stamina[b];
}

bool meetsAverageStamina(int r){
    return (long long)stamina[r] * N >= totalStamina;
}

bool isSwappedOrderValid(int pos, int curr){
    int prev = order[pos - 1];

    if(!canHandover(curr, prev)){
        return false;
    }

    if(pos >= 2 && !canHandover(order[pos - 2], curr)){
        return false;
    }

    if(pos >= N - 2 && !meetsAverageStamina(prev)){
        return false;
    }

    return true;
}

void backtrack(int pos){
    if(pos == N){
        validCount++;
        for (int i = 0; i < N; i++){
            cout<<order[i] + 1<<(i == N - 1 ? "" : " ");
        }
        cout<<endl;
        return;
    }

    for (int i = 0; i < N; i++){
        if(used[i]) 
             continue;

        if(pos >= N - 2 && !meetsAverageStamina(i)){
            continue;
        }

        if(pos >= 1){
            if(!canHandover(order[pos - 1], i)){
                continue;
            }
            if(isSwappedOrderValid(pos, i)){
                continue;
            }
        }

        order[pos] = i;
        used[i] = true;

        backtrack(pos + 1);

        used[i] = false;
    }
}

int main(){
    cout<<"Enter number of runners (N): ";
    cin>>N;

    stamina.resize(N);
    speed.resize(N);
    order.resize(N);
    used.assign(N, false);

    cout<< "Enter stamina and speed for each runner: "<<endl;
    for(int i = 0; i < N; i++){
        cin>>stamina[i]>>speed[i];
        totalStamina += stamina[i];
    }

    cout<<"Valid running orders: "<<endl;
    backtrack(0);

    if(validCount == 0){
        cout<<"None"<<endl;
    }

    return 0;
}