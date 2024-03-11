#ifndef RECV_STATE_H
#define RECV_STATE_H

#include <functional>

namespace printer {

struct State;

namespace recv {

void try_read(std::function<void(const State&)> cb);
void write(std::function<void(State*)> cb);

}
}

#endif