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
    
    long long nm_mod = nm % MOD;
    long long n0_mod = n0 % MOD;
    long long n1_mod = n1 % MOD;
    
    // Calculate (n0 - n1) mod MOD
    long long diff = (n0_mod - n1_mod + MOD) % MOD;
    
    // Calculate nm^K
    long long nm_pow_K = power(nm_mod, K, MOD);
    
    // Calculate (n0 - n1)^K
    long long diff_pow_K = power(diff, K, MOD);
    
    long long inv2 = modInv(2, MOD);
    
    // fK(0) = (nm^K + (n0-n1)^K) / 2
    // fK(1) = (nm^K - (n0-n1)^K) / 2
    
    long long fK0 = ((nm_pow_K + diff_pow_K) % MOD * inv2) % MOD;
    long long fK1 = ((nm_pow_K - diff_pow_K + MOD) % MOD * inv2) % MOD;
    
    if(K == 1) {
        // For K=1: vertices are single positions
        // Independent set is either all even positions or all odd positions
        // Weight = nm^K * S where S is sum of chosen positions
        // So max = nm * max(S0, S1)
        
        long long W0 = (nm_mod * S0) % MOD;
        long long W1 = (nm_mod * S1) % MOD;
        long long culv = max(W0, W1);
        cout << culv << "\n";
    } else {
        // For K >= 2:
        // Calculate (n0 - n1)^(K-1)
        long long diff_pow_K_minus_1 = power(diff, K - 1, MOD);
        
        // f_{K-1}(0) = (nm^(K-1) + (n0-n1)^(K-1)) / 2
        // f_{K-1}(1) = (nm^(K-1) - (n0-n1)^(K-1)) / 2
        
        long long nm_pow_K_minus_1 = power(nm_mod, K - 1, MOD);
        
        long long fK_minus_1_0 = ((nm_pow_K_minus_1 + diff_pow_K_minus_1) % MOD * inv2) % MOD;
        long long fK_minus_1_1 = ((nm_pow_K_minus_1 - diff_pow_K_minus_1 + MOD) % MOD * inv2) % MOD;
        
        // W0 = (NM)^K * K * (S0 * f_{K-1}(0) + S1 * f_{K-1}(1))
        // W1 = (NM)^K * K * (S0 * f_{K-1}(1) + S1 * f_{K-1}(0))
        
        long long S0_times_fK1_0 = (S0 * fK_minus_1_0) % MOD;
        long long S1_times_fK1_1 = (S1 * fK_minus_1_1) % MOD;
        long long term0 = (S0_times_fK1_0 + S1_times_fK1_1) % MOD;
        
        long long S0_times_fK1_1 = (S0 * fK_minus_1_1) % MOD;
        long long S1_times_fK1_0 = (S1 * fK_minus_1_0) % MOD;
        long long term1 = (S0_times_fK1_1 + S1_times_fK1_0) % MOD;
        
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
