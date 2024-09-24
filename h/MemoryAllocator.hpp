#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include "../lib/hw.h"

// Struktura u kojoj se cuvaju podaci o slobodnom ili zauzetom segmentu
struct Segment{
    Segment* next; // Pokazivac na sledeci slobodan ili zauzet segment (segmenti su sortirani od nizih ka visim adresama)
    size_t size; // Velicina segmenta u bajtovima
};

class MemoryAllocator {
public:
  static void* mem_alloc(size_t size);
  static int mem_free(void*);
  static void initFreeSegment();

protected:
  MemoryAllocator() {};
  static void tryToJoin(Segment* segment); // Pokusaj spajanja datog segmenta sa sledecim
  static void insert_segment(Segment* segment, Segment* prev, int code); // code == 0 -> rad sa listom slobodnih segmenata
  static void remove_segment(Segment* segment, Segment* prev, int code); // code == 1 -> rad sa listom zauzetih segmenata

private:
  static Segment* head_free_segment; // Pokazivac na prvi slobodan segment
  static Segment* head_data_segment; // Pokazivac na prvi zauzet segment
};

#endif