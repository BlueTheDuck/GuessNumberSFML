#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <ctime>
#include <string>

#ifdef _DEBUG 
#include <iostream>
#define c(x) std::cout << x
#else
#define c(...)
#endif

#undef AS_SHOWFRAMEINFO//ifdef AnimsSFML will output debugging stuff
#define W 800
#define H 600
#define FRAMERATE 27
#define WIN 4
#define COLD 3
#define WARM 2
#define HOT 1
#define BUT_SIZE 40
#define ROW_PADDING 5

#include "../../AnimsSFML/AnimsSFML/anims.cpp"

using namespace std;

////Variables\\\\
//GUI vars
sf::Font bubblegum;
/*--[[Hearts]]--*/
sf::Texture spHeart;
sf::RectangleShape rHeart;
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

#define c(x) std::cout << x
#include <iostream>

#include "functions.cpp"

sf::ContextSettings wcs;

int main() {
	wcs.antialiasingLevel = 2;
	c( "Now we're opening the window\n" );
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!", sf::Style::None,wcs );
	c( "As expected, nothings happened\n" );
	win.setFramerateLimit( FRAMERATE );
	win.setMouseCursorVisible( false );
	//win.getSettings = wcs;
	c( "Works...\n" );
	
	//t.onAnimationEnd.operator=( &processEventTest<sf::Text*> );//Set the onAnimationEnd for the [t] animation

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
					win.setView( sf::View( visibleArea ) );
				}
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

		win.clear();
		#ifdef _DEBUG
		win.draw( usableArea );
		#endif // _DEBUG

		for( int i = 0; i < tries; i++ ) {

		}
		for( int i = 0; i < 100; i++ ) {
			int x, y;
			y = ( i - ( i % 10 ) ) / 10;
			x = i % 10;
			y = y * ( BUT_SIZE + ROW_PADDING ) + 90;
			x = x * ( BUT_SIZE + ROW_PADDING ) + 100;
			number.setString( to_string( i ) );
			number.setPosition( x + ( BUT_SIZE / 2 - number.getLocalBounds().width / 2 ) + XAxisAligner, y + ( BUT_SIZE / 2 - number.getLocalBounds().height / 2 ) - 5 + YAxisAligner );
			numPlace.setPosition( x + XAxisAligner, y + YAxisAligner );
			if( tried[i] == COLD )
				number.setFillColor( sf::Color::Blue );
			else if( tried[i] == WARM )
				number.setFillColor( sf::Color::Yellow );
			else if( tried[i] == HOT )
				number.setFillColor( sf::Color::Red );
			else  if( tried[i] == WIN )
				number.setFillColor( sf::Color::Green );
			else
				number.setFillColor( sf::Color( 197, 255, 255 ) );
			if( i == 1 && false )c( "Now we are drawing the tricky shitty motherfucker part (like, FUCKIT!)\n" );
			win.draw( numPlace );
			win.draw( number );
			if( i == 1 && false )c( "It works... for now\n" );
		}
		win.draw( titleText );
		pointer.setPosition( win.mapPixelToCoords( mouse ) );
		win.draw( pointer );
		win.display();
		t.ProcessFrame( t.actFrame );
	}
}

