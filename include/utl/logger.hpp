#pragma once
#include <string>
#include <iostream>
#include <format>

//configs
#define LEVEL_OF_VERBOSITY 2
#define IS_SHOW_ERRORS_ENABLED true 

#define RED "\033[31m" // error
#define YELLOW "\033[33m" // starting
#define GREEN "\033[32m" // completed
#define LIGHT_BLUE  "\033[38;5;117m" // lv3
#define BLUE        "\033[38;5;27m" // lv2
#define DARK_BLUE   "\033[38;5;19m" // lv1
#define PURPLE "\033[38;5;129m" // main/lv 0
#define ORANGE "\033[38;5;208m" // warning
#define RESET "\033[0m" // reset color





struct Logger {

    /*
    3 params
    lv - level of importance/main
    type - Progress/complete/war/err

    lv0 - main
    lv1 - class intance
    lv2 - class func/func 
    lv3 repeated funcs
    
    */
    
    std::string main_space = "├";
    std::string lv1_space  = "├───";
    std::string lv2_space  = "├──────";
    std::string lv3_space  = "├─────────";

    int started_jobs_count;

    bool on = true;
    int level;
    bool errors;

    Logger(int level, bool errors);

    void log(std::string msg, bool progress, int lv);

    void log_war(std::string msg);
    void log_err(std::string msg);

};

// global instance
inline Logger lg{LEVEL_OF_VERBOSITY, IS_SHOW_ERRORS_ENABLED};