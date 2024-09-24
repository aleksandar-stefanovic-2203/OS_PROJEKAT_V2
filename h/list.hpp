#ifndef LIST_H
#define LIST_H

#include "MemoryAllocator.hpp"

template <typename T> class List {
public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;
    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T* item) {
        Element* element = new Element(item, head);
        head = element;
        if(!tail) tail = element;
    }

    void addLast(T* item) {
        Element* element = new Element(item, nullptr);
        if(tail) {
            tail->next = element;
            tail = element;
        } else head = tail = element;
    }

    T* removeFirst() {
        if(!head) return nullptr;
        Element* element = head;
        head = head->next;
        if(!head) tail = nullptr;

        T* result = element->item;
        delete element;
        return result;
    }

    T* removeLast() {
        if(!tail) return nullptr;

        Element* prev = nullptr;
        for(Element* curr = head; curr != tail; prev = curr, curr = curr->next) {}

        Element* element = tail;
        if(prev) prev->next = nullptr;
        else head = nullptr;
        tail = prev;

        T* result = element->item;
        delete element;
        return result;
    }

    T* peekFirst() {
        if(!head) return nullptr;
        return head->item;
    }

    T* peekLast() {
        if(!tail) return nullptr;
        return tail->item;
    }

private:
    struct Element {
        T* item;
        Element* next;

        Element(T* item, Element* next) : item(item), next(next) {}

        void* operator new(size_t size){
            return MemoryAllocator::mem_alloc(size);
        }

        void* operator new[](size_t size){
            return MemoryAllocator::mem_alloc(size);
        }

        void operator delete(void* ptr){
            MemoryAllocator::mem_free(ptr);
        }

        void operator delete[](void* ptr){
            MemoryAllocator::mem_free(ptr);
        }
    };

    Element* head, *tail;
};

#endif