#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector> 
#include <cstdlib> 
#include <time.h>
#include <math.h>
using namespace std;
struct Data {
    int number_of_task;
    int execution_time; 
    int weight;
    int deadline;
};

struct Table {
    int punishment;
    int time;
};

vector<Data> load_data(int number, int& n) {
    string s;
    ifstream file("witi.data.txt");
    string target = "data." + std::to_string(number) + ":";
    while (s != target) file >> s;
    file >> n;
    vector<Data> data(n);
    for (int i = 0; i < n; i++) {
        file >> data[i].execution_time >> data[i].weight >> data[i].deadline;
        data[i].number_of_task = i;
    }
    file.close();
    return data;
}

int WiTi(vector<Data>& data, vector<vector<int>>& order, int& n) {
    int length = pow(2, n);
    vector<Table> table(length, { 99999, 0 });
    table[0].punishment = 0; table[0].time = 0; int punishment = 0;
    for (int i = 1; i < length; i++) {
        for (int k = 0, b = 1; k < n; k++, b *= 2) {
            if (i & b) {
                punishment = max(table[i - (1 << data[k].number_of_task)].punishment + (table[i - (1 << data[k].number_of_task)].time + data[k].execution_time - data[k].deadline) * data[k].weight, table[i - (1 << data[k].number_of_task)].punishment);
                if (punishment < table[i].punishment) {
                    table[i].punishment = punishment;
                    table[i].time = data[k].execution_time + table[i - (1 << data[k].number_of_task)].time;
                    order[i] = order[i - (1 << data[k].number_of_task)];
                    order[i].push_back(data[k].number_of_task);
                }
            }
        }
    }
    return table[length-1].punishment;
}

int main()
{
    int n;
    vector<Data> data;
    for (int number = 10; number < 21; number++) {
        data = load_data(number, n);
        int length = pow(2, n);
        vector<vector<int>> order(length);
        int the_lowest_cost = WiTi(data, order, n);
        cout << "The least cost : " << the_lowest_cost << endl;
        cout << "Tasks order: ";
        for (const auto& task : order[length - 1]) {
            cout << task + 1 << " ";
        }
        cout << endl;
    }
    return 0;
}

