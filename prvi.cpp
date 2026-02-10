#include <iostream>

#include <vector>
#include <algorithm>

int modp(int x, int p) {
    while(x < 0){
        x += p;
    }
    return x % p;
}

std::vector<int> solution(int p) {
    
    std::vector<int> returner;
    int counter = 0;
    returner.push_back(p-1);
    while (counter < p - 1)
    {
        int last_val = returner[counter++];
        returner.push_back(modp(-counter*last_val,p));
    }
    std::reverse(returner.begin(),returner.end());
    return returner;
};


int main(){
    int n,p;
    std::cin >> n >> p;
    std::vector<int> sol = solution(p);
    for (int i = 0; i < n; i++)
    {
        std::cout << sol[i+1] << " ";
    }
    std::cout << "\n";
    return 0;
}