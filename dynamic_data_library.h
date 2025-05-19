#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

// Введення значень (спеціалізація для string)
template<typename T>
void inputValue(T& val) {
    std::cin >> val;
}

template<>
inline void inputValue<std::string>(std::string& val) {
    std::cin.ignore();
    std::getline(std::cin, val);
}

// ------------------ СТІК -------------------
template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& v) : data(v), next(nullptr) {}
    };
    Node* top;
public:
    Stack() noexcept : top(nullptr) {}
    ~Stack() { clear(); }

    void push(const T& val) {
        Node* node = new Node(val);
        node->next = top;
        top = node;
    }

    void pop() {
        if (!top) throw std::underflow_error("Стек порожній");
        Node* tmp = top;
        top = top->next;
        delete tmp;
    }

    T& peek() const {
        if (!top) throw std::underflow_error("Стек порожній");
        return top->data;
    }

    bool isEmpty() const noexcept { return top == nullptr; }

    void clear() {
        while (!isEmpty())
            pop();
    }

    void print() const {
        for (Node* curr = top; curr; curr = curr->next)
            std::cout << curr->data << " ";
        std::cout << std::endl;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) throw std::ios_base::failure("Неможливо відкрити файл для запису");
        std::vector<T> elems;
        for (Node* curr = top; curr; curr = curr->next)
            elems.push_back(curr->data);
        size_t n = elems.size();
        ofs.write(reinterpret_cast<const char*>(&n), sizeof(n));
        for (const auto& v : elems)
            ofs.write(reinterpret_cast<const char*>(&v), sizeof(T));
    }

    void loadFromFile(const std::string& filename) {
        clear();
        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) throw std::ios_base::failure("Неможливо відкрити файл для читання");
        size_t n;
        ifs.read(reinterpret_cast<char*>(&n), sizeof(n));
        std::vector<T> elems(n);
        for (size_t i = 0; i < n; ++i)
            ifs.read(reinterpret_cast<char*>(&elems[i]), sizeof(T));
        for (auto it = elems.rbegin(); it != elems.rend(); ++it)
            push(*it);
    }

    void sortStack(std::function<bool(const T&, const T&)> cmp = std::less<T>()) {
        std::vector<T> elems;
        while (!isEmpty()) {
            elems.push_back(peek());
            pop();
        }
        std::sort(elems.begin(), elems.end(), cmp);
        for (auto it = elems.rbegin(); it != elems.rend(); ++it)
            push(*it);
    }
};

// ------------------ ЧЕРГА -------------------
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& v) : data(v), next(nullptr) {}
    };
    Node* front;
    Node* rear;
public:
    Queue() noexcept : front(nullptr), rear(nullptr) {}
    ~Queue() { clear(); }

    void enqueue(const T& v) {
        Node* node = new Node(v);
        if (rear) rear->next = node;
        else front = node;
        rear = node;
    }

    void dequeue() {
        if (!front) throw std::underflow_error("Черга порожня");
        Node* tmp = front;
        front = front->next;
        if (!front) rear = nullptr;
        delete tmp;
    }

    T& peek() const {
        if (!front) throw std::underflow_error("Черга порожня");
        return front->data;
    }

    bool isEmpty() const noexcept { return front == nullptr; }

    void clear() {
        while (!isEmpty())
            dequeue();
    }

    void print() const {
        for (Node* curr = front; curr; curr = curr->next)
            std::cout << curr->data << " ";
        std::cout << std::endl;
    }

    void sortQueue(std::function<bool(const T&, const T&)> cmp = std::less<T>()) {
        std::vector<T> elems;
        while (!isEmpty()) {
            elems.push_back(peek());
            dequeue();
        }
        std::sort(elems.begin(), elems.end(), cmp);
        for (const auto& v : elems)
            enqueue(v);
    }
};

// -------- ДВОЗВ’ЯЗНИЙ КРУГОВИЙ СПИСОК ---------
template<typename T>
class DoublyCircularList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& v) : data(v), next(this), prev(this) {}
    };
    Node* head;
