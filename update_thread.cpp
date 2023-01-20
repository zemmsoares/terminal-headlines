#include <thread>

void update_thread() {
    Request request;
    while (true) {
        request.getRequests();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
