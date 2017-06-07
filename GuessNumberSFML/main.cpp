#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include <iostream>
#include <string>

/* Note:
I leave a gap of 100px on the left and right to make it more apealing, and on the top for the title.
+---------+
|  TITLE  |
| ####### |
| ####### |
+---------+
+ - |: Window border
#: Usable space

*/

#undef AS_SHOWFRAMEINFO
#define W 800
#define H 600
#define FRAMERATE 27

#include "../../AnimsSFML/AnimsSFML/anims.cpp"

#ifdef _DEBUG 
#include <iostream>
#endif


#define BUT_SIZE 40
#define ROW_PADDING 5

using namespace std;

////Functions\\\\
//Functionality
void centerText( sf::Text *t, sf::Vector2<int> cd );//Duh...
int getNumberClicked( int x, int y);
//Startup
bool loadRes();//Find and load resources
void init();//Reset game data to play again

////Variables\\\\
//GUI vars
sf::RenderWindow win;
/*--[[For the title]]--*/
sf::Font bubblegum;
sf::Text titleText;
sf::Clock titleNextFrame;
sf::Vector2<int> m( 50, 0 );
as::Animation<sf::Text*> t( &titleText, ( FRAMERATE * 0.5 ) );
/*--[[Square showing number]]--*/
sf::RectangleShape numPlace( sf::Vector2<float>( BUT_SIZE, BUT_SIZE ) );
sf::Texture spNumPlaceBase;
sf::Text number;
short theChosenOne = -1;
int XAxisAligner = ( ( W - 200 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;//This calculates the unused width from the [usable area], then divides it by to so it can be added to the objects, centering the game in the screen
int YAxisAligner = ( ( H - 90 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;
/*--[[Some debugging stuff]]--*/
sf::RectangleShape usableArea( sf::Vector2<float>( W - 200, H - 90 ) );
//Mechanics vars
sf::Vector2f mouse(0,0);
short * tried = new short[100];//This var stores the numbers that were tried and their result
/* 0: Nothings
   1: Cold
   2: Warm
   3: Hot
   4: That's it!
*/
short tries = 5;//Tries left

int main() {
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!" );
	win.setFramerateLimit( FRAMERATE );

	if( false == loadRes() ) {
		return EXIT_FAILURE;
	}

	////Object configuracion
	{
		//Title
		titleText.setFont( bubblegum );
		titleText.setOutlineColor( sf::Color( 242, 156, 216 ) );
		titleText.setOutlineThickness( 2 );
		titleText.setString( "Guessing Game!" );
		titleText.setOrigin( titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 );
		centerText( &titleText, sf::Vector2<int>( W, 50 ) );
		titleText.setRotation( -15 / 4 );
		t.Rotate( 15 / 2, false, 0, t.frames / 2 );
		t.Rotate( -15 / 2, false, t.frames / 2, t.frames );
		//Number placeholder
		numPlace.setTexture( &spNumPlaceBase );
		numPlace.setPosition( W / 2, H / 2 );
		number.setFont( bubblegum );
		number.setFillColor( sf::Color( 197, 255, 255 ) );
		number.setOutlineColor( sf::Color::Black );
		number.setOutlineThickness( 1 );
		number.setString( "0" );
		//Usable area for numbers
		usableArea.setPosition( sf::Vector2<float>((float)100.0,(float)90.0) );
		usableArea.setFillColor( sf::Color( 25, 25, 25 ) );
	}

	sf::Event event;
	while( win.isOpen() ) {
		while( win.pollEvent( event ) ) {
			switch( event.type ) {
				case sf::Event::Closed:
					win.close();
					break;
				case sf::Event::Resized:
					{
					sf::FloatRect visibleArea( 0, 0, event.size.width, event.size.height );
					win.setView( sf::View( visibleArea ) );}
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
				case sf::Event::MouseButtonPressed:
					/*#ifdef _DEBUG
					std::cout << "( " << event.mouseButton.x << "; " << event.mouseButton.y << " )\n";
					#endif // _DEBUG
					if( event.mouseButton.y >= 90 && event.mouseButton.x >= 100 && event.mouseButton.x <= W - 100 )
						getNumberClicked( event.mouseButton.x, event.mouseButton.y );*/
					//mouse = win.mapCoordsToPixel(sf::Vector2f(event.mouseButton.x,event.mouseButton.y));
					mouse = win.mapPixelToCoords( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) );
					#ifdef _DEBUG
						std::cout << "( " << mouse.x << "; " << mouse.y << " )\n";
					#endif // _DEBUG
					if( mouse.y >= 90 && mouse.x >= 100 && mouse.x <= W - 100 )
						getNumberClicked( mouse.x, mouse.y );
					break;
				default:
					break;
			}
		}
		win.clear();
		win.draw( usableArea );
		for( int i = 0; i < 100; i++ ) {
			int x, y;
			y = ( i - ( i % 10 ) ) / 10;
			x = i % 10;
			y = y * ( BUT_SIZE + ROW_PADDING ) + 90;
			x = x * ( BUT_SIZE + ROW_PADDING ) + 100;
			number.setString( to_string( i ) );
			number.setPosition( x + ( BUT_SIZE / 2 - number.getLocalBounds().width / 2 ) + XAxisAligner, y + ( BUT_SIZE / 2 - number.getLocalBounds().height / 2 ) - 5 + YAxisAligner );
			numPlace.setPosition( x + XAxisAligner, y + YAxisAligner );
			if( i == theChosenOne ) {
				number.setFillColor(sf::Color::Red);
			}
			win.draw( numPlace );
			win.draw( number );
			if( i == theChosenOne ) {
				number.setFillColor( sf::Color( 197, 255, 255 ) );
			}
		}
		win.draw( titleText );
		win.display();
		t.ProcessFrame( t.actFrame );
	}
}

void init() {

}

int getNumberClicked( int x, int y ) {
	sf::Vector2<int> boardClick( ( x - ( 100 + XAxisAligner ) ), ( y - ( 60 + YAxisAligner ) ) );//Store the coords relative to the game, not the top-left window corner
	sf::Vector2<int> gridClick(-1,-1);//Row and column clicked (x+y*10=number)
	int number = -1;//Clicked number, if [number]<0||[number]>99 then nothing was clicked
	//number = ( boardClick.x - ( boardClick.x % 10 ) ) + ( boardClick.y % 10 );
	for( int col = 0; col <= 10; col++ ) {
		if( boardClick.x > col*( BUT_SIZE + ROW_PADDING ) && boardClick.x < ( col*( BUT_SIZE + ROW_PADDING ) ) + BUT_SIZE ) {
			std::cout << "X: " << col << std::endl;
			gridClick.x = col;
		}
		if( boardClick.y > col*( BUT_SIZE + ROW_PADDING ) && boardClick.y < ( col*( BUT_SIZE + ROW_PADDING ) ) + BUT_SIZE  ) {
			std::cout << "Y: " << col << std::endl; 
			gridClick.y = col;
		}
	}
	if( gridClick.x != -1 && gridClick.y != -1 )
		number = gridClick.y * 10 + gridClick.x;
	else
		number = -1;
	theChosenOne = number;
	return number;
}

bool loadRes() {
	string res = "Res\\";
	bubblegum.loadFromFile( res + "Bubblegum.ttf" );
	spNumPlaceBase.loadFromFile( res + "PlaceholderForNums.png" );
	return true;
}

void centerText( sf::Text *t, sf::Vector2<int> cd ) {
	( *t ).setPosition( ( cd.x / 2 - ( *t ).getGlobalBounds().width / 2 ) + ( *t ).getOrigin().x, cd.y + ( *t ).getOrigin().y );//Centrar texto
}