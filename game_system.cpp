//game_system.hpp
struct GameSystem {
    //The global variables goes here
    ...

        //game system functions
        static void init();
    static void clean();
    static void update(const float& dt);
    static void render(sf::RenderWindow& window);
};