#include <iostream>
#include "Weather.h"
#include "WeatherForecast.h"
//std::unique_ptr<Weather> weather;
//std::unique_ptr<WeatherForecast> forecast;
int main() {
    string_t port(U("8080"));
    if(const char * s = std::getenv("LISTEN_PORT")){
        port = s;
    }
    std::unique_ptr<Weather> weather = std::unique_ptr<Weather>(new Weather("http://localhost:" + port + "/v1/current/"));
    std::unique_ptr<WeatherForecast> forecast = std::unique_ptr<WeatherForecast>(new WeatherForecast("http://localhost:" + port + "/v1/forecast/"));
    forecast->open().wait();
    weather->open().wait();
    std::cout << "Ввведите Enter: ";
    std::cin.get();
    weather->close().wait();
    forecast->close().wait();
    return 0;
}