#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H
#include <string>
class Utility final{
    public:
        static std::string readFile(const char* path);
    private:
        Utility()=delete;
        ~Utility()=delete;
};


#endif //GAME_UTILITY_H
