// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../inc/base_functions.h"
#include "../inc/defined_vars.h"
#include "../inc/options_parser.h"
#include "../inc/openssl_communication.h"
#include "../inc/utils.h"


int main(int argc, char *argv[]) {
    // parse arguments
    command_line_options config;
    config.parse(argc, argv);
    int port = config.get_port();
    std::string bin = config.get_bin();

    if (port == -1) {
        cout << "No port specified. Picking the default one" << endl;
        port = DEFAULT_PORT;  // Setting a default port
    }

    if (bin == "") {
        cout << "No path to binary file specified. Use --bin option" << endl;
        return FAIL;
    }

    std::string memory_in_bytes = config.get_memory_in_bytes();
    std::string cpu_shares = config.get_cpu_shares();
    std::string cpu_period = config.get_cpu_period();
    std::string cpu_quota = config.get_cpu_quota();
    std::string device_read_bps = config.get_device_read_bps();
    std::string device_write_bps = config.get_device_write_bps();

    std::string mount = config.get_mount();
    std::string volume = config.get_volume();

    std::vector<std::string> options = {
        memory_in_bytes,
        cpu_shares,
        cpu_period,
        cpu_quota,
        device_read_bps,
        device_write_bps,
        mount,
        volume
    };

    std::vector<std::string> errors = {
        WRONG_UNIT,
        TYPE_ERROR,
        WRONG_ARGUMENTS
    };

    std::vector<std::string> error_exist = intersection(options, errors);

    if (!error_exist.empty()) {
        return FAIL;
    }

    std::string command = bin;
    command += " --memory-in-bytes " + memory_in_bytes;
    command += " --cpu-shares " + cpu_shares;
    command += " --cpu-period " + cpu_period;
    command += " --cpu-quota " + cpu_quota;
    command += " --device-read-bps " + device_read_bps;
    command += " --device-write-bps " + device_write_bps;
    command += mount;
    command += volume;

    // std::cout << command << std::endl;

    // we need to create output object to receive the result
    std::string command_output_buf;

    run_encrypted_client(port, command, command_output_buf);

    // std::cout << "command_output_buf " << command_output_buf << std::endl;

    return 0;
}