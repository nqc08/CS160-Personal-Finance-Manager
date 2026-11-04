#pragma once

template<typename T>
class DynamicArray {
public:
    T *data;
    int size, cap;

    DynamicArray(int initCap = 4) : size(0), cap(initCap) { data = new T[cap]; }
    ~DynamicArray() { delete[] data; }

    void resize(int newCap) {
        T *newData = new T[newCap];
        for(int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    void push_back(const T &item) {
        if(size >= cap) resize(cap * 2);
        data[size++] = item;
    }

    void remove(int idx) {
        if(idx < 0 || idx >= size) return;
        for(int i = idx; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    T& operator[] (int idx) { return data[idx]; }
    const T& operator[] (int idx) const { return data[idx]; }
};