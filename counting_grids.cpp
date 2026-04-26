#include<bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;

long long power(long long a, long long b, long long mod) {
    long long res = 1;
    a %= mod;
    if(a < 0) a += mod;
    while(b > 0) {
        if(b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

long long modInv(long long a, long long mod) {
    return power(a, mod - 2, mod);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<vector<long long>> A(N, vector<long long>(M));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> A[i][j];
        }
    }
    
    int nm = N * M;
    
    // Calculate S0 (sum where (i+j) is even) and S1 (sum where (i+j) is odd)
    long long S0 = 0, S1 = 0;
    long long n0 = 0, n1 = 0;
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if((i + j) % 2 == 0) {
                S0 = (S0 + A[i][j]) % MOD;
                n0++;
            } else {
                S1 = (S1 + A[i][j]) % MOD;
                n1++;
            }
        }
    }
    
    // Calculate fK(c) = number of K-tuples with color c
    // fK(0) = ((n0+n1)^K + (n0-n1)^K) / 2
    // fK(1) = ((n0+n1)^K - (n0-n1)^K) / 2
    
    long long nm_pow_K = power(nm, K, MOD);
    long long diff = (n0 - n1 + MOD) % MOD;
    long long sum_pow_K = power(nm, K, MOD);
    long long diff_pow_K = power(diff, K, MOD);
    
    long long fK0 = ((sum_pow_K + diff_pow_K) % MOD * modInv(2, MOD)) % MOD;
    long long fK1 = ((sum_pow_K - diff_pow_K + MOD) % MOD * modInv(2, MOD)) % MOD;
    
    // For K >= 2:
    // Total weight for color class c = (NM)^K * K * (S0 * f_{K-1}(c) + S1 * f_{K-1}(1-c))
    
    if(K == 1) {
        // For K=1: max independent set is max(S0, S1)
        long long maxWeight = max(S0, S1);
        maxWeight = (maxWeight * nm_pow_K) % MOD;
        cout << maxWeight << "\n";
    } else {
        // Calculate f_{K-1}(0) and f_{K-1}(1)
        long long nm_pow_K_minus_1 = power(nm, K - 1, MOD);
        long long diff_pow_K_minus_1 = power(diff, K - 1, MOD);
        
        long long fK_minus_1_0 = ((sum_pow_K - diff_pow_K_minus_1 + MOD) % MOD * modInv(2, MOD)) % MOD;
        long long fK_minus_1_1 = ((sum_pow_K - diff_pow_K_minus_1 + MOD) % MOD * modInv(2, MOD)) % MOD;
        
        // Recalculate more carefully
        fK_minus_1_0 = ((nm_pow_K_minus_1 + diff_pow_K_minus_1) % MOD * modInv(2, MOD)) % MOD;
        fK_minus_1_1 = ((nm_pow_K_minus_1 - diff_pow_K_minus_1 + MOD) % MOD * modInv(2, MOD)) % MOD;
        
        // W0 = (NM)^K * K * (S0 * f_{K-1}(0) + S1 * f_{K-1}(1))
        // W1 = (NM)^K * K * (S0 * f_{K-1}(1) + S1 * f_{K-1}(0))
        
        long long term0 = (S0 * fK_minus_1_0) % MOD;
        term0 = (term0 + (S1 * fK_minus_1_1) % MOD) % MOD;
        
        long long term1 = (S0 * fK_minus_1_1) % MOD;
        term1 = (term1 + (S1 * fK_minus_1_0) % MOD) % MOD;
        
        long long K_mod = K % MOD;
        long long W0 = (nm_pow_K * K_mod) % MOD;
        W0 = (W0 * term0) % MOD;
        
        long long W1 = (nm_pow_K * K_mod) % MOD;
        W1 = (W1 * term1) % MOD;
        
        long long culv = max(W0, W1);
        cout << culv << "\n";
    }
    
    return 0;
}
