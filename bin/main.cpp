#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <winsock2.h>
#include <windows.h>
#include "cpr/cpr.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// PABEDA
void help(){
    std::cout<< "как пользоваться WeatherFrst";
    std::cout<< "Найдите или создайте файл config.txt";
    std::cout<< "В первой строке конфига введите название города";
    std::cout<< "Пример ввода city=Moscow";
    std::cout<< "Дальше введите(через запятую) частоту обновления starset=";
    std::cout<< "Частота обновления имеет значение по умолчанию";
    std::cout<< "Дальше введите нужное количество дней days=";
    std::cout<< "имеет значение по умолчанию days=16";
    std::cout<< "следующие города вводить через точку с запятой ;";
}
std::string get_weather(int code){
    switch(code){
        case 0: return "Clear sky       "; //
        case 1: return "Mainly clear    "; //
        case 2: return "Mainly clear    "; //
        case 3: return "Mainly clear    "; //
        case 45: return "Fog             "; //
        case 48: return "Fog             "; //  
        case 51: return "Drizzle         "; //
        case 53: return "Drizzle         "; //
        case 55: return "Drizzle         "; //
        case 56: return "Freezing Drizzle"; //
        case 57: return "Freezing Drizzle"; //
        case 61: return "Rain            "; //
        case 63: return "Rain            "; //
        case 65: return "Rain            "; //
        case 66: return "Freezing Rain   ";//
        case 67: return "Freezing Rain   ";//
        case 71: return "Snow            "; //
        case 73: return "Snow            "; //
        case 75: return "Snow            "; //
        case 77: return "Snow grains     "; //
        case 80: return "Rain showers    "; //
        case 81: return "Rain showers    ";//
        case 82: return "Rain showers    ";//
        case 85: return "Snow showers    "; //
        case 86: return "Snow showers    "; //
        case 95: return "Thunderstorm    "; //
        case 96: return "Thunderstorm    "; //
        case 99: return "Thunderstorm    "; //
        default: return "Unknown         "; //
    }
    return "Unknown         "; // 
}
std::vector<std::string> weather_pic(std::string weather){ // 5 высота,  10 длина
    std::vector<std::string> ret;
    if (weather == "Mainly clear    " || weather == "Clear sky       "){
        ret.push_back("  \\ . /  ");
        ret.push_back(" - .-. - ");
        ret.push_back("- (   ) -");
        ret.push_back(" . `-' . ");
        ret.push_back("  / ' \\  ");
    } else if (weather == "Fog             ")
    {
        ret.push_back("_ - _ - _");
        ret.push_back("_ - _ - _");
        ret.push_back("_ - _ - _");
        ret.push_back("_ - _ - _");
        ret.push_back("_ - _ - _");
    }
    else if (weather == "Thunderstorm    ")
    {
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back("⚡⚡⚡  ");
        ret.push_back("⚡⚡⚡  ");
    }
    else if (weather == "Drizzle         ")
    {
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(",',',',' ");
        ret.push_back(",',',',' ");
    }
    else if (weather == "Freezing Drizzle")
    {
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(",',',',' ");
        ret.push_back("* * * * *");
    }
    else if (weather == "Rain            ")
    {
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(" ʻ ʻ ʻ   ");
        ret.push_back(" ʻ ʻ ʻ   ");
    }
    else if (weather == "Freezing Rain   ")
    {
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(" * ʻ * ʻ ");
        ret.push_back(" ʻ * ʻ   ");
    }else if (weather == "Snow            "  || weather == "Snow grains     "){
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(" * * * * ");
        ret.push_back("* * * *  ");
    } else if (weather == "Snow showers    "){
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back("* * * * *");
        ret.push_back("* * * * *");

    }else if (weather == "Rain showers    "){
        ret.push_back("    .-.  ");
        ret.push_back("   (   ).");
        ret.push_back(" (___(__)");
        ret.push_back(" ',',',',");
        ret.push_back(",',',',',");
    }else{
        ret.push_back("    .-.  ");
        ret.push_back("    __)  ");
        ret.push_back("   (     ");
        ret.push_back("   `-'   "); 
        ret.push_back("    •    ");
    }
    return ret;
}
struct global_vars{
    std::string api;
    bool run_status = true;
    int _frequency;
    int _days;
    std::vector<std::string> _citys;
    int _hereWeAre = 0; // непонятно
};
struct city{
    std::string _city;
    std::vector<int> _weather;
    std::vector<std::string> _days;
    std::vector<std::string> _temperature_min;
    std::vector<std::string> _temperature_max;
};
std::pair<double,double> parse_api_ninja_json(std::string govno){
    std::string data = govno.substr(1, govno.length()-2);
    json j = json::parse(data);
    double lt = j["latitude"];
    double lg = j["longitude"];
    return std::make_pair(lt,lg);
}
city parse_open_meteo_json(std::string data, std::string city_name){
    json j = json::parse(data);
   city city;
   city._city = city_name;
   for (int i = 0; i < 16; i++){
        city._weather.push_back(j["daily"]["weather_code"][i]);
        city._days.push_back(j["daily"]["time"][i]);
        std::string temp_min = std::to_string(j["daily"]["temperature_2m_min"][i].get<float>());
        city._temperature_min.push_back(temp_min.substr(0,temp_min.find('.')));
        std::string temp_max = std::to_string(j["daily"]["temperature_2m_max"][i].get<float>());
        city._temperature_max.push_back(temp_max.substr(0, temp_max.find('.')));
   }
   return city;
}
void droo(std::vector<std::string> out){
    for (int i = 0; i < out.size(); i++){
        std::cout<<out[i];
        std::cout<<"\n";
    }
}
void otrisovschik(city c, int days){
    std::cout<<"\033[2J\033[1;1H"; // очистка
    std::cout<< "Прогноз погоды в ";
    std::cout<<c._city;
    std::cout<<"\n\n";
    std::vector<std::string> out(10);
    for (int i = 0; i < 10; i++){
        out[i] = "";
    }
    for (int i = 1; i < 17; i++){
        if (i > days){
            break;
        }
        if (i % 4 == 1){
            out[0] += "╔══════════════════════════════╦";
        }else if (i % 4 == 0){
            out[0] += "╦══════════════════════════════╗";
        }else{
            out[0] += "╦══════════════════════════════╦";
        }
        out[1] += "║ Data : ";
        out[1] += c._days[i-1];
        out[1] += "            ║";
        out[2] += "║ Weather : ";
        out[2] += get_weather(c._weather[i - 1]);
        out[2] += "   ║";
        out[3] += "║ Temperature : ";
        out[3] += c._temperature_min[i-1];
        out[3] += "(";
        out[3] += c._temperature_max[i-1];
        out[3] += ") ℃";
        out[3].insert(out[3].end(), 6 - c._temperature_max[i - 1].size() - c._temperature_min[i - 1].size(), ' ');
        out[3] += "     ║";
        std::vector<std::string> pic = weather_pic(get_weather(c._weather[i-1]));
        for (int j = 0; j < 5; j++){
            out[4+j] += "║       ";
            out[4+j] += pic[j];
            out[4+j] += "              ║";
        }
        if (i % 4 == 1){
            out[9] += "╚══════════════════════════════╩";
        }else if (i % 4 == 0){
            out[9] += "╩══════════════════════════════╝";
        }else{
            out[9] += "╩══════════════════════════════╩";
        }
        if (i % 4 == 0){
            droo(out);
            for (int i = 0; i < 10; i++){
                out[i] = "";
            }
        }  
    }
    droo(out);
}
int cityrun(std::string name, int days, std::string api){
    std::string city_name = name;
    cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                    cpr::Parameters{{"name", city_name}},
                    cpr::Header{{"X-Api-Key", api}});
    if (r.status_code != 200 || r.text == "[]"){
        std::cout<<"\033[2J\033[1;1H"; // очистка
        std::cout<<"ошибка api-ninjas";
        return 1;
    }
    std::pair<double, double> cords = parse_api_ninja_json(r.text);
    cpr::Response open_meteo_json = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                      cpr::Parameters{{"latitude", std::to_string(float(cords.first))}, {"longitude",std::to_string(float(cords.second))}, {"forecast_days", "16"},
                                    {"daily", "temperature_2m_max"},
                                {"daily", "temperature_2m_min"},
                            {"daily", "weather_code"}});
    std::cout<<open_meteo_json.text;
    city _city = parse_open_meteo_json(open_meteo_json.text, name);
    otrisovschik(_city, days);
    return 0;
}

