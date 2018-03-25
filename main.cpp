#include <iostream>
#include <fstream>


using namespace std;

struct Descriptor {
    bool isFree;
    int size;
    Descriptor *prev;
};

int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;


void *alloc(int num_bytes);

void free(void *ptr);

void Dump();//Количество блоков




void init() {
    int size = 1000;
    managed_memory_start = malloc(size);
    last_valid_address = managed_memory_start + size;
    has_initialized = 1;

    auto *firstDescriptor = (struct Descriptor *) managed_memory_start;
    firstDescriptor->isFree = true;
    firstDescriptor->size = size - sizeof(struct Descriptor);
    firstDescriptor->prev = NULL;

}

void *alloc(int num_bytes) {

    if (!has_initialized)
        init();

    struct Descriptor *currentDescriptor;

    void *memory_location;
    memory_location = 0;

    void *current_location;
    current_location = managed_memory_start;

    while (current_location < last_valid_address) {
        currentDescriptor = (struct Descriptor *) current_location;
        if (currentDescriptor->isFree && currentDescriptor->size >= num_bytes) {
            int oldBlockSize = currentDescriptor->size;
            //Нашли блок
            currentDescriptor->isFree = false;
            currentDescriptor->size = num_bytes;
            memory_location = current_location + sizeof(Descriptor);

            int available = oldBlockSize - sizeof(Descriptor) - num_bytes;

            if (available > 0) {
                Descriptor *newDescriptor = (struct Descriptor *) (memory_location + currentDescriptor->size);
                newDescriptor->isFree = true;
                newDescriptor->size = available;
                newDescriptor->prev = currentDescriptor;
            }
            break;
        }
        current_location += currentDescriptor->size + sizeof(Descriptor);
    }

    if (!memory_location)
        throw std::bad_alloc();

//    memory_location += sizeof(struct Descriptor);
    return memory_location;

}


void free(void *ptr) {
    struct Descriptor *descriptor;
    descriptor = (struct Descriptor *) (ptr - sizeof(struct Descriptor));

    descriptor->isFree = true;
    struct Descriptor *next = (Descriptor *) ((char *) descriptor + descriptor->size + sizeof(Descriptor));
    if (next < last_valid_address && next->isFree) {
        descriptor->size += next->size + sizeof(Descriptor);
    }

    if (descriptor->prev != NULL && descriptor->prev->isFree) {
        descriptor->prev->size += descriptor->size + sizeof(Descriptor);
    }
    //TODO просмотреть следующий блок и объеденить с ним или с предыдущим
    return;
}

void Dump() {
    void *current_location;
    current_location = managed_memory_start;
    struct Descriptor *currentDescriptor;
    while (current_location < last_valid_address) {
        currentDescriptor = (struct Descriptor *) current_location;
        std::cout << currentDescriptor->size << ' ' << currentDescriptor->isFree << endl;
        current_location += currentDescriptor->size + sizeof(Descriptor);
    }

}


int main() {
    void *qwe = (alloc(11));
    void *qwe2 = (alloc(12));
    void *qwe3 = (alloc(13));
    void *qwe4 = (alloc(14));
    void *qwe5 = (alloc(15));
    void *qwe6 = (alloc(16));
    Dump();
    std::cout<<endl;

    free(qwe3);
    free(qwe5);
    free(qwe4);

    Dump();

}