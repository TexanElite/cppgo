#include <boost/program_options.hpp>
#include <iostream>

#include "ogs_client.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
    po::options_description desc("Required options");
    desc.add_options()("client-id", po::value<std::string>(),
                       "The client id associated with the user application")(
        "username", po::value<std::string>(), "The username of the account")(
        "password", po::value<std::string>(), "The password of the account");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.contains("client-id") && vm.contains("username") &&
        vm.contains("password")) {
        cppgo::ogs::OgsClient ogs(vm["client-id"].as<std::string>(),
                                  vm["username"].as<std::string>(),
                                  vm["password"].as<std::string>());
    }
}