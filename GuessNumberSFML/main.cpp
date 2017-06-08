#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <ctime>
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
#define WIN 4
#define COLD 3
#define WARM 2
#define HOT 1

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
void initObjects();
void winGame();
int hintPlayer();

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
int XAxisAligner = ( ( W - 200 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;//This calculates the unused width from the [usable area], then divides it by to so it can be added to the objects, centering the game in the screen
int YAxisAligner = ( ( H - 90 ) - ( BUT_SIZE * 10 + ROW_PADDING * 9 ) ) / 2;
/*--[[Some debugging stuff]]--*/
sf::RectangleShape usableArea( sf::Vector2<float>( W - 200, H - 90 ) );
sf::CircleShape pointer(3);
//Mechanics vars
sf::Vector2i mouse(0,0);
int theChosenOne = -1;
int magicNumber;
int * tried = new int(100);//This var stores the numbers that were tried and their result
/* 0: Nothings
   1: Cold
   2: Warm
   3: Hot
   4: That's it!
*/
short tries = 5;//Tries left


int main() {
	sf::RenderWindow win( sf::VideoMode( W, H ), "Guessing Game!", sf::Style::Default );
	win.setFramerateLimit( FRAMERATE );

	if( false == loadRes() ) {//Resources loading
		return EXIT_FAILURE;
	}
	#ifdef _DEBUG
	std::cout << "Resource loading finished with no problems";
	#endif // _DEBUG


	////Object configuracion
	initObjects();

	init();//Game initializacion

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
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) ) {
						win.close();
						break;
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) ) {
					}
					if( sf::Keyboard::isKeyPressed( sf::Keyboard::Return ) ) {
						#ifdef _DEBUG
						#endif // _DEBUG
					}
					break;
				case sf::Event::MouseButtonPressed:
					std::cout << "( " << mouse.x << "; " << mouse.y << " )\n";
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
				case sf::Event::MouseMoved:
					break;
				default:
					break;
			}
		}

		mouse = sf::Mouse::getPosition( win );

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
				//number.setFillColor(sf::Color::Red);
			}
			//number.setFillColor( tried[i] == COLD?sf::Color::Blue:( tried[i] == WARM?sf::Color::Yellow:sf::Color::Red ) );
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
			win.draw( numPlace );
			win.draw( number );
		}
		win.draw( titleText );
		#ifdef _DEBUG
		pointer.setPosition( win.mapPixelToCoords( mouse ) );
		win.draw( pointer );
		#endif // _DEBUG
		win.display();
		t.ProcessFrame( t.actFrame );
	}
}

void initObjects() {//This function should only run ONCE
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
	//Debugging stuff
	usableArea.setPosition( sf::Vector2<float>( ( float )100.0, ( float )90.0 ) );
	usableArea.setFillColor( sf::Color( 25, 25, 25 ) );
	pointer.setFillColor( sf::Color::Red );
}

void init() {//This function should run every time the game is initialized
	for( int i = 0; i < 100; i++ ) tried[i] = 0;
	srand( time( NULL ) );
	magicNumber = ((float)rand() / RAND_MAX * 100);
	#ifdef _DEBUG
	std::cout << "Magic number: " << magicNumber;
	#endif
	//tried = { 3 };
}

int getNumberClicked( int x, int y ) {
	sf::Vector2<int> boardClick( ( x - ( 100 + XAxisAligner ) ), ( y - ( YAxisAligner + 90 ) ) );//Store the coords relative to the game, not the top-left window corner
	sf::Vector2<int> gridClick(-1,-1);//Row and column clicked (x+y*10=number)
	int number = -1;//Clicked number, if [number]<0||[number]>99 then nothing was clicked
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
	return number;
}

void winGame() {
	std::cout << "Win";
	tried[theChosenOne] = WIN;

}

int hintPlayer() {
	unsigned int nearRange = COLD;
	std::cout << "Generating hint\n";

	short hotMinRange = theChosenOne - 3, hotMaxRange = theChosenOne + 3;
	short warmMinRange = theChosenOne - 5, warmMaxRange = theChosenOne + 5;

	//Check if the ranging numbers are offset
	{
		if( hotMinRange < 0 )
			hotMinRange += 100;
		if( hotMaxRange > 100 )
			hotMaxRange -= 100;
		if( warmMinRange < 0 )
			warmMinRange += 100;
		if( warmMaxRange > 100 )
			warmMaxRange -= 100;
	}

	if( warmMinRange < magicNumber && warmMaxRange > magicNumber )//Warm
		nearRange = WARM;
	if( hotMinRange < magicNumber && hotMaxRange > magicNumber )//Warm
		nearRange = HOT;
	switch( nearRange ) {
		case 3:
			std::cout << "Cold";
			break;
		case 2:
			std::cout << "Warm";
			break;
		case 1:
			std::cout << "Hot";
			break;
		default:
			break;
	}
	tried[theChosenOne] = nearRange;
	return nearRange;
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