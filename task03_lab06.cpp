#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int N, R;
int catLimit[3];
int emergencyReserve;
int threshold;
int highPrio = -1, lowPrio = 1e9;

vector<int> cat, prio, minTok, maxTok;
vector<int> resU, resV, resMaxSum;
vector<int> currAlloc, bestAlloc;
int catUsed[3] = {0, 0, 0};
int bestEmergency;

int getEmergencyUsed(){
    int em = 0;
    for(int c = 0; c < 3; c++){
        if(catUsed[c] > catLimit[c]){
            em += (catUsed[c] - catLimit[c]);
        }
    }
    return em;
}

bool isPlacementValid(int idx, int tokens){
    for(int j = 0; j < idx; j++){
        if(cat[j] == cat[idx] && currAlloc[j] == tokens){
            return false;
        }
        if(prio[j] > prio[idx] && currAlloc[j] < tokens){
            return false;
        }
        if(prio[j] < prio[idx] && currAlloc[j] > tokens){
            return false;
        }
    }

    for(int r = 0; r < R; r++){
        if(resU[r] == idx && resV[r] < idx){
            if(tokens + currAlloc[resV[r]] > resMaxSum[r]){
                return false;
            }
        }
        if(resV[r] == idx && resU[r] < idx){
            if(currAlloc[resU[r]] + tokens > resMaxSum[r]){
                return false;
            }
        }
    }

    return true;
}

void backtrack(int idx){
    int emUsed = getEmergencyUsed();
    if(emUsed > emergencyReserve || emUsed >= bestEmergency){
        return;
    }

    if(idx == N){
        bool hasMin = false, hasMax = false;
        int highSum = 0, lowSum = 0;

        for(int i = 0; i < N; i++){
            if(currAlloc[i] == minTok[i]){
                hasMin = true;
            }
            if(currAlloc[i] == maxTok[i]){
                hasMax = true;
            }
            if(prio[i] == highPrio){
                highSum += currAlloc[i];
            }
            if(prio[i] == lowPrio){
                lowSum += currAlloc[i];
            }
        }

        if(!hasMin || !hasMax){
            return;
        }

        if(highPrio > lowPrio && (highSum - lowSum <= threshold)){
            return;
        }

        bestEmergency = emUsed;
        bestAlloc = currAlloc;
        return;
    }

    for(int tokens = minTok[idx]; tokens <= maxTok[idx]; tokens++){
        if(!isPlacementValid(idx, tokens)){
            continue;
        }

        currAlloc[idx] = tokens;
        catUsed[cat[idx]] += tokens;

        backtrack(idx + 1);

        catUsed[cat[idx]] -= tokens;
    }
}

string getCategoryName(int c){
    if(c == 0) return "Breakfast";
    if(c == 1) return "Lunch";
    return "Dinner";
}

int main(){
    cout<<"Enter regular token limits (Breakfast Lunch Dinner): ";
    cin>>catLimit[0]>>catLimit[1]>>catLimit[2];

    cout<<"Enter emergency reserve tokens: ";
    cin>>emergencyReserve;

    cout<<"Enter high-vs-low priority difference threshold: ";
    cin>>threshold;

    cout<<"Enter number of students: ";
    cin>>N;

    cat.resize(N);
    prio.resize(N);
    minTok.resize(N);
    maxTok.resize(N);
    currAlloc.resize(N);

    cout<<"Enter Category(0=B, 1=L, 2=D), Priority(2=High, 1=Low), Min, Max for each student: "<<endl;
    for(int i = 0; i < N; i++){
        cin>>cat[i]>>prio[i]>>minTok[i]>>maxTok[i];
        highPrio = max(highPrio, prio[i]);
        lowPrio = min(lowPrio, prio[i]);
    }

    cout<<"Enter number of student-to-student pair restrictions: ";
    cin>>R;
    resU.resize(R);
    resV.resize(R);
    resMaxSum.resize(R);

    for(int r = 0; r < R; r++){
        int u, v, cap;
        cin>>u>>v>>cap;
        resU[r] = u - 1;
        resV[r] = v - 1;
        resMaxSum[r] = cap;
    }

    bestEmergency = emergencyReserve + 1;
    backtrack(0);

    if(bestEmergency > emergencyReserve){
        cout<<"No valid token allocation found."<<endl;
        return 0;
    }

    int finalCatTotal[3] = {0, 0, 0};
    cout<<endl<<"--- Final Student Allocation ---"<<endl;
    for(int i = 0; i < N; i++){
        finalCatTotal[cat[i]] += bestAlloc[i];
        cout<<"Student "<<i + 1<<" ("<<getCategoryName(cat[i])<<", Prio "<<prio[i]<<"): "<<bestAlloc[i]<<" tokens"<<endl;
    }

    cout<<endl<<"--- Category Totals & Unused Regular Tokens ---"<<endl;
    for(int c = 0; c < 3; c++){
        int unusedReg = max(0, catLimit[c] - finalCatTotal[c]);
        cout<<getCategoryName(c)<<": Total Used = "<<finalCatTotal[c]<<", Regular Unused = "<<unusedReg<<endl;
    }

    cout<<endl<<"--- Emergency Reserve Summary ---"<<endl;
    cout<<"Emergency Tokens Used: "<<bestEmergency<<endl;
    cout<<"Emergency Tokens Unused: "<<(emergencyReserve - bestEmergency)<<endl;

    return 0;
}