#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "utils/log.hpp"


std::shared_ptr<spdlog::logger> Log::logger;

void Log::init(){

   spdlog::set_pattern(pattern);
   logger = spdlog::stdout_color_mt("console");
   logger->set_level(spdlog::level::debug);

}

