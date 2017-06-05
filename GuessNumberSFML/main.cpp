#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <string>

#define W 800
#define H 600
#define FRAMERATE 27

#include "anims.cpp"

#ifdef _DEBUG 
#include <iostream>
#endif


using namespace std;

bool loadRes();
void centerText( sf::Text *t, sf::Vector2<int> cd );

sf::Font bubblegum;
sf::Text titleText;
sf::Clock titleNextFrame;
sf::Vector2<int> m(50,0);
as::Animation<sf::Text*> t( &titleText, (FRAMERATE * 0.5) );
//KeyFrames fs( 15 );

//Try

int main() {
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!" );
	win.setFramerateLimit( FRAMERATE );

	if( false == loadRes() ) {
		return EXIT_FAILURE;
	}

	titleText.setFont( bubblegum ); 
	titleText.setOutlineColor( sf::Color( 242, 156, 216 ) );
	titleText.setOutlineThickness( 2 );
	titleText.setString( "Guessing Game!" );
	titleText.setOrigin( titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 );
	centerText( &titleText, sf::Vector2<int>( W, 60 ) );
	titleText.setRotation( -15 / 4 );
	t.Rotate( 15/2, false, 0, t.frames / 2 );
	t.Rotate( -15/2, false, t.frames / 2, t.frames );
	
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
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) ) {
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Return ) ) {
						#ifdef _DEBUG
						t.printDebug();
						#endif // _DEBUG
					}
					break;
				default:
					break;
			}
		}
		win.clear();
		win.draw( titleText );
		win.display();
		t.ProcessFrame( t.actFrame );
	}
}

bool loadRes() {
	string resFold = "Res\\";
	bubblegum.loadFromFile( resFold + "Bubblegum.ttf" );
	return true;
}

void centerText( sf::Text *t, sf::Vector2<int> cd ) {
	( *t ).setPosition( (cd.x / 2 - ( *t ).getGlobalBounds().width / 2) + ( *t ).getOrigin().x, cd.y + ( *t ).getOrigin().y );//Centrar texto
}