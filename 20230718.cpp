#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

// 実行コマンド例　if ($?) { g++ -O3 20230718.cpp -o 20230718 } ; if ($?) { .\20230718 }

int main() {
    const long long division = 1000; // 1年の分割数
    const long long years = 1000; // 回す年数
    const long long trials = 1000; // 試行回数
    const long long earn_money = 12500000; //1年に1250万円獲得
    const long long pocket = 75000000; //所持金7500万円
    const long long earn_money_per_division = earn_money / division;
    const int average_break_count = 1; //1年に1回壊れる
    const long long average_repair_cost = 10000000; //修理コスト1000万円

    random_device seed_gen;
    mt19937 count(seed_gen());

    poisson_distribution<int> poisson(average_break_count);//ポアソン分布
    uniform_int_distribution<int> dist(0, division - 1);//一様分布
    exponential_distribution<double> exponential(1.0 / (double)average_repair_cost);//指数分布

    auto start = chrono::high_resolution_clock::now();//実行時間計測
    int bankruptcy = 0;
    vector<int> numbers;
    for (int i = 0; i < trials; ++i) {
        long long money = pocket;
        bool go_next = false;
        for (int j = 0; j < years; ++j) {
            int break_count = poisson(count);
            if (break_count > 0){
                numbers.reserve(division);
                vector<int> break_time(break_count);
                for (int k = 0; k < division; ++k) {
                    numbers[k] = k;
                }
                shuffle(numbers.begin(), numbers.end(), count);
            }

            for (int l=0; l < division; ++l) {
                if (break_count > 0){
                    for(int m = 0; m < break_count; m++){
                        if(numbers[m] == l){
                            money -= static_cast<long long>(exponential(count));
                        }
                    }
                }
                if (money < 0) {
                    ++bankruptcy;
                    go_next = true;
                    break;
                }
                money += earn_money_per_division;
            }
            if(go_next)break;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Time: " << duration.count() << "(sec)\n";
    cout << "Bankrupt Count: " << bankruptcy << "\n";
    cout << "Bankrupt Probability: " << (double)bankruptcy / (double)trials << "\n";
    return 0;
}
