#include <iostream>
#include <cstdlib>
#include <chrono>

#define PROGRAM_NAME "pstime"
#define EXIT exit(0)

#define DIVISOR 1000000.0

class Time {
public:
    auto duration(std::string cmd)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "############Command##################" << std::endl;
        std::system(cmd.c_str());
        std::cout << "############Output##################" << std::endl;
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        return duration;
    }

    void print_usage(char mode = 'q')
    {
        std::cout << "Usage:: " << PROGRAM_NAME << " [shell -c] [command]" << std::endl;
        std::cout << "Example:: pstime -c ls" << std::endl;
        std::cout << "\n'-c' flag is used for running command in cmd" << std::endl;
        std::cout << "By default shell is powershell" << std::endl;
        if (mode == 'q') EXIT;
    }


    int binary_search(int len, char *args[], std::string key)
    {
        int s = 1;
        int e = len-1;

        int mid {};

        while (s <= e) {
            mid = (s+e)/2;
            if (args[mid] == key) return mid;
            else if (args[mid] > key) e = mid-1;
            else s = mid+1;
        }

        return -1;
    }

    void  control_errors(int argc, char* args[])
    {
        if (argc == 2 && ( (strcmp(args[1], "-c")) == 0) ) {
            std::cout << "error: not enough arguments!!" << std::endl;
            EXIT;
        }

        if (argc > 3) {
            std::cout << "error: only 3 arguments allowed" << std::endl;
        }

        if ((strcmp(args[1], "-h")) == 0) print_usage();
    }


    std::string get_shell(int argc, char* args[])
    {
        int cmd = binary_search(argc, args, "-c");

        if (cmd != -1) return "cmd";
        else return "powershell";
    }

    std::string get_cmd(size_t argc, char* args[], std::string shell)
    {
        int pos = binary_search(argc, args, "-p");
        std::string cmd;

        for (int i = 1; i < argc; i++) {
            if (i == pos) continue;
            cmd = args[i];
        }

        if (shell == "cmd") {
            return cmd;
        }

        std::string actual_cmd = shell;
        actual_cmd += " ";
        actual_cmd += "-Command ";
        actual_cmd += cmd;

        return actual_cmd;
    }
};

int main(int argc, char* argv[])
{

    Time time;
    if (argc < 2) time.print_usage();

    time.control_errors(argc, argv);

    std::string shell = time.get_shell(argc, argv);
    std::string cmd = time.get_cmd(argc, argv, shell);

    auto duration = time.duration(cmd);
    double duration_in_seconds = (duration.count()/DIVISOR);

    std::cout << "Shell: " << shell << std::endl;
    std::cout << "Command: " << cmd << std::endl;

    std::cout << std::endl;

    std::cout << "Time taken (in seconds): " << duration_in_seconds << std::endl;
    std::cout << "Time taken (in milliseconds): " << duration.count()/1000.0 << std::endl;
    std::cout << "Time taken (in microseconds): " << duration.count() << std::endl;

    return 0; 
}