global_vars parse_config(){
    std::ifstream config("./config.json");
    global_vars run;
    if (!config.is_open()){
        std::cout<<"\nнет файла конфигурации";
        run.run_status = false;
        return run;
    }
    json j = json::parse(config);
    run.api = j["api_key"];
    run._days = j["days"];
    run._frequency = j["frequency"];
    run._citys = j["city"];
    config.close();
    return run;
}
void knopki(global_vars& var){
    while (var.run_status) {
        if (GetAsyncKeyState(0x4E) < 0) {
            var._hereWeAre = (var._hereWeAre + 1) % var._citys.size();
            cityrun(var._citys[var._hereWeAre], var._days, var.api);
        }
        if (GetAsyncKeyState(0x50) < 0) {
            var._hereWeAre = (var._hereWeAre - 1 + var._citys.size()) % var._citys.size();
            cityrun(var._citys[var._hereWeAre], var._days, var.api);
        }
        if (GetAsyncKeyState(0xBB) < 0) {
            var._days = std::min(var._days + 1, 16);
            cityrun(var._citys[var._hereWeAre], var._days, var.api);
        }
        if (GetAsyncKeyState(0xBD) < 0) {
            var._days = std::max(var._days - 1, 1);
            cityrun(var._citys[var._hereWeAre], var._days, var.api);
        }
        if (GetAsyncKeyState(0x1B) < 0) {
            var.run_status = false;
            return;
        }
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);

    global_vars var;
    var = parse_config();

    std::atomic<bool> stop_view(false);
    std::condition_variable cv;
    std::mutex cv_m;
    std::thread keys([&](){
        knopki(var);
    });
    std::thread view([&]() {
        std::unique_lock<std::mutex> lk(cv_m);
        while (var.run_status) {
            cityrun(var._citys[var._hereWeAre],var._days,var.api);
            cv.wait_for(lk, std::chrono::seconds(var._frequency));
            if (stop_view.load()) {
                return;
            }
        }
    });
    keys.join();
    stop_view.store(true);
    cv.notify_one();
    view.join();
    return 0;
}