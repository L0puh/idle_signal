#ifndef LOG_H
#define LOG_H 

#include <memory>
#include <spdlog/logger.h>

class Log {

   const std::string pattern = "%^[%T] %n%$: %v";
   static std::shared_ptr<spdlog::logger> logger;

   protected:
      static Log* instance;   
      Log() {}
   public:
      static Log* get_instance() {
         if (instance == NULL) instance = new Log();
         return instance;
      }
   public:
      void init();
      static std::shared_ptr<spdlog::logger> get_logger() { return logger; }

};


#endif 
