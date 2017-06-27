#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <ctime>
#include <string>

#ifdef _DEBUG 
#include <thread>
#include <iostream>
#define c(x) std::cout << x
#else
#define c(...)
#endif

#define EXTRAINFO 0
#if EXTRAINFO 1
#define AS_SHOWFRAMEINFO//ifdef AnimsSFML will output debugging stuff
#endif

#define W 800
#define H 600
#define FRAMERATE 27
#define WIN 4
#define COLD 3
#define WARM 2
#define HOT 1
#define BUT_SIZE 40
#define ROW_PADDING 5
#define HEART_SIZE 25
#define LIVES 20

#include "../../AnimsSFML/AnimsSFML/anims.cpp"

using namespace std;


////Variables\\\\
//GUI vars
sf::Font bubblegum;
/*--[[Hearts]]--*/
sf::Texture spHeart;
sf::RectangleShape rHeart( sf::Vector2<float>( HEART_SIZE, HEART_SIZE ) );
/*--[[For the title]]--*/
sf::Text titleText;
as::Animation<sf::Text*> t( &titleText, ( FRAMERATE * 0.5 ) );
/*--[[Square showing number]]--*/
sf::RectangleShape numPlace( sf::Vector2<float>( BUT_SIZE, BUT_SIZE ) );
sf::Texture spNumPlaceBase;
sf::Text number( "", bubblegum );
int XAxisAligner = ( ( W - 200 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;//This calculates the unused width from the [usable area], then divides it by to so it can be added to the objects, centering the game in the screen
int YAxisAligner = ( ( H - 90 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;
/*--[[Some debugging stuff]]--*/
sf::RectangleShape usableArea( sf::Vector2<float>( W - 200, H - 90 ) );
sf::CircleShape pointer( 3 );
//Mechanics vars
sf::Vector2i mouse( 0, 0 );
int theChosenOne = -1;
int magicNumber = 1;
int * tried = new int( 100 );//This var stores the numbers that were tried and their result
/* 0: Nothings
   1: Cold
   2: Warm
   3: Hot
   4: That's it!
*/
short tries = 5;//Tries left

unsigned long poiWin = 0;

#define c(x) std::cout << x
#include <iostream>

#include "functions.cpp"

int main() {
	#ifdef _DEBUG
	std::thread deb(debuggingThread);
	#endif
	sf::ContextSettings wcs;
	wcs.antialiasingLevel = 2;
	c( "Now we're opening the window\n" );
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!", sf::Style::None, wcs );
	poiWin = reinterpret_cast<int>(&win);
	std::cout << "Windows is localized at " << std::hex << poiWin;
	c( "As expected, nothings happened\n" );
	win.setFramerateLimit( FRAMERATE );
	win.setMouseCursorVisible( false );
	c( "Works...\n" );
	
	if( false == loadRes() ) {//Resources loading
		#ifndef _DEBUG
		system( "error.bat \"Resources loading failed\"" );
		#endif // !_DEBUG
		return EXIT_FAILURE;
	}
	#ifdef _DEBUG
	std::cout << "Resource loading finished with no problems";
	#endif // _DEBUG
	c( "Rrcs OK\n" );

	////Object configuracion
	initObjects();
	c( "Object initialization OK\n" );

	init();//Game initializacion
	c( "Game initialization OK\n" );
	c( "Can we start? " << win.isOpen() << std::endl );
	sf::Event event;
	while( win.isOpen() ) {
		while( win.pollEvent( event ) ) {
			switch( event.type ) {
				case sf::Event::Closed:
					c( "Closing!" );
					win.close();
					break;
				case sf::Event::Resized:
				{
					sf::FloatRect visibleArea( 0, 0, event.size.width, event.size.height );
					win.setView( sf::View( visibleArea ) );}
				break;
				case sf::Event::KeyPressed:
					c( "Key pressed\n" );
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) ) {
						c( "\nIt's ok for now\n" );
						try {
							win.close();
						} catch( void* ) {
							c( "sf::Event::KeyPressed > Error" );
						}
						c( "We're closing\n" );
						return 0;
						break;
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) ) {
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Return ) ) {
						#ifdef _DEBUG
						#endif // _DEBUG
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::R ) ) init();
					break;
				case sf::Event::MouseButtonPressed:
					c( "( " << mouse.x << "; " << mouse.y << " )\n" );
					//std::cout << "( " << mose.getPosition().x << "; " << mose.getPosition().y << " )\n";
					if( mouse.y >= 90 && mouse.x >= 100 && mouse.x <= W - 100 ) {
						theChosenOne = getNumberClicked( mouse.x, mouse.y );
						if( theChosenOne == magicNumber ) {
							winGame();
						} else {
							tries--;
							hintPlayer();
						}
					}
					break;
				default:
					c( "I don't want this shit...(" << event.type << ")\n" );
					break;

			}
		}

		mouse = sf::Mouse::getPosition( win );

		drawGui(win);
	}
}

