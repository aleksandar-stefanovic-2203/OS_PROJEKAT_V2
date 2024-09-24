#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

Segment* MemoryAllocator::head_free_segment;
Segment* MemoryAllocator::head_data_segment;

void* MemoryAllocator::mem_alloc(size_t size) {
    if(size == 0) return nullptr; // Greska
    size_t new_size = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    for(Segment* segment = head_free_segment, *prev = nullptr ; segment; prev = segment, segment = segment->next) {
        if(segment->size >= new_size) {
            remove_segment(segment, prev, 0); // Izbacivanje slobodnog segmenta iz liste slobodnih segmenata

            size_t data_size;
            if(segment->size - new_size >= sizeof(Segment)) {
                Segment* new_segment = (Segment*) ((char*) segment + sizeof(Segment) + new_size);
                new_segment->size = segment->size - new_size - sizeof(Segment);
                insert_segment(new_segment, prev, 0); // Ubacivanje slobodnog segmenta koji je ostao nakon alokacije
                data_size = new_size;
            } else {
                data_size = segment->size;
            }

            Segment* data_segment = segment;
            data_segment->size = data_size;


            for(segment = head_data_segment, prev = nullptr; segment && segment < data_segment; prev = segment, segment = segment->next){}
            insert_segment(data_segment, prev, 1); // Ubacivanje segmenta u listu zauzetih segmenata

            return (char*)data_segment + sizeof(Segment);
        }
    }

    return nullptr; // Nije pronadjen slobodan segment dovoljne velicine
}

int MemoryAllocator::mem_free(void* ptr) {
    if(ptr == nullptr || ptr < HEAP_START_ADDR || ptr >= HEAP_END_ADDR) return -1; // Adresa se nalazi van opsega

    for(Segment* segment = head_data_segment, *prev = nullptr; segment; prev = segment, segment = segment->next) {
        if((char*) segment + sizeof(Segment) == (char*) ptr) {
            remove_segment(segment, prev, 1); // Izbacivanje segmenta iz liste zauzetih segmenata
            for(prev = head_free_segment; prev && prev->next && prev->next < segment; prev = prev->next){}
            if(prev > segment) prev = nullptr;
            insert_segment(segment, prev, 0); // Ubacivanje segmenta u listu slobodnih segmenata
            tryToJoin(segment);
            tryToJoin(prev);

            return 0; //OK
        }
        if((char*) segment + sizeof(Segment) > (char*) ptr) break;
    }

    return -2; // Data adresa nije dobijena sa mem_alloc
}

void MemoryAllocator::tryToJoin(Segment *segment) {
    if(!segment || !segment->next) return;
    if((char*) segment + sizeof(Segment) + segment->size == (char*) segment->next) {
        segment->size += sizeof(Segment) + segment->next->size;
        segment->next = segment->next->next;
    }
}

void MemoryAllocator::insert_segment(Segment* segment, Segment* prev, int code) {
    if(!segment || code < 0 || code > 1) return;
    if(!prev) {
        if(code == 0) {
            segment->next = head_free_segment;
            head_free_segment = segment;
        } else {
            segment->next = head_data_segment;
            head_data_segment = segment;
        }
    }
    else {
        segment->next = prev->next;
        prev->next = segment;
    }
}

void MemoryAllocator::remove_segment(Segment* segment, Segment* prev, int code) {
    if(!segment || code < 0 || code > 1) return;
    if(!prev) {
        if(code == 0) head_free_segment = segment->next;
        else head_data_segment = segment->next;
    }
    else prev->next = segment->next;
    segment->next = nullptr;
}

void MemoryAllocator::initFreeSegment() {
    head_free_segment = (Segment*) HEAP_START_ADDR;
    head_data_segment = nullptr;

    head_free_segment->next = nullptr;
    head_free_segment->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(Segment);
}
