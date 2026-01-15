#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>

class Graph {
    protected:
    int n;
    std::vector<std::vector<int>> adj_matrix; 

    public:
    Graph(int v) : n(v) {
        if (v < 0) throw std::invalid_argument("К-ть вершин має бути додатньою.");
        adj_matrix.resize(n, std::vector<int>(n, 0));
    }

    virtual void add_vertex() {
        n++;
        for (auto &row : adj_matrix) {
            row.push_back(0);
        }
        adj_matrix.push_back(std::vector<int>(n, 0));
    }

    virtual void remove_vertex(int v) {
        if (v < 0 || v >= n) {
            throw std::out_of_range("Неприпустимо.");
        }
        adj_matrix.erase(adj_matrix.begin() + v);
        n--;
        for (auto &row : adj_matrix) {
            row.erase(row.begin() + v);
        }
    }

    virtual void add_edge(int v, int u, int w = 1) = 0;  
    virtual void remove_edge(int v, int u) = 0;

    virtual std::vector<std::list<std::pair<int, int>>> conv_to_adj_list() const {
        std::vector<std::list<std::pair<int, int>>> adj_list(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adj_matrix[i][j] != 0) {
                    adj_list[i].push_back({j, adj_matrix[i][j]});
                }
            }
        }
        return adj_list;
    }

    virtual void display_matrix() const {
        std::cout << "Матриця суміжности:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << adj_matrix[i][j] << ' ';
            }
            std::cout << "\n";
        }
    }

    virtual ~Graph() = default;
};

class Undirected_Graph : public Graph {
    public:
    Undirected_Graph(int n) : Graph(n) {}

    void add_edge(int u, int v, int w = 1) override {
        if (u >= 0 && v >= 0 && u < n && v < n) {
            adj_matrix[u][v] = w;
            adj_matrix[v][u] = w;
        }
    }

    void remove_edge(int u, int v) override {
        if (u >= 0 && v >= 0 && u < n && v < n) {
            adj_matrix[u][v] = 0;
            adj_matrix[v][u] = 0;
        }
    }
};

class Weighted_Graph : public Graph {
    public:
    Weighted_Graph(int n) : Graph(n) {}
    
    void add_edge(int u, int v, int w = 1) override {
        if (u >= 0 && v >= 0 && u < n && v < n) {
            adj_matrix[u][v] = w;
        }
    }

    void remove_edge(int u, int v) override {
        if (u >= 0 && v >= 0 && u < n && v < n) {
            adj_matrix[u][v] = 0;
        }
    }

    void display_matrix() const override {
        std::cout << "Матриця суміжности:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << adj_matrix[i][j] << ' ';
            }
            std::cout << "\n";
        }

        std::cout << "Ребра зваженого орієнтованого графа:\n";
        bool has_edges = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adj_matrix[i][j] != 0) {
                    std::cout << i << " -> " << j << " (вага: " << adj_matrix[i][j] << ")\n";
                    has_edges = true;
                }
            }
        }
        if (!has_edges) {
            std::cout << "Граф порожній.\n";
        }
    }
};

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);

        if (rx == ry) return false;

        if (rank[rx] < rank[ry])
            parent[rx] = ry;
        else if (rank[rx] > rank[ry])
            parent[ry] = rx;
        else {
            parent[ry] = rx;
            rank[rx]++;
        }
        return true;
    }
};

struct Edge {
    int u, v, w;
};

std::vector<Edge> kruskal(const Undirected_Graph& g) {
    int n = g.conv_to_adj_list().size();
    std::vector<Edge> edges;

    auto adj = g.conv_to_adj_list();
    for (int u = 0; u < n; ++u) {
        for (auto [v, w] : adj[u]) {
            if (u < v) { 
                edges.push_back({u, v, w});
            }
        }
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) {
                  return a.w < b.w;
              });

    UnionFind uf(n);
    std::vector<Edge> tree;
    
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            tree.push_back(e);
            if (tree.size() == n - 1) break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Час виконання алгоритму Крускала: " << duration.count() << " мкс\n";

    return tree;
}

int main() {
    Undirected_Graph g(8);

    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 6);
    g.add_edge(0, 3, 5);
    g.add_edge(1, 3, 15);
    g.add_edge(2, 3, 4);
    g.add_edge(2, 5, 13);
    g.add_edge(0, 4, 9);
    g.add_edge(1, 5, 18);


    auto tree = kruskal(g);

    std::cout << "Мінімальне кістякове дерево:\n";
    int total = 0;
    for (const auto& e : tree) {
        std::cout << e.u << " - " << e.v << " (вага " << e.w << ")\n";
        total += e.w;
    }
    std::cout << "Загальна вага МКД: " << total << "\n";

    return 0;
}
