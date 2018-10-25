//
// Created by zombibober on 04.10.18.
//

#include "WeatherForecast.h"

WeatherForecast::WeatherForecast(string_t url):listener(url) {
    listener.support(methods::GET, std::bind(&WeatherForecast::handle_get,this,std::placeholders::_1));
    // api_key = std::getenv("API_KEY");
    if(const char * key = std::getenv("API_KEY")) {
        api_key = key;
    }else{
        ucout << "Создайте env = api_key" ;
        exit(1);
    }
    client = std::unique_ptr<http_client>(new http_client("http://api.weatherbit.io/"));
}

void WeatherForecast::handle_get(http_request message) {
    std::map<string_t, string_t> get_uri = uri::split_query(message.request_uri().query());
    if (get_uri[U("city")].empty()) {
        message.reply(status_codes::BadRequest);
        return;
    }
    uri_builder builder(U("v2.0/forecast/daily"));
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
    if(get_uri["dt"].empty()) {
        std::vector<json::value> vector(16);
        for(int i = 0 ; i<16; i++){
            std::vector<std::pair<string_t,json::value>> map;
            map.push_back(std::make_pair(U("datetime"),result_api["data"][i]["datetime"]));
            map.push_back(std::make_pair(U("temperature"),result_api["data"][i]["temp"]));
            vector[i] = answer.object(map);
        }
        answer["data"] = answer.array(vector);
        message.reply(status_codes::OK, answer);
    }else{
        for(int i=0;i<16;i++)
            if(result_api["data"][i]["datetime"].as_string() == get_uri["dt"] ) {
                answer["temperature"] = result_api["data"][i]["temp"];
                message.reply(status_codes::OK,answer);
            }
            message.reply(status_codes::BadRequest);
    }
}
