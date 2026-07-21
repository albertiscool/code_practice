#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 5

int main() {
    int pipe_fds[2]; // Simulated hardware pipe: [0] = Read end, [1] = Write end
    
    // 1. Create a Linux pipe
    if (pipe(pipe_fds) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    printf("========================================================\n");
    printf("--- Day 34: Linux Epoll & File Descriptor Event Loop ---\n");
    printf("========================================================\n\n");

    // 2. Create an Epoll instance inside Linux Kernel
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Epoll creation failed");
        exit(EXIT_FAILURE);
    }

    // 3. Register the Read FD of our simulated device into Epoll
    struct epoll_event ev;
    ev.events = EPOLLIN; // Monitor for incoming data (Readable event)
    ev.data.fd = pipe_fds[0];

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fds[0], &ev) == -1) {
        perror("Epoll control failed");
        exit(EXIT_FAILURE);
    }

    printf("[Epoll System] Successfully registered Simulated Sensor FD (%d) into Epoll Kernel Tree.\n", pipe_fds[0]);

    // 4. Simulate a hardware sensor writing data in background
    printf("\n[Hardware Simulation] Writing sensor byte stream into hardware pipe...\n");
    const char *sensor_payload = "TELEMETRY_TEMP_75C";
    write(pipe_fds[1], sensor_payload, strlen(sensor_payload));

    // 5. Epoll Wait: Thread sleeps efficiently until Linux Kernel fires a hardware event
    struct epoll_event events[MAX_EVENTS];
    printf("[Epoll System] Entering epoll_wait(). Thread sleeping until event triggers...\n");
    
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 2000); // 2000ms timeout limit

    if (nfds == -1) {
        perror("epoll_wait failed");
        exit(EXIT_FAILURE);
    } else if (nfds == 0) {
        printf("[Epoll System] Timeout reached! No data arrived.\n");
    } else {
        printf("\n>>> [EPOLL EVENT TRIGGERED] Kernel notified %d active File Descriptor(s)! <<<\n", nfds);
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == pipe_fds[0]) {
                char read_buffer[64] = {0};
                read(pipe_fds[0], read_buffer, sizeof(read_buffer) - 1);
                printf("  ↳ [Read Handler] Captured Payload from FD %d: \"%s\"\n", events[i].data.fd, read_buffer);
            }
        }
    }

    // Clean up file descriptors
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    close(epoll_fd);

    printf("\n[Main System] Epoll lifecycle closed cleanly.\n");
    return 0;
}