#ifndef PROGRAM_FUNCTIONS
#define PROGRAM_FUNCTIONS
////Functions\\\\
//Functionality
void centerText( sf::Text *t, sf::Vector2<int> cd );//Duh...
int getNumberClicked( int x, int y );
void winGame();
int hintPlayer();
void drawGui( sf::RenderWindow & win );
void processCommand( std::string cmd, std::string arg1, std::string arg2 );
//Startup
bool loadRes();//Find and load resources
void init();//Reset game data to play again
void initObjects();

//Functionality
void centerText( sf::Text *t, sf::Vector2<int> cd ) {
	( *t ).setPosition( ( cd.x / 2 - ( *t ).getGlobalBounds().width / 2 ) + ( *t ).getOrigin().x, cd.y + ( *t ).getOrigin().y );//Centrar texto
}

int getNumberClicked( int x, int y ) {
	sf::Vector2<int> boardClick( ( x - ( 100 + XAxisAligner ) ), ( y - ( YAxisAligner + 90 ) ) );//Store the coords relative to the game, not the top-left window corner
	sf::Vector2<int> gridClick( -1, -1 );//Row and column clicked (x+y*10=number)
	int number = -1;//Clicked number, if [number]<0||[number]>99 then nothing was clicked
	for( int col = 0; col <= 10; col++ ) {
		if( boardClick.x > col*( BUT_SIZE + ROW_PADDING ) && boardClick.x < ( col*( BUT_SIZE + ROW_PADDING ) ) + BUT_SIZE ) {
			c("X: " << col << std::endl);
			gridClick.x = col;
		}
		if( boardClick.y > col*( BUT_SIZE + ROW_PADDING ) && boardClick.y < ( col*( BUT_SIZE + ROW_PADDING ) ) + BUT_SIZE ) {
			c("Y: " << col << std::endl);
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
	c("Win");
	tried[theChosenOne] = WIN;
	//titleText.setPosition( W / 2, H / 2 );
	titleText.setString("WIN!");
	titleText.setOrigin( titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 );
	titleText.setFillColor(sf::Color::Green);
	titleText.setOutlineColor( sf::Color( 0, 218, 0 ) );
	centerText( &titleText, sf::Vector2i( W, H / 2 ) );
}

int hintPlayer() {
	unsigned int nearRange = COLD;//If it isn't warm or hot, then use the default value: Cold
	c("Generating hint\n");

	int dist = theChosenOne - magicNumber;
	if( dist < 0 ) dist *= -1;
	if( dist < 4 ) {
		nearRange = HOT;
	} else if( dist < 7 ) {
		nearRange = WARM;
	}

	switch( nearRange ) {
		case 3:
			c("Cold");
			break;
		case 2:
			c("Warm");
			break;
		case 1:
			c("Hot");
			break;
		default:
			break;
	}
	tried[theChosenOne] = nearRange;
	return nearRange;
}

//Startup
bool resourcesLoaded = false;
bool loadRes() {//This should only run ONCE
	string res = "Res\\";
	if( resourcesLoaded == false ) {
		resourcesLoaded = ( bubblegum ).loadFromFile( res + "Bubblegum.ttf" ) &&
			spNumPlaceBase.loadFromFile( res + "PlaceholderForNums.png" ) && 
			spHeart.loadFromFile( res + "Heart.png" );
		/*std::cout << ( bubblegum ).loadFromFile( res + "Bubblegum.ttf" );
		std::cout << spNumPlaceBase.loadFromFile( res + "PlaceholderForNums.png" );*/
		//resourcesLoaded = true;
		spNumPlaceBase.setSmooth( true );
		spHeart.setSmooth( true );
	}
	c(resourcesLoaded);
	return resourcesLoaded;
}

void init() {//This function should run every time the game is initialized
	for( int i = 0; i < 100; i++ ) tried[i] = 0;
	srand( time( NULL ) );
	magicNumber = rand() % 100;//( (float)rand() / RAND_MAX * 100 );
	#ifdef _DEBUG
	std::cout << "Magic number: " << magicNumber;
	#endif
	//Title (Changed during void winGame())
	titleText.setFillColor( sf::Color::White );
	titleText.setOutlineColor( sf::Color( 242, 156, 216 ) );
	titleText.setOutlineThickness( 2 );
	titleText.setString( "Guessing Game!" );
	titleText.setOrigin( titleText.getGlobalBounds().width / 2, titleText.getGlobalBounds().height / 2 );
	centerText( &titleText, sf::Vector2<int>( W, 50 ) );
	tries = LIVES;
}

void initObjects() {//This function should only run ONCE
	//Title (generic)
	titleText.setFont( bubblegum );
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
	//Hearts
	rHeart.setTexture( &spHeart );
	//Debugging stuff
	usableArea.setPosition( sf::Vector2<float>( ( float )100.0, ( float )90.0 ) );
	usableArea.setFillColor( sf::Color( 25, 25, 25 ) );
	pointer.setFillColor( sf::Color::Red );
}

/*template <typename T>
bool processEventTest( as::Animation<T> *const ptr ) {
	std::cout << "Event finished. ID: " << (*ptr).ID << "\n";
	return true;
}*/

void drawGui( sf::RenderWindow &win ) {
	win.clear();
	win.draw( usableArea );

	#define COLS 5
	for( int i = 0; i < tries; i++ ) {//0-4
		int x = i % COLS;
		int y = ( i - x ) / COLS;
		rHeart.setPosition( W - HEART_SIZE*( x + 1 ), HEART_SIZE * ( y ) );
		win.draw( rHeart );
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
		win.draw( numPlace );
		win.draw( number );
	}

	win.draw( titleText );
	pointer.setPosition( win.mapPixelToCoords( mouse ) );
	win.draw( pointer );
	win.display();
	t.ProcessFrame( t.actFrame );
}

void debuggingThread() {
	std::string command;
	std::string in;
	std::string cmd;
	std::string arg1;
	std::string arg2;
	for( ;;) {
		std::cout << ">> ";
		std::getline(std::cin,in);
		std::cout << in << std::endl;
		if( in[0] == '/' ) {
			std::cout << "That was a command\n";
			int space = in.find( " " );
			cmd = in.substr( 1, space - 1 );
			int space2 = in.find( " ", space );
			if(space2!=-1) {
				arg1 = in.substr( space2 + 1, space2 - 1 );
				int space3 = in.find( " ", space2 );
				if( space3 != -1 ) {
					arg2 = in.substr( space3 + 1, space3 - 1 );
				} else {
					arg2 = "";
				}
			} else {
				arg1 = "";
				arg2 = "";
			}
			std::cout << "'" << cmd << "','" << arg1 << "','" << arg2 << "'\n";
			processCommand(cmd,arg1,arg2);
		} else {
			cmd = "";
		}
	}
}
void processCommand( std::string cmd, std::string arg1, std::string arg2 ) {
	#define CMDS 3
	int cmdi = -1;
	std::string validCmds[CMDS] = {
		"exit",
		"set",
		"print"
	};
	int vars[2] = {
		0x0,
		0x0
	};
	vars[0] = poiWin;
	for( int i = 0; i < CMDS; i++ ) {
		if( validCmds[i] == cmd ) {
			cmdi = i;
		}
	}
	switch( cmdi ) {
		case 0:
			//reinterpret_cast<sf::RenderWindow>( poiWin );
			break;
		case 1:
			break;
		case 2: {
			int arg1i = std::stoi( arg1 );
			printf("Addr: %i",vars[arg1i]);}
			break;
		case -1:
			std::cout << "Invalid CMD";
			break;
		default:
			break;
	}
}
#endif
