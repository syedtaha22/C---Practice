#include <iostream>
#include <vector>
#include <cmath>

std::vector<double> cal(int n){
    std::vector<double> dp(n);
    dp[0] = 1;
    dp[1] = 3;

    for(int i = 2; i < n;i++){
        dp[i] = -(3 * dp[i-1]) - ( 2* dp[n-2]);
    }

    return dp;
}

std::vector<double> diff(int n){
    std::vector<double> vals(n);
    for(int i = 1; i <=n; i++){
        vals[i] = 3*(pow(-1, i-1)) - (4/3)*(pow(-2, i-1 ));
    }

    return vals;
}


void print(std::vector<double> res){
    for (int num : res) {
        std::cout << num << ", ";
    }
    std::cout << std::endl;
}

int main(){
    int n = 10;
    std::vector<double> res = cal(n);
    std::vector<double> vals = diff(n);
    print(res);
    print(vals);


}