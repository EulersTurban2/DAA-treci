#include <iostream>

#include <vector>
#include <algorithm>

std::vector<int> izracunajPS(const std::string &p) {
  int m = p.size() + 1;
  std::vector<int> pi(m);
  int i = 0;
  int j = -1;
  
  pi[i] = j;
  
  while (i + 1 < m) {
    while (j >= 0 && p[i] != p[j]){
      j = pi[j];
    }
    pi[++i] = ++j;
  }
  return pi;
}

int main(){
    std::string s;
    std::string p;
    std::cin >> s >> p;
    
    std::string ss = s+s;
    std::string tmp = p + "#" + ss;

    int p_size = p.size();
    std::vector<int> ps_tmp = izracunajPS(tmp);
    for (int i = 0; i < ps_tmp.size(); i++)
    {
        if (ps_tmp[i] == p_size)
        {
            std::cout << "da" << std::endl;
            return 0;
        }
    }
    std::cout << "ne" << std::endl;
    return 0;
}