public:
    DoublyCircularList() noexcept : head(nullptr) {}
    ~DoublyCircularList() { clear(); }

    bool isEmpty() const noexcept { return head == nullptr; }

    void insertBack(const T& v) {
        Node* node = new Node(v);
        if (!head) head = node;
        else {
            Node* tail = head->prev;
            tail->next = node;
            node->prev = tail;
            node->next = head;
            head->prev = node;
        }
    }

    Node* find(const T& key) const {
        if (!head) return nullptr;
        Node* curr = head;
        do {
            if (curr->data == key) return curr;
            curr = curr->next;
        } while (curr != head);
        return nullptr;
    }

    void removeAll(const T& key) {
        if (!head) return;
        Node* curr = head;
        bool found = false;
        do {
            if (curr->data == key) {
                found = true;
                Node* del = curr;
                del->prev->next = del->next;
                del->next->prev = del->prev;
                curr = curr->next;
                if (del == head) head = (del->next != del ? del->next : nullptr);
                delete del;
                if (!head) break;
            }
            else curr = curr->next;
        } while (curr != head);
        if (!found) throw std::runtime_error("Елемент не знайдено");
    }

    void print() const {
        if (!head) { std::cout << "Список порожній\n"; return; }
        Node* curr = head;
        do {
            std::cout << curr->data << " ";
            curr = curr->next;
        } while (curr != head);
        std::cout << std::endl;
    }

    void clear() {
        if (!head) return;
        Node* curr = head->next;
        while (curr != head) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
        delete head;
        head = nullptr;
    }

    void sortList(std::function<bool(const T&, const T&)> cmp = std::less<T>()) {
        if (!head || head->next == head) return;
        std::vector<T> elems;
        Node* curr = head;
        do {
            elems.push_back(curr->data);
            curr = curr->next;
        } while (curr != head);
        std::sort(elems.begin(), elems.end(), cmp);
        clear();
        for (const auto& v : elems)
            insertBack(v);
    }
};

// ------------------ МЕНЮ --------------------

template<typename T>
void showMenu() {
    Stack<T> st;
    Queue<T> qu;
    DoublyCircularList<T> lst;

    int structureChoice;
    do {
        std::cout << "\n1.Стек  2.Черга  3.Список  0.Повернутися до вибору типу\nВибір: ";
        std::cin >> structureChoice;

        switch (structureChoice) {
        case 1: {
            int c;
            do {
                std::cout << "\nСтек: 1.Додати  2.Видалити  3.Верхній елемент  4.Друк  5.Зберегти у файл  6.Завантажити з файлу  7.Сортувати  0.Повернутися\nВибір: ";
                std::cin >> c;
                try {
                    if (c == 1) {
                        T x;
                        std::cout << "Введіть значення: ";
                        inputValue(x);
                        st.push(x);
                    }
                    else if (c == 2) st.pop();
                    else if (c == 3) std::cout << "Верхній елемент: " << st.peek() << "\n";
                    else if (c == 4) st.print();
                    else if (c == 5) {
                        std::string filename; std::cout << "Назва файлу: "; std::cin >> filename;
                        st.saveToFile(filename);
                    }
                    else if (c == 6) {
                        std::string filename; std::cout << "Назва файлу: "; std::cin >> filename;
                        st.loadFromFile(filename);
                    }
                    else if (c == 7) {
                        st.sortStack();
                    }
                }
                catch (const std::exception& ex) { std::cout << "Помилка: " << ex.what() << "\n"; }
            } while (c != 0);
            break;
        }
        case 2: {
            int c;
            do {
                std::cout << "\nЧерга: 1.Додати  2.Видалити  3.Передній елемент  4.Друк  5.Сортувати  0.Повернутися\nВибір: ";
                std::cin >> c;
                try {
                    if (c == 1) {
                        T x;
                        std::cout << "Введіть значення: ";
                        inputValue(x);
                        qu.enqueue(x);
                    }
                    else if (c == 2) qu.dequeue();
                    else if (c == 3) std::cout << "Передній елемент: " << qu.peek() << "\n";
                    else if (c == 4) qu.print();
                    else if (c == 5) {
                        qu.sortQueue();
                    }
                }
                catch (const std::exception& ex) { std::cout << "Помилка: " << ex.what() << "\n"; }
            } while (c != 0);
            break;
        }
        case 3: {
            int c;
            do {
                std::cout << "\nСписок: 1.Додати в кінець  2.Знайти  3.Видалити всі  4.Друк  5.Сортувати  0.Повернутися\nВибір: ";
                std::cin >> c;
                try {
                    if (c == 1) {
                        T x;
                        std::cout << "Введіть значення: ";
                        inputValue(x);
                        lst.insertBack(x);
                    }
                    else if (c == 2) {
                        T x;
                        std::cout << "Значення для пошуку: ";
                        inputValue(x);
                        auto node = lst.find(x);
                        if (node) std::cout << "Знайдено: " << node->data << "\n";
                        else std::cout << "Не знайдено\n";
                    }
                    else if (c == 3) {
                        T x;
                        std::cout << "Значення для видалення: ";
                        inputValue(x);
                        lst.removeAll(x);
                    }
                    else if (c == 4) lst.print();
                    else if (c == 5) lst.sortList();
                }
                catch (const std::exception& ex) { std::cout << "Помилка: " << ex.what() << "\n"; }
            } while (c != 0);
            break;
        }
        }
    } while (structureChoice != 0);
}

void showMainMenu() {
    std::cout << "Виберіть тип даних:\n"
        << "1. int\n"
        << "2. float\n"
        << "3. char\n"
        << "4. string\n"
        << "0. Вихід\nВибір: ";
}
