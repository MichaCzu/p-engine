//zapisywanie i wczytywanie danych z pliku. ( niby zbędne ale dla czytelności potem )
#ifndef PGE_THREAD_HPP
#define PGE_THREAD_HPP

#include <string>
#include <vector>

namespace pge {
namespace thread {

    enum en_priority {
        PRIORITY_LOW = 0b0001,
        PRIORITY_NORMAL = 0b0010,
        PRIORITY_HIGH = 0b0100,
        PRIORITY_REAL = 0b1000,
    };

    void create(void* function, en_priority priority, void* parameters);
    void stop();

    void* get_return();
    void* wait_return();
}
}

#endif //PGE_THREAD_HPP
