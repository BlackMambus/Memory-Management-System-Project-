#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <cstring>

class MemoryManager {
private:
    std::unordered_map<void*, size_t> allocations;
    size_t totalAllocated = 0;
    size_t totalFreed = 0;

public:
    void* allocate(size_t size) {
        void* ptr = std::malloc(size);
        if (ptr) {
            allocations[ptr] = size;
            totalAllocated += size;
            std::cout << "[ALLOC] " << size << " bytes at " << ptr << "\n";
        }
        return ptr;
    }

    void deallocate(void* ptr) {
        auto it = allocations.find(ptr);
        if (it != allocations.end()) {
            size_t size = it->second;
            totalFreed += size;
            std::free(ptr);
            allocations.erase(it);
            std::cout << "[FREE ] " << size << " bytes from " << ptr << "\n";
        } else {
            std::cerr << "[ERROR] Attempt to free untracked memory at " << ptr << "\n";
        }
    }

    void reportLeaks() const {
        std::cout << "\n📊 Memory Report:\n";
        std::cout << "  Total Allocated: " << totalAllocated << " bytes\n";
        std::cout << "  Total Freed:     " << totalFreed << " bytes\n";
        if (!allocations.empty()) {
            std::cout << "  🔥 Memory Leaks Detected:\n";
            for (const auto& pair : allocations) {
                std::cout << "    - Leak of " << pair.second << " bytes at " << pair.first << "\n";
            }
        } else {
            std::cout << "  ✅ No memory leaks detected.\n";
        }
    }

    ~MemoryManager() {
        reportLeaks();
        // Optionally free remaining allocations
        for (const auto& pair : allocations) {
            std::free(pair.first);
        }
    }
};

// Global instance
MemoryManager memManager;

// Wrappers for easier use
void* mm_alloc(size_t size) {
    return memManager.allocate(size);
}

void mm_free(void* ptr) {
    memManager.deallocate(ptr);
}
