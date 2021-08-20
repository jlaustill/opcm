//
// Created by jlaustill on 8/5/21.
//
#include <Arduino.h>
#include "CircularList.h"

template<typename T>
CircularList<T>::CircularList(uint8_t _size) {
    this->data = new T[_size];
    this->size = _size;
}

template<typename T>
void CircularList<T>::next(T _next) {
    this->data[this->index] = _next;

    if (++this->index >= this->size) {
        this->index = 0;
    }
}

template<typename T>
T CircularList<T>::average() {
    unsigned long sum = 0;

    for (uint8_t i = 0; i < size; i++) {
        sum += this->data[i];
    }

    return sum / size;
}
