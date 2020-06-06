#include <iostream>
#include <fstream>

int main() {
    //std::cout << "Hello, World!" << std::endl;
    std::ofstream ofs("talking_log.txt");
    std::string message;
    while(message != "overi"){
        std::cin >> message;
        ofs << message << std::endl;
        message += 'i';
        std::cout << message << std::endl;
    }
    ofs.close();
    return 0;
}
