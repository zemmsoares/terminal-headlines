#include "json.hpp"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

using json = nlohmann::json;

class Request {
    public:
    Request() {}

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    void getRequests(){
    
    CURL *curl;
    CURLcode res;
    json newsJson;
    std::string readBuffer;

    /*read config json for sources and api key*/
    std::ifstream jsonFile("./data/config.json");
    json configJson;
    jsonFile >> configJson;
    jsonFile.close();
     /*grab api key and sources from /data/config.json*/
    std::string api_key = configJson["api_key"];
    std::string sources = configJson["sources"];
    std::string endpoint =
        "https://newsapi.org/v2/top-headlines?apiKey=" + api_key +
        "&sources=" + sources;

    curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Polynews/2.0");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                try {
                    newsJson = json::parse(readBuffer);
                    std::ofstream jsonFile("./data/data.json");
                    jsonFile << newsJson.dump(4);
                    jsonFile.close();
                } catch (json::parse_error& e) {
                    std::cout << "Error parsing json: " << e.what() << std::endl;
                }
      } else {
            std::cout << "CURL NOT OK\n";
      }
    }
  }
};
