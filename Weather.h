//
// Created by zombibober on 01.10.18.
//

#ifndef GEOMICROSERVICES_WEATHER_H
#define GEOMICROSERVICES_WEATHER_H


#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_client.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;
using namespace web::http::client;

class Weather {
public:
    explicit Weather(string_t url);
    pplx::task<void> open() { return  listener.open();}
    pplx::task<void> close() { return listener.close();}
private:

    void handle_get(http_request message);
    string_t api_key;
    http_listener listener;
    std::unique_ptr<http_client> client;
};


#endif //GEOMICROSERVICES_WEATHER_H
