#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H


#include "Face.h"

class Player {
    public:
        /**
         * Gets the Player Object. If no Player Object exists, make one.
         *
         * @param object A Face object, which will represent the Player
         * @return A non-NULL pointer to the Player Object
         */
        static Player* getPlayer(Face* object){
            if(instance!=nullptr) [[likely]] return instance;
            else return new Player(object);
        }
        /**
        * Gets the Player Object. If no Player Object exists, make one.
        *
        * @param object A Face object, which will represent the Player
        * @return A nullable pointer to the Player Object
        */
        static Player* getPlayer(){
            return instance;
        }

        void Draw(){ return object->Draw();}

        void moveX(float amount) {return object->moveX(amount);};
        void moveY(float amount) {return object->moveY(amount);};
        void moveZ(float amount) {return object->moveZ(amount);};

        //amount is in radians. It is easier to work with
        void rotate(glm::vec3 amount){return object->rotate(amount);};
        void scale(glm::vec3 amount){return object->scale(amount);};
    private:
        inline static Player* instance=nullptr;
        Face* object;

        Player(Face* object):object(object){instance=this;};
        ~Player(){delete object;};
        Player(const Player&)= delete;
        Player& operator=(const Player&)= delete;
};


#endif //GAME_PLAYER_H
