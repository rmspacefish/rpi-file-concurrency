#include <iostream>
#include <thread>

void readerFunc();
void writerFunc();

int main() {
    std::cout << "Hello World!" << std::endl;

    std::thread readerTask(readerFunc);
    std::thread writerTask(writerFunc);
    readerTask.join();
    writerTask.join();

	std::cout << "Finished." << std::endl;
}

void readerFunc() {

}

void writerFunc() {

}
