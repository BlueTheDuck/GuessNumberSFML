#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <string>

#define W 800
#define H 600
#define FRAMERATE 27
/* Note:
I leave a gap of 100px on the left and right to make it more apealing and on the top for the title.
+---------+
|  TITLE  |
| ####### |
| ####### |
+---------+
+ - |: Window border
#: Usable space

*/

#include "anims.cpp"

#ifdef _DEBUG 
#include <iostream>
#endif

using namespace std;

////Functions\\\\
//Functionality
void centerText( sf::Text *t, sf::Vector2<int> cd );//Duh...
//Startup
bool loadRes();//Find and load resources
void init();//Reset game assets

////Variables\\\\
//GUI vars
/*--[[For the title]]--*/
sf::Font bubblegum;
sf::Text titleText;
sf::Clock titleNextFrame;
sf::Vector2<int> m(50,0);
as::Animation<sf::Text*> t( &titleText, (FRAMERATE * 0.5) );
/*--[[Quare showing number]]--*/
sf::RectangleShape numPlace( sf::Vector2<float>( 60, 60 ) );
sf::Texture spNumPlaceBase;
//Mechanics vars
bool * tried = new bool[100];//This var stores the numbers that were tried
short tries = 5;//Tries left


int main() {
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!" );
	win.setFramerateLimit( FRAMERATE );

	if( false == loadRes() ) {
		return EXIT_FAILURE;
	}

	////Object configuracion
	//Title
	titleText.setFont( bubblegum ); 
	titleText.setOutlineColor( sf::Color( 242, 156, 216 ) );
	titleText.setOutlineThickness( 2 );
	titleText.setString( "Guessing Game!" );
	titleText.setOrigin( titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 );
	centerText( &titleText, sf::Vector2<int>( W, 60 ) );
	titleText.setRotation( -15 / 4 );
	t.Rotate( 15/2, false, 0, t.frames / 2 );
	t.Rotate( -15/2, false, t.frames / 2, t.frames );
	//Number placeholder
	//numPlace
	numPlace.setTexture( &spNumPlaceBase );
	numPlace.setPosition( W/2, H/2 );

	sf::Event event;
	while( win.isOpen() ) {
		while( win.pollEvent( event ) ) {
			switch( event.type ) {
				case sf::Event::Closed:
					win.close();
					break;
				case sf::Event::KeyPressed:
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
						win.close();
						break;
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) ) {
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Return ) ) {
						#ifdef _DEBUG
						t.printDebug();
						#endif // _DEBUG
					}
					break;
				case sf::Event::MouseButtonReleased:
					#ifdef _DEBUG
					std::cout << "( " << event.mouseButton.x << "; " << event.mouseButton.y << " )\n";
					#endif // _DEBUG
					break;
				default:
					break;
			}
		}
		win.clear();
		win.draw( numPlace );
		win.draw( titleText );
		win.display();
		t.ProcessFrame( t.actFrame );
	}
}

void init() {

}

bool loadRes() {
	string res = "Res\\";
	bubblegum.loadFromFile( res + "Bubblegum.ttf" );
	spNumPlaceBase.loadFromFile( res + "PlaceholderForNums.png" );
	return true;
}

void centerText( sf::Text *t, sf::Vector2<int> cd ) {
	( *t ).setPosition( (cd.x / 2 - ( *t ).getGlobalBounds().width / 2) + ( *t ).getOrigin().x, cd.y + ( *t ).getOrigin().y );//Centrar texto
}