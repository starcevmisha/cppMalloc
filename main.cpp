#include <iostream>
#include <fstream>


using namespace std;

struct Descriptor {
    bool isFree;
    int size;
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
                Descriptor *newDescriptor = (struct Descriptor *)(memory_location + currentDescriptor->size);
                newDescriptor->isFree = true;
                newDescriptor->size = available;
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
    //TODO просмотреть следующий блок и объеденить с ним или с предыдущим
    return;
}

void Dump() {
    void *current_location;
    current_location = managed_memory_start;
    struct Descriptor *currentDescriptor;
    while (current_location < last_valid_address) {
        currentDescriptor = (struct Descriptor *) current_location;
        std::cout << currentDescriptor->size<<' '<<currentDescriptor->isFree <<endl;
        current_location += currentDescriptor->size + sizeof(Descriptor);
    }

}


int main() {

    void *qwe = static_cast<char *>(alloc(101));
    void *qwe2 = static_cast<char *>(alloc(102));
    void *qwe3 = static_cast<char *>(alloc(103));
    void *qwe4 = static_cast<char *>(alloc(104));

//    std::cout << ((Descriptor *) (qwe - sizeof(struct Descriptor)))->size << endl;
    Descriptor* a = static_cast<Descriptor *>(qwe- sizeof(Descriptor));
    std::cout << qwe - sizeof(struct Descriptor)<<endl;
    free(qwe2);
    Dump();

}