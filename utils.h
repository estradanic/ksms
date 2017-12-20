#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

inline std::string execute(const char* cmd){
   std::array<char, 128> buffer;
   std::string result;
   std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
   if (!pipe) throw std::runtime_error("popen() failed!");
   while (!feof(pipe.get())) {
       if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
           result += buffer.data();
   }
   return result;
}

#endif // UTILS_H
