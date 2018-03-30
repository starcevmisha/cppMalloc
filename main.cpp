#include <iostream>
#include <fstream>


using namespace std;

struct Descriptor {
    bool isFree;
    int size;
    Descriptor *prev;
};

int hasInitialized = 0;
void *managedMemoryStart;
void *lastValidAddress;


void *alloc(int numBytes);

void free(void *ptr);

void Dump();//Количество блоков




void init() {
    int size = 10000;
    managedMemoryStart = malloc(size);
    lastValidAddress = managedMemoryStart + size;
    hasInitialized = 1;

    auto *firstDescriptor = (struct Descriptor *) managedMemoryStart;
    firstDescriptor->isFree = true;
    firstDescriptor->size = size - sizeof(struct Descriptor);
    firstDescriptor->prev = NULL;

}

void *alloc(int numBytes) {

    if (!hasInitialized)
        init();

    struct Descriptor *currentDescriptor;

    void *memoryLocation;
    memoryLocation = 0;

    void *currentLocation;
    currentLocation = managedMemoryStart;

    while (currentLocation < lastValidAddress) {
        currentDescriptor = (struct Descriptor *) currentLocation;
        if (currentDescriptor->isFree && currentDescriptor->size >= numBytes) {
            int oldBlockSize = currentDescriptor->size;
            //Нашли блок
            currentDescriptor->isFree = false;
            currentDescriptor->size = numBytes;
            memoryLocation = currentLocation + sizeof(Descriptor);

            int available = oldBlockSize - sizeof(Descriptor) - numBytes;

            if (available > 0) {
                Descriptor *newDescriptor = (struct Descriptor *) (memoryLocation + currentDescriptor->size);
                newDescriptor->isFree = true;
                newDescriptor->size = available;
                newDescriptor->prev = currentDescriptor;
            }
            break;
        }
        currentLocation += currentDescriptor->size + sizeof(Descriptor);
    }

    if (!memoryLocation)
        throw std::bad_alloc();

//    memoryLocation += sizeof(struct Descriptor);
    return memoryLocation;

}


void free(void *ptr) {
    struct Descriptor *descriptor;
    descriptor = (struct Descriptor *) (ptr - sizeof(struct Descriptor));

    descriptor->isFree = true;

    //Предыдущий дескриптор
    struct Descriptor *next = (Descriptor *) ((char *) descriptor + descriptor->size + sizeof(Descriptor));
    if (next < lastValidAddress && next->isFree) {
        descriptor->size += next->size + sizeof(Descriptor);
    }
    //Следующий
    if (descriptor->prev != NULL && descriptor->prev->isFree) {
        int size = descriptor->size + sizeof(Descriptor);
        descriptor = descriptor->prev;
        descriptor->size += size;
//        descriptor->prev->size += descriptor->size + sizeof(Descriptor);
    }

    //Меняем ссылку на предыдущий и следующего за следующим
    struct Descriptor *nextByNext = (Descriptor *) ((char *) descriptor + descriptor->size + sizeof(Descriptor));
    nextByNext->prev = descriptor;
    return;
}

void Dump() {
    std::cout << endl;
    void *current_location;
    current_location = managedMemoryStart;
    struct Descriptor *currentDescriptor;

    while (current_location < lastValidAddress) {
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


    free(qwe3);
    free(qwe5);
    free(qwe4);
    free(qwe6);
    free(qwe2);
    free(qwe);
    Dump();
    struct Descriptor *a = (struct Descriptor *) (qwe2 - sizeof(Descriptor));

    Dump();

}