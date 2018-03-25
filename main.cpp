#include <iostream>
#include <fstream>
#include <winnt.h>
#include <fileapi.h>

using namespace std;

struct Descriptor {
    bool isFree;
    int size;
};

int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;
Descriptor *firstDescriptor;


void *alloc(int num_bytes);

void free(void *ptr);

void Dump();//Количество блоков




void init() {
    managed_memory_start = malloc(1000);
    last_valid_address = managed_memory_start + 1000;
    has_initialized = 1;

    firstDescriptor = static_cast<Descriptor *>(managed_memory_start);
    firstDescriptor->isFree = false;
    firstDescriptor->size = 1000 - sizeof(struct Descriptor);

}

void *alloc(int num_bytes) {
//    void *ret = first;
//    first += num_bytes;
//    return ret;

    struct Descriptor *currentDescriptor;

    if (!has_initialized)
        init();

    void *memory_location;
    memory_location = 0;

    void *current_location;

    current_location = managed_memory_start;

    while (current_location != last_valid_address){
        currentDescriptor = (struct Descriptor *)current_location;
        if (currentDescriptor->size >= num_bytes){
            //Нашли блок
        }
    }


}

void free(void *ptr) {
    struct Descriptor *descriptor;
    descriptor = static_cast<Descriptor *>(ptr - sizeof(struct Descriptor));

    descriptor->isFree = 1;
    return;
}


int main() {
    void *qwe = new int[10000];
    std::cout << "Hello";
}