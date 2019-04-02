#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cassert>

pid_t profiler_start() {
#if PROF_ENABLE
    std::stringstream ss;
    ss << getpid();

    auto pid = fork();
    if (pid != 0) {
        return pid;
    }

    int console = open("/dev/null", O_RDWR);
    assert(console > 0);
    assert(dup2(console, STDOUT_FILENO) != -1);
    assert(dup2(console, STDERR_FILENO) != -1);
    exit(execl("/usr/bin/perf", "perf", "record", "-g", "-o", "perf.data",
               "-p", ss.str().c_str(), nullptr));
    return -1;
#else
    return 0;
#endif
}

bool profiler_stop(pid_t pid) {
#if PROF_ENABLE
    if (kill(pid, SIGINT) != 0)
        return false;
    if (waitpid(pid, nullptr, 0) != pid)
        return false;
#else
    (void)pid;
#endif
    return true;
}

