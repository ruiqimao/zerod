#ifndef CST816S_H
#define CST816S_H

namespace display {
namespace cst816s {

void init();
bool ready();
bool read(int &x, int &y);

}
}

#endif