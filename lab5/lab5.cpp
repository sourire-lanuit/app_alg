#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

class Vector {
public:
    int n;
    std::vector<double> v;

    Vector(int n) : n(n), v(n, 0.0) {}

    double& operator[](int i) {
        return v[i];
    }

    const double& operator[](int i) const {
        return v[i];
    }

    Vector operator+(const Vector& u) const {
        Vector res(n);
        for (int i = 0; i < n; i++) res[i] = v[i] + u[i];
        return res;
    }

    Vector operator-(const Vector& u) const {
        Vector res(n);
        for (int i = 0; i < n; i++) res[i] = v[i] - u[i];
        return res;
    }

    Vector operator*(double u) const {
        Vector res(n);
        for (int i = 0; i < n; i++) res[i] = v[i] * u;
        return res;
    }

    double dot(const Vector& u) const {
        double sum = 0.0;
        for (int i = 0; i < n; i++) sum += v[i] * u[i];
        return sum;
    }

    void print() const {
        std::cout << std::fixed << std::setprecision(6);
        for (int i = 0; i < n; i++) {
            std::cout << "x[" << i + 1 << "] = " << v[i] << std::endl;
        }
    }
};

class Matrix {
public:
    int n;
    std::vector<std::vector<double>> a;

    Matrix(int n) : n(n), a(n, std::vector<double>(n, 0.0)) {}

    std::vector<double>& operator[](int i) {
        return a[i];
    }

    const std::vector<double>& operator[](int i) const {
        return a[i];
    }

    static Matrix ident(int n) {
        Matrix I(n);
        for (int i = 0; i < n; i++) I[i][i] = 1.0;
        return I;
    }

    Matrix operator+(const Matrix& b) const {
        Matrix res(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) res[i][j] = a[i][j] + b[i][j];
        }
        return res;
    }

    Matrix operator-(const Matrix& b) const {
        Matrix res(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) res[i][j] = a[i][j] - b[i][j];
        }
        return res;
    }

    Matrix operator*(const Matrix& b) const {
        Matrix res(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) res[i][j] += a[i][k] * b[k][j];
            }
        }
        return res;
    }

    Vector operator*(const Vector& b) const {
        Vector res(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) res[i] += a[i][j] * b[j];
        }
        return res;
    }

    void print() const {
        std::cout << std::fixed << std::setprecision(2);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << a[i][j];
                if (j + 1 < n) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
};

class LUP {
public:
    int n;
    Matrix L, U;
    std::vector<int> P;

    LUP(const Matrix& A) : n(A.n), L(n), U(A), P(n) {
        for (int i = 0; i < n; i++) {
            P[i] = i;
            L[i][i] = 1.0;
        }

        for (int k = 0; k < n; k++) {
            double max_val = 0.0;
            int pivot = k;
            for (int i = k; i < n; i++) {
                double val = fabs(U[i][k]);
                if (val > max_val) {
                    max_val = val;
                    pivot = i;
                }
            }

            if (max_val < 1e-12) {
                std::cout << "Матриця вироджена" << std::endl;
                return;
            }

            std::swap(U[k], U[pivot]);
            std::swap(P[k], P[pivot]);
            for (int j = 0; j < k; j++) std::swap(L[k][j], L[pivot][j]);

            for (int i = k + 1; i < n; i++) {
                L[i][k] = U[i][k] / U[k][k];
                for (int j = k; j < n; j++) {
                    U[i][j] = U[i][j] - L[i][k] * U[k][j];
                }
            }
        }
    }

    Vector solve(const Vector& a) {
        Vector Pa(n), y(n), x(n);
        for (int i = 0; i < n; i++) Pa[i] = a[P[i]];
        for (int i = 0; i < n; i++) {
            y[i] = Pa[i];
            for (int j = 0; j < i; j++) {
                y[i] = y[i] - L[i][j] * y[j];
            }
        }
        for (int i = n - 1; i >= 0; i--) {
            x[i] = y[i];
            for (int j = i + 1; j < n; j++) {
                x[i] = x[i] - U[i][j] * x[j];
            }
            x[i] = x[i] / U[i][i];
        }

        return x;
    }

    void printLU() const {
        std::cout << "\nМатриця L:\n";
        L.print();
        std::cout << "\nМатриця U:\n";
        U.print();
        std::cout << "\nВектор перестановок P: ";
        for (int i = 0; i < n; i++) {
            std::cout << P[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    int n = 10;
    Matrix A(n);

    double data[10][10] = {
        {2,  3, -1,  4,  1, -2,  1,  3, -1,  2},
        {1, -2,  4,  1, -3,  2,  1, -1,  2,  1},
        {3,  1,  2, -3,  1,  4, -2,  1,  1, -1},
        {-1, 4,  1,  2, -1,  1,  3, -2,  1,  2},
        {2, -1,  3,  1,  2, -3,  1,  4, -1,  1},
        {1,  3, -2,  4,  1,  1, -1,  2,  3, -2},
        {4,  1,  1, -1,  3,  2,  1, -3,  1,  1},
        {-2, 2,  3,  1, -2,  1,  4,  1, -1,  3},
        {1, -1,  1,  3,  2, -1,  2,  1,  4, -2},
        {3,  2, -3,  1,  1,  3, -1,  2,  1,  4},
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = data[i][j];
        }
    }

    Vector a(n);
    double a_data[10] = {15, 8, 12, 6, 10, 14, 9, 11, 7, 13};
    for (int i = 0; i < n; i++)
        a[i] = a_data[i];

    std::cout << "Матриця A:\n";
    A.print();

    std::cout << "\nВектор a:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "a[" << i + 1 << "] = " << a[i] << std::endl;
    }

    LUP lup(A);
    lup.printLU();
    Vector x = lup.solve(a);

    std::cout << "\nРозв'язок системи\n";
    x.print();

    Vector check = A * x;
    std::cout << "\nПеревірка (A * x)\n";
    for (int i = 0; i < n; i++) {
        std::cout << "A * x [" << i + 1 << "] = " << std::setprecision(2) << check[i]  << std::endl;
    }

    return 0;
}