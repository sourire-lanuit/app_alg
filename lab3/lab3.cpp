#include <iostream>
#include <vector>
#include <cmath>

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

class Finding_Components : public Directed_Graph {
    public:
    Finding_Components(int n) : Directed_Graph(n) {}

    void generate_graph(int edges, unsigned s = 0) {
    if (s == 0) {
        s = time(nullptr);
    }
    srand(s);
    
    int edges_added = 0;
    int max_edges = n * (n - 1);  
    
    while (edges_added < edges) {
        int i = rand() % n; 
        int j = rand() % n;  
        if (i != j && adj_matrix[i][j] == 0) {
            add_edge(i, j, 1);
            edges_added++;
        }
    }
}

    void dfs_enum(int v, std::vector<bool>& visited, std::vector<int>& order) {
        visited[v] = true;
        for (int u = 0; u < n; u++) {
            if (adj_matrix[v][u] != 0 && !visited[u]) {
                dfs_enum(u, visited, order);
            }
        }
        order.push_back(v);
    }

    std::vector<std::vector<int>> transposed_graph() const {
        std::vector<std::vector<int>> trans(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) {
                trans[j][i] = adj_matrix[i][j];
            }
        }
        return trans;
    }

    void dfs_dn(int v, std::vector<bool>& visited, std::vector<int>& enum_order) {
        enum_order.push_back(v);
        visited[v] = true;
        for (int u = 0; u < n; u++) {
            if (adj_matrix[u][v] != 0 && !visited[u]) {
                dfs_dn(u, visited, enum_order);
            }
        }
    }

    std::vector<std::vector<int>> find_comp() {
        std::vector<std::vector<int>> find;
        std::vector<bool> visited(n, false);
        std::vector<int> order;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs_enum(i, visited, order);
            }
        }

        auto trans = transposed_graph();
        std::fill(visited.begin(), visited.end(), false);
          
        for (int i = order.size() - 1; i >= 0; i--) { 
            int v = order[i];
            if (!visited[v]) {
                std::vector<int> scc;
                dfs_dn(v, visited, scc); 
                find.push_back(scc);
            }
        }

        return find;
    }
};

struct Analysis {
    int vertices;
    int experiment;
    double density;
    double num_scc;
    double biggest_scc_size;
    double average_scc;
    double single_scc;
    double num_scc_var;
    double biggest_scc_size_var;
    double average_scc_var;
    double single_scc_var;

    void print() const {
        std::cout << "\n===== Орграф із n вершин, n = " << vertices << ", щільність f = " << density << " ======\n";
        std::cout << "Кількість КСЗ:\n";
        std::cout << "Мат. сподівання MX_1 = " << num_scc << "\n";
        std::cout << "Дисперсія DX_1 = " << num_scc_var << "\n";
        std::cout << "========== \n";
        std::cout << "Розмір найбільшої КСЗ:\n";
        std::cout << "Мат. сподівання MX_2 = " << biggest_scc_size << "\n";
        std::cout << "Дисперсія DX_2 = " << biggest_scc_size_var << "\n";
        std::cout << "========== \n";
        std::cout << "Середній розмір КСЗ:\n";
        std::cout << "Мат. сподівання MX_3 = " << average_scc << "\n";
        std::cout << "Дисперсія DX_3 = " << average_scc_var << "\n";
        std::cout << "========== \n";
        std::cout << "Відсоток одноелементних КСЗ:\n";
        std::cout << "Мат. сподівання MX_4 = " << single_scc << "\n";
        std::cout << "Дисперсія DX_4 = " << single_scc_var << "\n";
    }
};

Analysis analyze(int vertices, double density, int experiment) {
    Analysis stats;
    stats.vertices = vertices;
    stats.density = density;
    stats.experiment = experiment;
    
    std::vector<double> num_scc_data;
    std::vector<double> biggest_scc_size_data;
    std::vector<double> average_scc_data;
    std::vector<double> single_scc_data;
    
    for (int exp = 0; exp < experiment; exp++) {
         Finding_Components g(vertices);
         int edges = static_cast<int>(density * vertices * (vertices - 1));
         g.generate_graph(edges, exp + 1);
         auto sccs = g.find_comp();
        
        int num_sccs = sccs.size();
        num_scc_data.push_back(num_sccs);
        
        int biggest = 0;
        int singles = 0;
        double total_size = 0;
        
        for (const auto& scc : sccs) {
            int size = scc.size();
            biggest = std::max(biggest, size);
            total_size += size;
            if (size == 1) singles++;
        }
        
        biggest_scc_size_data.push_back(biggest);
        
        double avg_size = num_sccs > 0 ? total_size / num_sccs : 0;
        average_scc_data.push_back(avg_size);
       
        double single_ratio = num_sccs > 0 ? (double)singles / num_sccs : 0;
        single_scc_data.push_back(single_ratio);
    }
    
    auto calc = [](const std::vector<double>& data) -> std::pair<double, double> {
        double sum = 0;
        int n = data.size();
        
        for (double val : data) sum += val;
        
        double median = sum / n;
        double variance = (pow(sum, 2)  / n) - (median * median);
        
        return {median, variance};
    };
    
    auto [m1, var1] = calc(num_scc_data);
    stats.num_scc = m1;
    stats.num_scc_var = var1;
    
    auto [m2, var2] = calc(biggest_scc_size_data);
    stats.biggest_scc_size = m2;
    stats.biggest_scc_size_var = var2;
    
    auto [m3, var3] = calc(average_scc_data);
    stats.average_scc = m3;
    stats.average_scc_var = var3;
    
    auto [m4, var4] = calc(single_scc_data);
    stats.single_scc = m4;
    stats.single_scc_var = var4;
    
    return stats;
}

int main() {
    int n, edges, experiment;
    std::cout << "Введіть значення для кількіть вершин, ребер, кількість теоретичних запусків алгоритму: ";
    std::cin >> n >> edges >> experiment;
    Finding_Components graph(n);
    graph.generate_graph(edges);
    graph.display_matrix();

    auto scc = graph.find_comp();
    std::cout << "Знайдено " << scc.size() << " компонент сильної зв'язності:\n";
    for (size_t i = 0; i < scc.size(); i++) {
        std::cout << "КСЗ " << i + 1 << ": { ";
        for (int v : scc[i]) {
            std::cout << v << " ";
        }
        std::cout << "}\n";
    }

    double density = static_cast<double>(edges) / (n * (n - 1));
    Analysis stats = analyze(n, density, experiment);
    stats.print();

    return 0;
}