#include <iostream>

int main() {
    std::string message;
    for(int i = 0; i < 5; i++){
        message += 'a';
        std::cout << message << std::endl;
        std::cin >> message;
    }
    std::cout << "over" << std::endl;

    return 0;
}
