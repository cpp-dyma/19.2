
#include <iostream>
#include <vector>
#include <future>
#include <fstream>
#include <mutex>
#include <chrono>
#include <thread>
#include <string_view>

class Fraction {

    public:
    Fraction() : numerator(0), denominator(1) {}
    Fraction(int num, int denom) : numerator(num), denominator(denom) {}

    void display() const {
        std::cout<<std::this_thread::get_id()<<" - "<< numerator << "/" << denominator<<"\n";
    }

    private:
    int numerator;
    int denominator;
};

constexpr int totalFileSize = 1000000;
constexpr int chunkSize = 100000;
std::vector<char> fileData(totalFileSize);

int downloadChunk(int startIndex, int size = chunkSize) {
    std::thread::id id {std::this_thread::get_id()};
    // Simulate downloading a chunk of data from the internet
    std::this_thread::sleep_for(std::chrono::milliseconds(size/100));
    
    for (int i = 0; i < chunkSize; ++i) {
        fileData[startIndex + i] = 'X'; // Mark downloaded part
    }

    std::cout <<id<< " - Downloaded block: " << startIndex <<"-"<< startIndex+chunkSize << std::endl;

    return (startIndex+chunkSize);
}

void downloadFile(std::string_view url){
    std::cout<<"Downloading from.... : "<<url<<"\n";
    for(auto i{0} ; i<totalFileSize ; i+=chunkSize){
        downloadChunk(i);
    }
}

void displayTime(){
    std::thread::id id {std::this_thread::get_id()};
    while(true){
        // Obtenir l'heure actuelle
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto time_str = std::ctime(&now_time);
        std::cout <<id<<" - Heure actuelle : " << time_str;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() 
{
    std::thread thread_clock(displayTime);
    //std::thread thread_downloading(downloadFile, std::string_view{"http://dyma.fr"});

    std::thread thread_downloading([](std::string_view url){
        std::cout<<"Downloading from.... : "<<url<<"\n";
        for(auto i{0} ; i<totalFileSize ; i+=chunkSize){
            downloadChunk(i);
        }
    }, std::string_view{"http://dyma.fr"});

    Fraction f(4,9);

    std::thread thread_fraction(&Fraction::display, &f);
    
    thread_clock.detach();
    thread_fraction.join();
    thread_downloading.join();
    return 0;
}