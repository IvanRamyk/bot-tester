#include <iostream>
#include <fstream>
#include <future>
#include <zconf.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    const std::string clientPath = "../../client/cmake-build-debug/testClient";

        int serverPort = 8010,
                currentBotPort = serverPort+1,
                nextBotPort = currentBotPort;//serverPort+2;

std::string firstPlayer = "../team_1/cmake-build-debug/team_1";
        std::string pipeline = "mkfifo pipeline";
    std::string commandOne = pipeline + " && ./" + clientPath + " ";
    commandOne += std::to_string(serverPort) + " " + std::to_string(currentBotPort) + " < pipeline | ";
    commandOne += firstPlayer + " > pipeline";
    std::cout << commandOne << std::endl;

    // std::string commandTwo = pipeline + "2\n ./" + clientPath + " ";
    // commandTwo += std::to_string(serverPort) + " " + std::to_string(currentBotPort) + " < pipeline1452 | ";
    // commandTwo += secondPlayer + " > pipeline1452";

    std::ofstream file1("script1.sh");
    file1 << commandOne << std::endl;
    // std::ofstream file2("script2.sh");
    // file2 << commandTwo << std::endl;
    file1.close();
    // file2.close();

  //  auto* game = new TestGame();
   // auto* interactor = new Interactor<TestGame>(game);
   std::cout << "Ready to release!\n";
    //auto fut1 = std::async (std::launch::async, system, "./script1.sh");
    system("./script1.sh");
    std::cout << "RELEASED!\n";
    usleep(50000);

    return 0;
}
