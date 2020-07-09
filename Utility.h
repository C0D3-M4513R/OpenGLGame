#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H

class Utility final{
    public:
        static char * readFile(const char* path,char* out);
    private:
        Utility()=delete;
        ~Utility()=delete;
};


#endif //GAME_UTILITY_H
