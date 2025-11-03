#include <sstream>
#include <SFML/Graphics.hpp>

const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::A,   // Player1 UP
    sf::Keyboard::Z,   // Player1 Down
    sf::Keyboard::Up,  // Player2 UP
    sf::Keyboard::Down // Player2 Down
};

//Parameters
const sf::Vector2f paddle_size(25.f, 100.f);
const float ball_radius = 10.f;
const int game_width = 800;
const int game_height = 600;
const float paddle_speed = 400.f;
const float paddle_offset_wall = 10.f;
const float time_step = 0.017f; //60 fps

sf::Vector2f ball_velocity;
bool is_player_serving = true;
const float initial_velocity_x = 100.f;
const float initial_velocity_y = 60.f;
const float velocity_multiplier = 1.1f;

int score[2] = {0,0};

//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];
sf::Text text;    
sf::Font font;


void reset(){
    ball_velocity = { (is_player_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };

    // reset paddle position
    paddles[0].setPosition(paddle_offset_wall + paddle_size.x / 2.f, game_height / 2.f);
    paddles[1].setPosition(game_width - paddle_offset_wall - paddle_size.x /2.f, game_height / 2.f);
    // reset Ball Position in the centre of the left paddle
    ball.setPosition(paddle_offset_wall + paddle_size.x + ball_radius*2.f,game_height / 2.f);

     // Update Score Text
    std::stringstream sstr;
    sstr << score[0] << " - " << score[1];
    text.setString(sstr.str()) ;
    // Keep Score Text Centered
    text.setPosition((game_width * .5f) - (text.getLocalBounds().width * .5f), 0);
}

void init() {
    // Set size and origin of paddles
    for (sf::RectangleShape &p : paddles) {
        p.setSize(paddle_size);
        p.setOrigin(paddle_size / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ball_radius);
    ball.setOrigin(ball_radius,ball_radius); //Should be half the ball width and height
    
     // Load font-face from res dir
    font.loadFromFile("resources/fonts/RobotoMono-Regular.ttf");
    // Set text element to use font
    text.setFont(font);
    // set the character size to 24 pixels
    text.setCharacterSize(24);
    text.setColor(sf::Color(255,255,255));
    reset();
}


void update(float dt) {


    // handle paddle movement
    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0]) &&
    paddles[0].getPosition().y > paddle_size.y/2.f ) {
        direction--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1]) &&
    paddles[0].getPosition().y < game_height - paddle_size.y/2.f ) {
        direction++;
    }

    paddles[0].move(sf::Vector2f(0.f, direction * paddle_speed * dt));

    // handle paddle movement
    direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[2]) && 
        paddles[1].getPosition().y > paddle_size.y/2.f) {
        direction--;
    }
    if (sf::Keyboard::isKeyPressed(controls[3]) &&
    paddles[1].getPosition().y < game_height - paddle_size.y/2.f) {
        direction++;
    }

    paddles[1].move(sf::Vector2f(0.f, direction * paddle_speed * dt));
    ball.move(ball_velocity * dt);

    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > game_height) { //bottom wall
        // bottom wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, -10.f));
    } else if (by < 0) { //top wall
        // top wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, 10.f));
    } 
    else if (bx > game_width) {
        // right wall
        score[0]++;
        reset();
    } else if (bx < 0) {
        // left wall
        score[1]++;
        reset();
    }
    else if (
	    //ball is inline or behind paddle AND
	    bx < paddle_size.x + paddle_offset_wall && 
	     //ball is below top edge of paddle AND
	    by > paddles[0].getPosition().y - (paddle_size.y * 0.5) &&
	    //ball is above bottom edge of paddle
	    by < paddles[0].getPosition().y + (paddle_size.y * 0.5)){
            //bounces off left paddle
        ball_velocity.x *= -velocity_multiplier;
        ball_velocity.y *= velocity_multiplier;
        ball.move(sf::Vector2f(+10.f, 0));
    } else if (
        //ball is inline or behind paddle AND
	    bx > game_width - paddle_size.x - paddle_offset_wall  && 
	     //ball is below top edge of paddle AND
	    by > paddles[1].getPosition().y - (paddle_size.y * 0.5) &&
	    //ball is above bottom edge of paddle
	    by < paddles[1].getPosition().y + (paddle_size.y * 0.5)) {
        // bounce off right paddle
        ball_velocity.x *= -velocity_multiplier;
        ball_velocity.y *= velocity_multiplier;
        ball.move(sf::Vector2f(-10.f, 0));
    }
}



void render(sf::RenderWindow &window) {
	// Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text);
}

int main () {
    sf::RenderWindow window(sf::VideoMode({game_width, game_height}), "PONG");
    window.setVerticalSyncEnabled(true);	
    init();
    //initialise and load
	while(window.isOpen()){
        // Reset clock, recalculate deltatime
        static sf::Clock clock;
        float dt = clock.restart().asSeconds();
        // check and consume events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }
        // Quit Via ESC Key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

		window.clear();
		update(dt);
		render(window);
        //wait for the time_step to finish before displaying the next frame.
        sf::sleep(sf::seconds(time_step));
		//Wait for Vsync
        window.display();
	}
    return 0;
}