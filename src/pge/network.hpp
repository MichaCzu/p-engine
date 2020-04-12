//
#pragma once

#ifdef NETWORKER_ACTIVE
#include <SDL2/SDL_net.h>
#endif
#include <string>
#include <vector>

namespace pge {
	namespace net {

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
}