#include "rpc/server.h"
#include "rpc/this_session.h"

#include <iostream>

int main() {
    rpc::server server("127.0.0.1", rpc::constants::DEFAULT_PORT);
    auto socket_observer =
        std::make_shared<rpc::observer<rpc::session_id_t, rpc::socket_status>>(
            [](rpc::session_id_t id, rpc::socket_status status) {
                std::cout << "socket id '" << id << "' status '"
                          << (status == rpc::socket_status::connected
                                  ? "connected"
                                  : "disconnected")
                          << "'\n";
            });

    server.socket_connection().subscribe(socket_observer);

    server.bind("add", [](int a, int b) {
        int sum = 0;
        for (size_t i = 0; i < 10000000000; ++i) {
            sum += a + b;
        }
        return sum;
    });
    server.run();
    return 0;
}
