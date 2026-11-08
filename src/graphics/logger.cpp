#include "utl/logger.hpp"

Logger::Logger(int level, bool errors) {
    this->level = level;
    this->errors = errors;

    std::cout << "┌────────────────Logger starting...────────────────]" << "\n";

}

void Logger::log(std::string msg, bool progress, int lv) {
    if (lv > level) {return;}

    if (!on) {return;}
    
    switch (lv)
    {
    case 0:
        if (progress) {std::cout << main_space << "[LOGGER]" << PURPLE << "[MAIN]" << RESET << YELLOW << "[STARTING...] " << RESET << msg << "\n";}
        else {std::cout << main_space << "[LOGGER]" << PURPLE << "[MAIN]" << RESET << GREEN << "[COMPLETE] " << RESET << msg << "\n";}
        break;

    case 1:
        if (progress) {std::cout << lv1_space << "[LOGGER]" << DARK_BLUE << "[LV1]" << RESET << YELLOW << "[STARTING...] " << RESET << msg << "\n";}
        else {std::cout << lv1_space << "[LOGGER]" << DARK_BLUE << "[LV1]" << RESET << GREEN << "[COMPLETE] " << RESET << msg << "\n";}
        break;

    case 2:
        if (progress) {std::cout << lv2_space << "[LOGGER]" << BLUE << "[LV2]" << RESET << YELLOW << "[STARTING...] " << RESET << msg << "\n";}
        else {std::cout << lv2_space << "[LOGGER]" << BLUE << "[LV2]" << RESET << GREEN << "[COMPLETE] " << RESET << msg << "\n";}
        break;

    case 3:
        if (progress) {std::cout << lv3_space << "[LOGGER]" << LIGHT_BLUE << "[LV3]" << RESET << YELLOW << "[STARTING...] " << RESET << msg << "\n";}
        else {std::cout << lv3_space << "[LOGGER]" << LIGHT_BLUE << "[LV3]" << RESET << GREEN << "[COMPLETE] " << RESET << msg << "\n";}
        break;

    default:
        if (progress) {std::cout << "[LOGGER]" << LIGHT_BLUE << "[LV3]" << RESET << YELLOW << "[STARTING...] " << RESET << msg << "\n";}
        else {std::cout << "[LOGGER]" << LIGHT_BLUE << "[LV3]" << RESET << GREEN << "[COMPLETE] " << RESET << msg << "\n";}
        break;
    }


}

void Logger::log_war(std::string msg) {
    if (!on) {return;}
    
    std::cout << "[LOGGER] " << ORANGE << "[WARNING]" << RESET << msg << "\n";

}

void Logger::log_err(std::string msg) {
    if (!on) {return;}
    if (!errors) {return;}

    std::cout << "[LOGGER] " << RED << "[ERROR]" << RESET << msg << "\n";
    
}
