#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

using namespace std;

// Segment tree for range maximum query with lazy propagation
struct SegmentTree {
    vector<int> tree, lazy;
    int n;
    
    SegmentTree(int size) {
        n = size;
        tree.resize(4 * (n + 1), 0);
        lazy.resize(4 * (n + 1), 0);
    }
    
    void push(int node, int left, int right) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (left != right) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }
    
    void update(int node, int left, int right, int start, int end, int value) {
        push(node, left, right);
        if (start > right || end < left) return;
        if (start <= left && right <= end) {
            lazy[node] += value;
            push(node, left, right);
            return;
        }
        int mid = (left + right) / 2;
        update(node * 2, left, mid, start, end, value);
        update(node * 2 + 1, mid + 1, right, start, end, value);
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }
    
    int query(int node, int left, int right, int start, int end) {
        push(node, left, right);
        if (start > right || end < left) return 0;
        if (start <= left && right <= end) return tree[node];
        int mid = (left + right) / 2;
        return max(query(node * 2, left, mid, start, end), 
                   query(node * 2 + 1, mid + 1, right, start, end));
    }
    
    void update_range(int start, int end, int value) {
        if (start <= end && start >= 1 && end <= n) {
            update(1, 1, n, start, end, value);
        }
    }
    
    int query_range(int start, int end) {
        if (start <= end && start >= 1 && end <= n) {
            return query(1, 1, n, start, end);
        }
        return 0;
    }
};

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    SegmentTree st(x);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // For a journey from u to v, we need seats from station u to v-1
        // Check if we can fulfill this order
        int max_usage = st.query_range(u, v - 1);
        
        if (max_usage + n <= k) {
            // Can fulfill this order
            st.update_range(u, v - 1, n);
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}
