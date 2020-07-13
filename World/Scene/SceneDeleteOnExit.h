#ifndef GAME_SCENEDELETEONEXIT_H
#define GAME_SCENEDELETEONEXIT_H
struct SceneDeleteOnExit{
    friend class MenuScene;
    friend class Scene;
protected:
    bool deleteOnExit=true;
};

#endif //GAME_SCENEDELETEONEXIT_H
