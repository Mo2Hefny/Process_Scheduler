#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define all(x) (x).begin(), (x).end()
#define fi first
#define se second
using namespace std;

void solve() {
    cout << 2 << ' ' << 1 << ' ' << 3 << endl;
    cout << 3 << ' ' << 5 << endl;
    cout << 4 << ' '  << 50 << ' ' << 100 << ' ' << 10 << ' ' << 1 << endl;
    srand(time(0));
    multimap<int, pair<int, int>>processes;
    cout << 35 << endl;
    for(int i = 1; i <= 35; i++) {
        int rand1 = 1 + rand() % 35;
        int rand2 = 1 + rand() % 50;
        int rand3 = rand() % 4;
        processes.insert({rand1, {rand2, rand3}});
    }
    int i = 1;
    for(auto &e : processes) {
        cout << e.fi << ' ' << i << ' ' << e.se.fi << ' ' << e.se.se << ' ';
        int pairs = e.se.se;
        vector<pair<int, int>> randdd;
        while(pairs--) {
            randdd.emplace_back(1 + rand() % e.se.fi, 1 + rand() % 15);
        }
        sort(all(randdd));
        pairs = e.se.se;
        int j = 0;
        while(pairs--) {
            cout << '(' << randdd[j].fi << ',' << randdd[j].se << ')';
            if(pairs != 0) {
                cout << ',';
            }
            j++;
        }
        cout << endl;
        i++;
    }
    int kill = 1 + rand() % 15;
    vector<pair<int, int>>killsig;
    set<int>vis;
    while(kill--) {
        int p = 1 + rand() % 35;
        if(vis.count(p)) {
            kill++;
            continue;
        }
        vis.insert(p);
        killsig.emplace_back(1 + rand() % 400, p);
    }
    sort(all(killsig));
    for(auto &e : killsig) cout << e.fi << ' ' << e.se << endl;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    //freopen("lis.in", "r", stdin);
    //freopen("Aout.txt", "w", stdout);
    int t = 1;
    //cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}