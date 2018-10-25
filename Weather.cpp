//
// Created by zombibober on 01.10.18.
//

#include "Weather.h"
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace web::http;
using namespace web::http::client;
Weather::Weather(string_t url) : listener(url) {
    listener.support(methods::GET, std::bind(&Weather::handle_get,this,std::placeholders::_1));
   // api_key = std::getenv("API_KEY");
    if(const char * key = std::getenv("API_KEY")) {
        api_key = key;
    }else{
        ucout << "Создайте env = api_key" ;
        exit(1);
    }
    client = std::unique_ptr<http_client>(new http_client("http://api.weatherbit.io/"));
}

/*GET /v1/current/?city=<city>
response:
{
"city": "Moscow",
"unit": "celsius",
"temperature": 25
}
*/
void Weather::handle_get(http_request message) {
    std::map<string_t, string_t> get_uri = uri::split_query(message.request_uri().query());
    if (get_uri[U("city")].empty()) {
        message.reply(status_codes::BadRequest);
        return;
    }

    uri_builder builder(U("/v2.0/current"));
    builder.append_query(U("key"), api_key);
    builder.append_query(U("city"), U(get_uri[U("city")]));

    http_response response = client->request(methods::GET, builder.to_string()).get();

    if(response.status_code() != status_codes::OK){
        message.reply(status_codes::BadRequest);
        return;
    }


    json::value result_api = response.extract_json().get();
    json::value answer;
    answer["city"] = json::value::string(get_uri[U("city")]);
    answer["unit"] = json::value::string(U("celsius"));
    answer["temperature"]  = result_api["data"][0]["temp"];

    message.reply(status_codes::OK, answer);

}

