#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef _DEBUG
#include <cassert>
#include <cstring>
#endif // _DEBUG

template <typename T>
class LinkedList
{
private:
    T value;
    LinkedList<T>* next;
    bool init;
    int size;
public:
    LinkedList();
    LinkedList(const T&);
    LinkedList(const int&, const T&);
    void push_back(const T&);
    void pop_back();
    void push_front(const T&);
    void pop_front();
    void insert_at(const int&, const T&);
    void delete_at(const int&);
    int find(const T&) const;
    T at(const int&) const;
    int get_size() const;
    void clear();
    ~LinkedList();

#ifdef _DEBUG
    friend void tests();
#endif // _DEBUG
};

// конструктор по умолчанию
template <typename T>
LinkedList<T>::LinkedList() : next(nullptr), init(false), size(0) {}

// создает одноэлементный список с заданным значением
template <typename T>
LinkedList<T>::LinkedList(const T& val) : value(val), next(nullptr),
                                   init(true), size(1) {}

// создает список с count элементами, в каждом содержится val
template <typename T>
LinkedList<T>::LinkedList(const int& count, const T& val)
{
    if (count <= 0)
        throw "Invalid size";
    
    this->value = val;
    this->next = nullptr;
    this->init = true;
    this->size = 1;

    if (count == 1)
        return;

    int temp_count = count;
    LinkedList<T>* curr = this;
    while (--temp_count)
    {
        curr->next = new LinkedList<T>(val);
        curr = curr->next;
    }
}

// добавляет элемент в конец
template<typename T>
void LinkedList<T>::push_back(const T& val)
{
    if (!this->init)
    {
        this->value = val;
        this->init = true;
        this->size = 1;
        return;
    }

    LinkedList<T>* curr = this;
    while (curr->next != nullptr)
        curr = curr->next;
    curr->next = new LinkedList<T>(val);
    this->size++;
}

// удаляет элемент из конца
template<typename T>
void LinkedList<T>::pop_back()
{
    if (this->size == 0)
        throw "Cannot delete from empty list";

    if (this->next == nullptr)
    {
        value = T();
        init = false;
        size = 0;
        return;
    }
    
    LinkedList<T>* curr = this;
    while (curr->next->next != nullptr)
        curr = curr->next;
    delete curr->next;
    curr->next = nullptr;
    this->size--;
}

// добавляет элемент в начало
template<typename T>
void LinkedList<T>::push_front(const T& val)
{
    if (!this->init)
    {
        this->value = val;
        this->init = true;
        this->size = 1;
        return;
    }

    LinkedList<T>* new_node = new LinkedList<T>(val);
    new_node->next = this->next;
    this->next = new_node;

    T temp;
    temp = this->value;
    this->value = new_node->value;
    new_node->value = temp;

    this->size++;
}

// удаляет элемент из начала
template<typename T>
void LinkedList<T>::pop_front()
{
    if (this->size == 0)
        throw "Cannot delete from empty list";
    
    if (this->next == nullptr)
    {
        value = T();
        init = false;
        size = 0;
        return;
    }
    
    T temp;
    temp = this->value;
    this->value = this->next->value;
    this->next->value = temp;

    LinkedList<T>* temp_node = this->next;
    this->next = temp_node->next;
    temp_node->next = nullptr;
    delete temp_node;

    this->size--;
}

// добавляет элемент по индексу
template <typename T>
void LinkedList<T>::insert_at(const int& index, const T& value)
{
    if (index < 0 || index >= this->size)
        throw "Index out of range";
    
    if (index == 0)
    {
        this->push_front(value);
        return;
    }
    
    LinkedList<T>* curr = this;
    int count = 0;
    
    while (count != index - 1)
    {
        curr = curr->next;
        count++;
    }
    
    LinkedList<T>* new_node = new LinkedList<T>(value);
    new_node->next = curr->next;
    curr->next = new_node;
    this->size++;
}

// удаляет элемент по индексу
template <typename T>
void LinkedList<T>::delete_at(const int& index)
{
    if (index < 0 || index >= this->size)
        throw "Index out of range";
    
    if (index == 0)
    {
        this->pop_front();
        return;
    }
    
    if (index == this->size - 1)
    {
        this->pop_back();
        return;
    }
    
    LinkedList<T>* curr = this;
    int count = 0;
    
    while (count != index - 1)
    {
        curr = curr->next;
        count++;
    }
    
    LinkedList<T>* next_node = curr->next->next;
    curr->next->next = nullptr;
    delete curr->next;
    curr->next = next_node;
    this->size--;
}

// возвращает первый индекс, по которому найден элемент
template<typename T>
int LinkedList<T>::find(const T& val) const
{
    const LinkedList<T>* curr = this;
    int index = 0;
    while (curr->value != val)
    {
        if (curr->next == nullptr)
            return -1;
        curr = curr->next;
        index++;
    }

    return index;
}

// возвращает значение по индексу
template <typename T>
T LinkedList<T>::at(const int& index) const
{
    if (index < 0 || index >= this->size)
        throw "Index out of range";

    const LinkedList<T>* curr = this;
    int count = 0;
    while (count != index)
    {
        curr = curr->next;
        count++;
    }

    return curr->value;
}

// возвращает размер списка
template <typename T>
int LinkedList<T>::get_size() const
{
    return this->size;
}

// очищает список
template <typename T>
void LinkedList<T>::clear()
{
    this->~LinkedList();
    this->next = nullptr;
    this->value = T();
    this->init = false;
    this->size = 0;
}

// деструктор
template <typename T>
LinkedList<T>::~LinkedList()
{
    LinkedList<T>* curr = this->next, *next;
    while (curr)
    {
        next = curr->next;
        curr->next = nullptr;
        delete curr;
        curr = next;
    }
}

#ifdef _DEBUG
// тесты
void tests()
{
    LinkedList<int> l;
    assert(l.size == 0);
    assert(l.init == false);
    assert(l.next == nullptr);

    try {l.pop_back();}
    catch (const char* error)
    {assert(strcmp(error, "Cannot delete from empty list") == 0);}

    try {l.pop_front();}
    catch (const char* error)
    {assert(strcmp(error, "Cannot delete from empty list") == 0);}

    l.push_front(10);
    assert(l.init == true);

    l.push_back(20);
    assert(l.next != nullptr);

    l.push_front(30);
    assert(l.size == 3);

    assert(l.at(0) == 30);
    assert(l.at(1) == 10);
    assert(l.at(2) == 20);

    try {l.at(3);}
    catch (const char* error)
    {assert(strcmp(error, "Index out of range") == 0);}

    assert(l.find(20) == 2);
    assert(l.find(40) == -1);

    l.delete_at(1);
    assert(l.at(1) == 20);
    assert(l.size == 2);

    l.insert_at(0, 50);
    assert(l.at(0) == 50);
    assert(l.size == 3);

    l.pop_back();
    l.pop_front();

    assert(l.at(0) == 30);
    assert(l.size == 1);

    l.clear();
    l.push_back(1);
    assert(l.init == true);
    assert(l.size == true);
    assert(l.next == nullptr);
}
#endif // _DEBUG

#endif // LINKED_LIST_H