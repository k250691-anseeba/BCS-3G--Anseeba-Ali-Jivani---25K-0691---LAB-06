#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
int capacity, highRiskThreshold;
vector<vector<int>> people, units;
vector<bool> rowHasAffected;
vector<int> minRowUnits;
vector<vector<bool>> hasValidAfter;
vector<vector<int>> suffixPeople;

int bestPeople = -1;
int bestUnits = 0;
vector<pair<int, int>> currSelected, bestSelected;

bool isAccessible(int r, int c){
    return people[r][c] != -1 && units[r][c] != -1;
}

bool isHighRisk(int r, int c){
    return people[r][c] >= highRiskThreshold;
}

int futureMinUnits(int nextRow){
    int sum = 0;
    for(int r = nextRow; r < N; r++){
        if(rowHasAffected[r]){
            sum += minRowUnits[r];
        }
    }
    return sum;
}

void backtrack(int r, int c, int currPeople, int currUnits, int highRiskCount, int rowSelected){
    if(currUnits > capacity){
        return;
    }

    if(r == N){
        if(currPeople > bestPeople || (currPeople == bestPeople && currUnits < bestUnits)){
            bestPeople = currPeople;
            bestUnits = currUnits;
            bestSelected = currSelected;
        }
        return;
    }

    if(currPeople + suffixPeople[r][c] < bestPeople){
        return;
    }

    if(c == M){
        if(rowHasAffected[r] && rowSelected == 0){
            return;
        }
        if(currUnits + futureMinUnits(r + 1) > capacity){
            return;
        }
        backtrack(r + 1, 0, currPeople, currUnits, highRiskCount, 0);
        return;
    }

    if(isAccessible(r, c) && people[r][c] > 0){
        int extraUnits = isHighRisk(r, c) ? highRiskCount : 0;
        int totalCost = units[r][c] + extraUnits;
        int minNeededAfter = futureMinUnits(r + 1);

        if(currUnits + totalCost + minNeededAfter <= capacity){
            currSelected.push_back({r, c});
            backtrack(r, c + 1,
                      currPeople + people[r][c],
                      currUnits + totalCost,
                      highRiskCount + (isHighRisk(r, c) ? 1 : 0),
                      rowSelected + 1);
            currSelected.pop_back();
        }
    }

    bool mustSelectNow = rowHasAffected[r] && (rowSelected == 0) && !hasValidAfter[r][c];
    if(!mustSelectNow){
        backtrack(r, c + 1, currPeople, currUnits, highRiskCount, rowSelected);
    }
}

int main(){
    cout<<"Enter rows (N) and columns (M): ";
    cin>>N>>M;

    people.assign(N, vector<int>(M));
    units.assign(N, vector<int>(M));

    cout<<"Enter People Matrix (-1 for inaccessible): "<<endl;
    for(int r = 0; r < N; r++){
        for(int c = 0; c < M; c++){
            cin>>people[r][c];
        }
    }

    cout<<"Enter Rescue Units Required Matrix (-1 for inaccessible): "<<endl;
    for(int r = 0; r < N; r++){
        for(int c = 0; c < M; c++){
            cin>>units[r][c];
        }
    }

    cout<<"Enter total rescue units capacity: ";
    cin>>capacity;

    cout<<"Enter high-risk people threshold: ";
    cin>>highRiskThreshold;

    rowHasAffected.assign(N, false);
    minRowUnits.assign(N, 1e9);
    hasValidAfter.assign(N, vector<bool>(M, false));
    suffixPeople.assign(N + 1, vector<int>(M + 1, 0));

    for(int r = 0; r < N; r++){
        for(int c = 0; c < M; c++){
            if(isAccessible(r, c) && people[r][c] > 0){
                rowHasAffected[r] = true;
                minRowUnits[r] = min(minRowUnits[r], units[r][c]);
            }
        }
        bool validSeen = false;
        for(int c = M - 1; c >= 0; c--){
            hasValidAfter[r][c] = validSeen;
            if(isAccessible(r, c) && people[r][c] > 0){
                validSeen = true;
            }
        }
    }

    int runningPeople = 0;
    for(int r = N - 1; r >= 0; r--){
        for(int c = M - 1; c >= 0; c--){
            if(isAccessible(r, c) && people[r][c] > 0){
                runningPeople += people[r][c];
            }
            suffixPeople[r][c] = runningPeople;
        }
    }

    if(futureMinUnits(0) <= capacity){
        backtrack(0, 0, 0, 0, 0, 0);
    }

    if(bestPeople == -1){
        cout<<endl<<"No valid rescue allocation exists within the given capacity and row constraints."<<endl;
        return 0;
    }

    cout<<endl<<"--- Optimal Rescue Allocation ---"<<endl;
    cout<<"Selected Matrix Positions (1-based): ";
    for(int i = 0; i < (int)bestSelected.size(); i++){
        int r = bestSelected[i].first;
        int c = bestSelected[i].second;
        cout<<"("<<r + 1<<", "<<c + 1<<")";
        if(i + 1 < (int)bestSelected.size()){
            cout<<", ";
        }
    }
    cout<<endl;

    cout<<"Total People Rescued: "<<bestPeople<<endl;
    cout<<"Total Rescue Units Used: "<<bestUnits<<endl;
    cout<<"Remaining Rescue Units: "<<(capacity - bestUnits)<<endl;

    return 0;
}