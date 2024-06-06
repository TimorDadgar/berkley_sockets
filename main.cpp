#include "server.hpp"

int main(int, char**) {

    Server ServerInterface{};
    ServerInterface.berkley_socket();
    ServerInterface.start_pong_game();
    ServerInterface.clean_up();

    return 0;
}