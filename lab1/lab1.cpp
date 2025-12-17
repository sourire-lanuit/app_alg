#include <iostream>
#include <chrono>

struct Set {
    double el;
    Set* next;
    Set(double num) : el(num), next(nullptr) {}
};

class OrdSet {
private:
    Set* head;

public:
    OrdSet() : head(nullptr) {}
    
    void Insert(double num) {
        if (!head || num < head->el) {
            if (!head || head->el != num) {
                Set* newSet = new Set(num);
                newSet->next = head;
                head = newSet;
            }
            return;
        }
        Set* current = head;
        while (current->next && current->next->el < num) {
            current = current->next;
        }
        if (current->next && current->next->el == num) return;
        Set* newSet = new Set(num);
        newSet->next = current->next;
        current->next = newSet;
    }

    void Delete(double num) {
        if (!head) return;
        if (head->el == num) {
            Set* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Set* current = head;
        while (current->next && current->next->el != num) {
            current = current->next;
        }
        if (current->next) {
            Set* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    bool Search(double num) const {
        Set* current = head;
        while (current && current->el <= num) {
            if (current->el == num) return true;
            current = current->next;
        }
        return false;
    }

    void Clear() {
        Set* current = head;
        while (current) {
            Set* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

    static OrdSet Union(const OrdSet& A, const OrdSet& B) {
        OrdSet result;
        Set* a = A.head;
        Set* b = B.head;
        while (a || b) {
            if (!b || (a && a->el < b->el)) {
                result.Insert(a->el);
                a = a->next;
            } else if (!a || (b && b->el < a->el)) {
                result.Insert(b->el);
                b = b->next;
            } else {
                result.Insert(a->el);
                a = a->next;
                b = b->next;
            }
        }
        return result;
    }

    static OrdSet Intersection(const OrdSet& A, const OrdSet& B) {
        OrdSet result;
        Set* a = A.head;
        Set* b = B.head;
        while (a && b) {
            if (a->el == b->el) {
                result.Insert(a->el);
                a = a->next;
                b = b->next;
            } else if (a->el < b->el) {
                a = a->next;
            } else {
                b = b->next;
            }
        }
        return result;
    }

    static OrdSet SetDifference(const OrdSet& A, const OrdSet& B) {
        OrdSet result;
        Set* a = A.head;
        Set* b = B.head;
        while (a) {
            if (!b || a->el < b->el) {
                result.Insert(a->el);
                a = a->next;
            } else if (a->el == b->el) {
                a = a->next;
                b = b->next;
            } else {
                b = b->next;
            }
        }
        return result;
    }

    static OrdSet SymDifference(const OrdSet& A, const OrdSet& B) {
        OrdSet sym_diffAB = SetDifference(A, B);
        OrdSet sym_diffBA = SetDifference(B, A);
        return Union(sym_diffAB, sym_diffBA);
    }

    static bool IsSubset(const OrdSet& A, const OrdSet& B) {
        Set* a = A.head;
        while (a) {
            if (!B.Search(a->el)) return false;
            a = a->next;
        }
        return true;
    }

     bool Contains(const OrdSet& other) const {
        Set* current = other.head;
        while (current) {
            if (!Search(current->el)) return false;
            current = current->next;
        }
        return true;
    }

    ~OrdSet() { Clear(); }
};


int main() {
    OrdSet A, B;

    int n;
    std::cout << "Enter n: ";
    std::cin >> n;
    srand(time(nullptr));

    double num = (double)rand() / RAND_MAX * 100000.0; 
    A.Insert(num);
    B.Insert(num);

    std::cout << "Two sets are created with " << n << " elements each.\n";

    double elements;
    while (true) {
        elements = (double)rand() / RAND_MAX * 100000.0;
        if (A.Search(elements)) break; 
    }

    double not_in;
    while (true) {
        not_in = (double)rand() / RAND_MAX * 100000.0;
        if (!A.Search(not_in)) break;  
    }

    auto start = std::chrono::high_resolution_clock::now();
    A.Search(elements);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Search time for in-set element: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    A.Search(not_in);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Search time for not-in-set element: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    double new_el;
    while (true) {
        new_el = (double)rand() / RAND_MAX * 100000.0;
        if (!A.Search(new_el)) break;  
    }

    start = std::chrono::high_resolution_clock::now();
    A.Insert(new_el);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Insert time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    A.Delete(new_el);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Delete time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    OrdSet C = OrdSet::Union(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Union time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    OrdSet D = OrdSet::Intersection(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Intersection time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    OrdSet E = OrdSet::SetDifference(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Difference time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    start = std::chrono::high_resolution_clock::now();
    OrdSet F = OrdSet::SymDifference(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SymDifference time: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    std::cout << "IsSubset check: " << (OrdSet::IsSubset(A, B) ? "true" : "false") << std::endl;

    bool correct_Union = C.Contains(A) && C.Contains(B);
    std::cout << "Union is correct: " << (correct_Union ? "Yes" : "No") << std::endl;

    return 0;
}