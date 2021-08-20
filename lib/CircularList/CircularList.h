//
// Created by jlaustill on 8/5/21.
//
#include <Arduino.h>

#ifndef NPCM_CIRCULARLIST_H
#define NPCM_CIRCULARLIST_H

template <typename T>
class CircularList {
private:
    T* data;
    uint8_t size;
    uint8_t index = 0;

public:
    explicit CircularList(uint8_t _size);
    void next(T _next);
    T average();
};


#endif //NPCM_CIRCULARLIST_H
