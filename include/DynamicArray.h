#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

template<typename T>
class DynamicArray {
public:
    T *data;
    int size, cap;

    DynamicArray(int initCap = 4) : size(0), cap(initCap) { data = new T[cap]; }
    ~DynamicArray() { delete[] data; }

    void expand(int newCap) {
        T *newData = new T[newCap];
        for(int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    void push_back(const T &item) {
        if(size >= cap) expand(cap * 2);
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

    void swap(DynamicArray<T>& other) noexcept {

        T* tempData = data;
        data = other.data;
        other.data = tempData;

        int tempSize = size;
        size = other.size;
        other.size = tempSize;

        int tempCap = cap;
        cap = other.cap;
        other.cap = tempCap;
    }
};

#endif