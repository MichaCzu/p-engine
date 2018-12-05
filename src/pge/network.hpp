//
#ifndef PGE_NETWORK_HPP
#define PGE_NETWORK_HPP

#ifdef NETWORKER_ACTIVE
#include <SDL2/SDL_net.h>
#endif
#include <string>
#include <vector>

namespace pge::net {

namespace tcp {
    void open() {}
    void close() {}
    void bind() {}
    void unbind() {}
    void send() {}
    void recv() {}
    void sendV() {}
    void recvV() {}
}

namespace udp {
    void open() {}
    void close() {}
    void bind() {}
    void unbind() {}
    void send() {}
    void recv() {}
    void sendV() {}
    void recvV() {}
}

namespace http {
    void open() {}
    void close() {}
    void bind() {}
    void unbind() {}
    void send() {}
    void recv() {}
    void sendV() {}
    void recvV() {}
}
}
#endif //PGE_NETWORK_HPP
