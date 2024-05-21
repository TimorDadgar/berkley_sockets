#include "server.hpp"

int main(int, char**) {
    berkley_socket();
    start_pong_game();
    return 0;
}