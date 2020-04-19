//
// Created by light on 18.04.2020.
//

#include <stdlib.h>
#include "pdp.h"

void do_halt() {
    trace(ERROR, "--- HALTED ---");
    exit(0);
}
void do_add() {
}
void do_mov() {
}
void do_nothing() {
}

Command cmd[] = {
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0060000, "add", do_add},
        {0177777, 0000000, "halt", do_halt},
        {0177777, 0000001, "nothing", do_nothing}, //  резервная команда
        {0000000, 0000000, "unknown", do_nothing} // команда конца списка, есть пока не доделан код команд
};
