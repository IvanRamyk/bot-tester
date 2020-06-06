#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;
condition_variable cv;

int value;

void read_value() {
    cin >> value;
    cv.notify_one();
}

int main() {
    cout << "Please enter the input: ";
    thread th(read_value);

    mutex mtx;
    unique_lock<mutex> lck(mtx);
    while (cv.wait_for(lck, chrono::seconds(2)) == cv_status::timeout) {
        cout << "\nTime-Out: 2 second:";
        cout << "\nPlease enter the input:";
    }
    cout << "You entered: " << value << '\n';

    th.join();

    return 0;
}