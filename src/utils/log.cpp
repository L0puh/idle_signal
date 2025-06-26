#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "utils/log.hpp"


std::shared_ptr<spdlog::logger> Log::logger;

void Log::init(){
   logger = spdlog::stdout_color_mt("console");
   logger->set_pattern(pattern);
   logger->set_level(level);

}

