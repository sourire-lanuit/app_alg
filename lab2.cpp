#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <cstdlib>

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

class Directed_Graph : public Graph {
    public:
    Directed_Graph(int n) : Graph(n) {}

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

class Erdos_Renyi_Model : public Graph {
    public: 
    static Directed_Graph generate_dir(int n, double p) {
        Directed_Graph grph(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    double q = (double)rand() / RAND_MAX;
                    if (q < p) {
                        grph.add_edge(i, j);
                    }
                }
            }
        }
        return grph;
    }

    static Undirected_Graph generate_undir(int n, double p) {
        Undirected_Graph grph(n);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double q = (double)rand() / RAND_MAX;
                if (q < p) {
                    grph.add_edge(i, j);
                }
            }
        }
        return grph;
    }

    static Weighted_Graph generate_wg(int n, double p, int min_w, int max_w) {
        Weighted_Graph grph(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    double q = (double)rand() / RAND_MAX;
                    if (q < p) {
                        int w = min_w + rand() % (max_w - min_w + 1);
                        grph.add_edge(i, j, w);
                    }
                }
            }
        }
        return grph;
    }
};

int main() {
    srand(time(nullptr));

    std::cout << "##### Орієнтований граф: #####\n";
    Directed_Graph dg(6);
    dg.add_edge(0, 1);
    dg.add_edge(2, 3);
    dg.add_edge(2, 4);
    dg.add_edge(3, 5);
    dg.add_vertex();
    dg.add_vertex();
    dg.add_vertex();
    dg.remove_vertex(1);
    dg.remove_edge(2, 3);
    dg.display_matrix();

    std::cout << "\n##### Неорієнтований граф: #####\n";
    Undirected_Graph ug(4);
    ug.add_edge(0, 1);
    ug.add_edge(1, 2);
    ug.add_edge(2, 3);
    ug.add_vertex();
    ug.display_matrix();

    std::cout << "\n##### Зважений орієнтований граф: #####\n";
    Weighted_Graph wg(4);
    wg.add_edge(0, 1, 5);
    wg.add_edge(2, 3, 7);
    wg.display_matrix();

    auto adj_list = wg.conv_to_adj_list();
    std::cout << "Список суміжности:\n";
        for (int i = 0; i < adj_list.size(); ++i) {
            std::cout << "Вершина " << i << ": ";
            for (const auto &edge : adj_list[i]) {
                std::cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            std::cout << "\n";
        }


    std::cout << "\n##### Граф у моделі Ердеша-Реньї: #####\n";
    auto rand_dir = Erdos_Renyi_Model::generate_dir(4, 0.6);
    rand_dir.display_matrix();
    std::cout << "##########\n";
    auto rand_undir = Erdos_Renyi_Model::generate_undir(5, 0.3);
    rand_undir.display_matrix();
    std::cout << "##########\n";
    auto rand_wg = Erdos_Renyi_Model::generate_wg(6, 0.45, 2, 5);
    rand_wg.display_matrix();

    return 0;
}