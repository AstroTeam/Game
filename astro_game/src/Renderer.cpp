#include "main.hpp"
//#include "engine.cpp"
#include "SDL/SDL.h"
//#include "SDL/SDL_image.h"
#include <string>
#include <iostream>
using namespace std;
//0 ->std, 1->blit, 2->nothing

void Renderer::render(void *sdlSurface){
/////////////////////////////////////////////////////////////////////rendering doubled up walkable decor!
	SDL_Surface *screen=(SDL_Surface *)sdlSurface;
	static bool first=true;
	if ( first ) {
			first=false;
		// set blue(255) as transparent for the root console
		// so that we only blit characters
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,255);
			
	}
	if (engine.invState == 0)
	{
	
	if (engine.menuState == 0)
	{
	//cout << "start new frame" << endl;
	//set fps
	TCODSystem::setFps(60);
	//floors
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,255);
	SDL_Surface *floorTiles = SDL_LoadBMP("tile_assets/tiles.bmp");
	SDL_SetColorKey(floorTiles,SDL_SRCCOLORKEY,255);
	//glowing variable alpha set
	static int alpha = 255*.5;
	static bool Down = true;
	if (alpha > 0 && Down)
	{
		alpha = alpha-5;
		if (alpha < 0)
			alpha = 0;
	}
	else if (alpha == 0 && Down)
	{
		Down = false;
	}
	else if (alpha < 255*.5 )
	{
		alpha = alpha+5;
	}
	else if (alpha > 255*.5)
	{
		Down = true;
	}
	//glows
	SDL_Surface *itemsGlow = SDL_LoadBMP("tile_assets/alphaGlow.bmp");
	SDL_SetAlpha( itemsGlow, SDL_SRCALPHA, alpha);
	SDL_SetColorKey(itemsGlow,SDL_SRCCOLORKEY,255);
	SDL_Surface *serverLight = SDL_LoadBMP("tile_assets/server_lights.bmp");
	SDL_Surface *fire = SDL_LoadBMP("tile_assets/fire.bmp");
	SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.9);
	SDL_SetColorKey(serverLight,SDL_SRCCOLORKEY,255);
	SDL_SetColorKey(fire,SDL_SRCCOLORKEY,255);
	
	SDL_Surface *flareGlow= SDL_LoadBMP("tile_assets/alphaGlow_flare.bmp");
	SDL_SetAlpha(flareGlow, SDL_SRCALPHA, alpha*1.25);
	SDL_SetColorKey(flareGlow,SDL_SRCCOLORKEY,255);
	//shadows
	SDL_Surface *shadows = SDL_LoadBMP("tile_assets/itemShadows.bmp");
	SDL_SetAlpha(shadows, SDL_SRCALPHA, 255*.25);
	SDL_SetColorKey(shadows,SDL_SRCCOLORKEY,255);
	//EQUIPMENT
	SDL_Surface *equipment = SDL_LoadBMP("tile_assets/equipment.bmp");
	SDL_SetColorKey(equipment,SDL_SRCCOLORKEY,255);
	//DECORATIONS
	SDL_Surface *decor = SDL_LoadBMP("tile_assets/decorations.bmp");
	SDL_SetColorKey(decor,SDL_SRCCOLORKEY,255);
	//Background
	SDL_Surface *floorMap = SDL_LoadBMP("starmap2.bmp");
	SDL_Surface *terminal = SDL_LoadBMP("tile_assets/terminal.bmp");
	SDL_SetColorKey(terminal,SDL_SRCCOLORKEY,255);
	//flowers
	SDL_Surface *flowers = SDL_LoadBMP("tile_assets/flowers.bmp");
	SDL_SetColorKey(flowers,SDL_SRCCOLORKEY,255);
	//alcohols
	SDL_Surface *alcohol = SDL_LoadBMP("tile_assets/alcohol.bmp");
	SDL_SetColorKey(alcohol,SDL_SRCCOLORKEY,255);
	
	
	
	
	//messages
	//engine.gui->message(TCODColor::red, "x1 is %d",engine.mapx1);
	//engine.gui->message(TCODColor::red, "x2 is %d",engine.mapx2);
	//engine.gui->message(TCODColor::red, "y1 is %d",engine.mapy1);
	//engine.gui->message(TCODColor::red, "y2 is %d",engine.mapy2);
	//engine.gui->message(TCODColor::red, "playerx  %d",plyx);
	//engine.gui->message(TCODColor::red, "playery  %d",plyy);
	static int fireInt = 0;
	static int slowing = 0;
	
	SDL_Rect srcRect={0,0,16,16};
	SDL_Rect dstRect1={22*16,0,(engine.mapx2-engine.mapx1)*16+16,(engine.mapy2-engine.mapy1)*16+16};
	int x = 0, y = 0;
	int plyx = 0, plyy = 0;
	//cout << "start main loop" << endl;
	for (int xM = engine.mapx1; xM < engine.mapx2+16; xM++) {
		//cout << "xM loop" << endl;
		for (int yM = engine.mapy1; yM < engine.mapy2+16; yM++) {
			//cout << xM << " , " << yM << endl;
			//cout << "yM loop" << endl;
			//refresh if looking
			SDL_Rect dstRect={x*16,y*16,16,16};
			//cout << "looking refresh" << endl;
			if (TCODConsole::root->getCharBackground(22*16+xM,yM) == TCODColor::darkerPink || //refresh if looking
				TCODConsole::root->getCharBackground(22*16+xM,yM) == TCODColor::pink)
			{
				TCODConsole::root->clear();
			}
			//cout << "player refresh/looking over" << endl;
			//detects where the player is, if it is @, human, alien or robot base
			if(engine.mapcon->getChar(xM,yM) == 64 || engine.mapcon->getChar(xM,yM) == 143 ||
     		  engine.mapcon->getChar(xM,yM) == 159 || engine.mapcon->getChar(xM,yM) == 175)
			{
				plyx = x;
				plyy = y;
				//refresh if not deadz  (163 is corpse)
				if (engine.mapcon->getChar(xM,yM) != 163){//|| engine.gameStatus != engine.MAIN_MENU){ //refresh if player is alive
					TCODConsole::root->clear();	
				}
				
			}
			//cout << "refreshing over, begin floor scans" << endl;
			//if the tile is a floor
			if(engine.mapconCpy->getChar(xM,yM) == 31) 
			{ //replace 'down arrow thing' (31 ascii) with basic floor tiles
				//cout << "refreshing over, begin floor scans2" << endl;
				//office floors, can test for any floors
				int r = engine.map->tileType(xM,yM);
				if (r == 2)//2 is office
				{
					srcRect.x = 48;
				}
				else if (r == 3)//3 is barracks
				{
					srcRect.x = 16;
				}
				else if (r == 4)//4 is generator
				{
					srcRect.x = 32;
				}
				else if (r == 5 || r == 7)//5 is kitchen and messhall
				{
					srcRect.x = 160;
				}
				else if (r == 6)//6 is server
				{
					srcRect.x = 176;
				}
				else if (r == 9)//9 is observatory
				{
					srcRect.x = 176+16+16;
				}
				else if (r == 10)//10 is hydroponics
				{
					srcRect.x = 176+16;
				}
				else if(r == 8)//8 is armory
				{
					srcRect.x = 176+16+16+16;
				}
				else if(r == 12)//12 is bar
				{
					srcRect.x = 240;
				}
				else if(r == 14)//14 is bar display
				{
					srcRect.x = 240+16;
				}
				else //else is regular floors
				{
					srcRect.x = 0;
				}
				srcRect.y = 0;
				SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
				
				//add environment stuffs-> under things here (scorch, ice)
				
				
				//render infection over it
				if (engine.map->infectionState(xM,yM) > 0)
				{
					if (engine.map->infectionState(xM,yM) <= 6) {
						srcRect.x = 64 + 16 * (engine.map->infectionState(xM, yM) - 1);
					}
					else {
						srcRect.x = 64 + 16 * 5;
					}
					srcRect.y = 0;
					SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
					//render flowers
					if (engine.map->tiles[xM+yM*engine.map->width].flower != -1) {
						srcRect.x = 16 * engine.map->tiles[xM+yM*engine.map->width].flower;
							srcRect.y = 0;
						SDL_BlitSurface(flowers,&srcRect,floorMap,&dstRect);
					}
				}
				else if (engine.mapcon->getChar(xM,yM) == 7)
				{
					srcRect.x = 64 + 16 * 5;
					srcRect.y = 0;
					SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
				}

				/*//any decor to render just on top of floors
				if (engine.mapconDec->getChar(xM,yM) == ' ')
				{
					//do nothing if common case (i.e.) no decor
					//commmon case is fast
				}
				//if the decor is papers, render over
				else if (engine.mapconDec->getChar(xM,yM) == 5 || engine.mapconDec->getChar(xM,yM) == 6 || 
						 engine.mapconDec->getChar(xM,yM) == 7 || engine.mapconDec->getChar(xM,yM) == 8)
				{
					srcRect.y = 32;
					srcRect.x = 32+ (engine.mapconDec->getChar(xM,yM) - 5 ) * 16;
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
								
				}*/
						
					
				
				
				//OPTIMIZE ME -->  COMMON CASE IS SLOW
				
				//everything bodies to render behind
				if (engine.mapcon->getChar(xM,yM) == 181 || engine.mapcon->getChar(xM,yM) == 182 || engine.mapcon->getChar(xM,yM) == 183 || 
				engine.mapcon->getChar(xM,yM) == 184 || engine.mapcon->getChar(xM,yM) == 64 || engine.mapcon->getChar(xM,yM) == 164 || 
				engine.mapcon->getChar(xM,yM) == 165 || engine.mapcon->getChar(xM,yM) == 148 || engine.mapcon->getChar(xM,yM) == 132 ||
				engine.mapcon->getChar(xM,yM) == 186 || engine.mapcon->getChar(xM,yM) == 198 ||engine.mapcon->getChar(xM,yM) == 169) 
				{
					for (Actor **it = engine.actors.begin(); it != engine.actors.end(); it++) {//is walkable?
						Actor *actor = *it;
						if (actor->x == xM && actor->y == yM && actor->destructible && actor->destructible->isDead()) {
							//doubles += 1;
							SDL_Rect srcRect2={10*16,3*16,16,16};
							SDL_Rect dstRect={x*16,y*16,16,16};
							//10 width 3 height for standard bodies
							//if they are spore bodies
							if (actor->ch == 162){
								srcRect2.y = 2*16;
							}
							else if (actor->ch == 161){
								srcRect2.y = 1*16;
							}
							
							SDL_BlitSurface(terminal,&srcRect2,floorMap,&dstRect);
						}
					}
				}
				
			}
			//replace 'up arrow thing' with darker floor tiles
			
			else if(engine.mapconCpy->getChar(xM,yM) == 30)
			{
				//render office floors, same as lit tiles, just different y value
				int r = engine.map->tileType(xM,yM);
				if (r == 2) 
				{
					srcRect.x = 48;
				}
				else if (r == 3)//3 is barracks
				{
					srcRect.x = 16;
				}
				else if (r == 4)//4 is generator
				{
					srcRect.x = 32;
				}
				else if (r == 5 || r == 7)//5 is kitchen mess hall
				{
					srcRect.x = 160;
				}
				else if (r == 6)//6 is server
				{
					srcRect.x = 176;
				}
				else if (r == 9)//9 is observatory
				{
					srcRect.x = 176+16+16;
				}
				else if (r == 10 )//10 is hydroponics
				{
					srcRect.x = 176+16;
				}
				else if(r == 8)//8 is armory
				{
					srcRect.x = 176+16+16+16;
				}
				else if(r == 12)//12 is bar
				{
					srcRect.x = 240;
				}
				else if(r == 14)//14 is bar display
				{
					srcRect.x = 240+16;
				}
				else
				{
					srcRect.x = 0;
				}
				srcRect.y = 16;
				SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
				
				//add environment stuffs-> under things here (scorch, ice)
				
				
				if (engine.map->infectionState(xM,yM) > 0)
				{
					if (engine.map->infectionState(xM,yM) <= 6) {
						srcRect.x = 64 + 16 * (engine.map->infectionState(xM, yM)-1);
					}
					else {
						srcRect.x = 64 + 16 * 5;
					}
					srcRect.y = 16;
					SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
					//render flowers
					if (engine.map->tiles[xM+yM*engine.map->width].flower != -1) {
						srcRect.x = 16 * engine.map->tiles[xM+yM*engine.map->width].flower;
						srcRect.y = 16;
						SDL_BlitSurface(flowers,&srcRect,floorMap,&dstRect);
					}
				}
				else if (engine.mapcon->getChar(xM,yM) == 7)
				{
					srcRect.x = 64 + 16 * 5;
					srcRect.y = 16;
					SDL_BlitSurface(floorTiles,&srcRect,floorMap,&dstRect);
				}
				
				/*//render decor *DARK*
				if (engine.mapconDec->getChar(xM,yM) == ' ')
				{
					//do nothing if common case (i.e.) no decor
					//common case is fast
				}
				else if (engine.mapconDec->getChar(xM,yM) == 5 || engine.mapconDec->getChar(xM,yM) == 6 ||  //dark papers
						 engine.mapconDec->getChar(xM,yM) == 7 || engine.mapconDec->getChar(xM,yM) == 8)
				{
					srcRect.y = 48;
					srcRect.x = 32+ (engine.mapconDec->getChar(xM,yM) - 5 ) * 16;
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
								
				}*/
			}
			
			//cout << "refreshing over, begin floor scans3" << endl;
			
			//cout << "done with floors" << endl;
			//OPTIMIZE ME -->  COMMON CASE IS SLOW
			
			//shadows, always after tiles
			//flashbang shadow and glow
			if (engine.mapcon->getChar(xM,yM) == 181)  
			{
				srcRect.x = 16;
				srcRect.y = 0;
				SDL_BlitSurface(itemsGlow,&srcRect,floorMap,&dstRect);
				srcRect.x = 0;
				srcRect.y = 0;				
				SDL_BlitSurface(shadows,&srcRect,floorMap,&dstRect);	
			}
			//firebomb shadow and glow
			else if (engine.mapcon->getChar(xM,yM) == 182)  
			{
				srcRect.x = 0;
				srcRect.y = 0;
				SDL_BlitSurface(itemsGlow,&srcRect,floorMap,&dstRect);		
				SDL_BlitSurface(shadows,&srcRect,floorMap,&dstRect);	
			}
			//frag glow
			else if (engine.mapcon->getChar(xM,yM) == 198)  
			{
				srcRect.x = 64;
				srcRect.y = 0;
				SDL_BlitSurface(itemsGlow,&srcRect,floorMap,&dstRect);
				srcRect.x = 0;
				srcRect.y = 0;
				SDL_BlitSurface(shadows,&srcRect,floorMap,&dstRect);	
			}
			//EMP glow
			else if (engine.mapcon->getChar(xM,yM) == 183)  
			{
				srcRect.x = 32;
				srcRect.y = 0;
				SDL_BlitSurface(itemsGlow,&srcRect,floorMap,&dstRect);
			}
			//Battery Glow
			else if (engine.mapcon->getChar(xM,yM) == 186)  
			{
				srcRect.x = 48;
				srcRect.y = 0;
				SDL_BlitSurface(itemsGlow,&srcRect,floorMap,&dstRect);
			}
			//Medkit shadow
			else if (engine.mapcon->getChar(xM,yM) == 184)  
			{
				srcRect.x = 16;
				srcRect.y = 0;
				SDL_BlitSurface(shadows,&srcRect,floorMap,&dstRect);
			}
			//render BOOZE
			else if (engine.mapcon->getChar(xM,yM) == 15)  
			{
				srcRect.y = 0;
				srcRect.x = 0;
				//if (engine.map->tileType(xM,yM) == 12)
				//{
				//	srcRect.y += 32;
				//}
				if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white)
					{
						//light
						srcRect.y += 0;
					}
					else
					{
						//dark
						srcRect.y += 16;
					}
					int inc = engine.map->tiles[xM+yM*engine.map->width].decoration - 61; //61 because 0 indexed
					srcRect.x = inc*16;
					SDL_BlitSurface(alcohol,&srcRect,floorMap,&dstRect);
					
			}
			
			
			//render all decorations
			//cout << "decor start" << endl;
			//PROBLEM IN HERE BRO!
			//cout << (engine.map->tiles[19+110*engine.map->width].decoration != 0) << endl;
			//cout << "decor NOT rendering at ( "<< xM << "," << yM << ")" << endl;
			if((engine.gameStatus == engine.IDLE || engine.gameStatus == engine.NEW_TURN) && 
			(engine.map->tiles[xM+yM*engine.map->width].decoration != 0) && (engine.map->tiles[xM+yM*engine.map->width].explored) && engine.mapcon->getChar(xM,yM) != 15)
			{
				//cout << "decor rendering at ( "<< xM << "," << yM << ")" << endl;
				//if (xM <= 100 && yM <= 100)
				//{
					//cout << "decor rendering at ( "<< xM << "," << yM << ") which is less than 100" << endl;
				///////////////////////////////////////////////////////////////////////////////////OFFICE
				if (engine.map->tileType(xM,yM) == 2)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white)
					{
						//light
						srcRect.x = 0;
					}
					else
					{
						//dark
						srcRect.x = 16;
					}
					
					if (engine.map->tiles[xM+yM*engine.map->width].decoration == -1)//smashed filing cabinets
					{
						//srcRect.x = 0;
						srcRect.y = 64;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == -2)//filing cabinets upper
					{
						//srcRect.x = 0;
						srcRect.y = 0;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == -3)//filing cabinets left
					{
						//srcRect.x = 0;
						srcRect.y = 16;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == -4)//filing cabinets right
					{
						//srcRect.x = 0;
						srcRect.y = 32;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == -5)//filing cabinets lower
					{
						//srcRect.x = 0;
						srcRect.y = 48;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration >= 1 && engine.map->tiles[xM+yM*engine.map->width].decoration <= 4)
					{
						if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white) //&& engine.map->tiles[xM+yM*engine.map->width].decoration > 0 )
						{
							//light
							srcRect.y = 0;
						}
						else //if (engine.map->tiles[xM+yM*engine.map->width].decoration > 0)
						{
							//dark
							
							srcRect.y = 16;
						}
						
						if (engine.map->tiles[xM+yM*engine.map->width].decoration == 1)//desk 1
						{
							//srcRect.x = 0;
							//srcRect.y = 0;
							srcRect.x=32+48;
						}
						else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 2)//desk 2
						{
							//srcRect.x = 0;
							//srcRect.y = 0;
							srcRect.x=32+16;
						}
						else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 3)//desk 3
						{
							//srcRect.x = 0;
							//srcRect.y = 0;
							srcRect.x=32+32;
						}
						else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 4)//desk 4
						{
							//srcRect.x = 0;
							//srcRect.y = 0;
							srcRect.x = 32;
						}
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration >= 5 && engine.map->tiles[xM+yM*engine.map->width].decoration <= 8)
					{
						if (engine.map->tiles[xM+yM*engine.map->width].lit == true) //&& engine.map->tiles[xM+yM*engine.map->width].decoration > 0 )
						{
							//light
							srcRect.y = 32;
						}
						else //if (engine.map->tiles[xM+yM*engine.map->width].decoration > 0)
						{
							//dark
							
							srcRect.y = 48;
						}
						//srcRect.y += 32;
						srcRect.x = 32+ (engine.map->tiles[xM+yM*engine.map->width].decoration - 5 ) * 16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
				}
				////////////////////////////////////////////////////////////////////BARRACKS
				else if (engine.map->tileType(xM,yM) == 3)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white)
					{
						//light
						srcRect.y = 0;
					}
					else
					{
						//dark
						srcRect.y = 16;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 9)//headboard of bed 1
					{
						srcRect.x=96;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 10)//headboard of bed 2
					{
						srcRect.x=112;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 11)//headboardof bed 3
					{
						srcRect.x=128;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 15)//foot of bed 1
					{
						srcRect.x=144;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 16)//foot of bed 2
					{
						srcRect.x=160;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 17)//foot of bed 3
					{
						srcRect.x=176;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 12)//backward headboard 1
					{
						srcRect.x=96;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 13)//backward headboard 2
					{
						srcRect.x=112;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 14)//backward headboard 3
					{
						srcRect.x=128;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 18)//backward foot 1
					{
						srcRect.x=144;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 19)//backward foot 2
					{
						srcRect.x=160;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 20)//backward foot 3
					{
						srcRect.x=176;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 21)//endtable
					{
						srcRect.x=192;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 22)//endtable backwards
					{
						srcRect.x=208;
						srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 23)//lockers
					{
						srcRect.x=192;
					//srcRect.y += 32;
						//srcRect.y += 32;
					}else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 24)//locker looted
					{
						srcRect.x=208;
						//srcRect.y += 32;
					}
					
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
				}
				////////////////////////////////////////////////////////////////////GENERATORS
				else if (engine.map->tileType(xM,yM) == 4)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=0;
					}else if (engine.map->tileType(xM,yM) == 4){
						//dark
						srcRect.x=32;
					}
					
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 25)//jerry rigged tile
					{
						srcRect.x +=224;
						srcRect.y = 0;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 26)//danger sign
					{
						srcRect.x +=240;
						srcRect.y = 0;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 27)//cables
					{
						srcRect.x +=224;
						srcRect.y = 16;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 28)//oil drum
					{						
						srcRect.x +=240;
						srcRect.y = 16;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 29)//generator
					{
						srcRect.x +=224;
						srcRect.y = 32;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 30)//console
					{
						srcRect.x +=240;
						srcRect.y = 32;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 31)//blowtorch
					{
						srcRect.x +=224;
						srcRect.y = 48;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 32)//pallet
					{
						srcRect.x +=240;
						srcRect.y = 48;
					}
					
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
				}
				////////////////////////////////////////////////////////////////////KITCHEN
				else if (engine.map->tileType(xM,yM) == 5)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=0;
					}else{
						//dark
						srcRect.x=16;
					}
					
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 35)//counter
					{
						srcRect.x += 32;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 36)//upper counter
					{
						srcRect.x += 64;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 37)//sink
					{
						srcRect.x += 6*16;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 38)//upper sink
					{
						srcRect.x += 8*16;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 39)//oven-stove combo
					{
						srcRect.x += 10*16;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 40)//refrigerator
					{
						srcRect.x += 12*16;
						srcRect.y = 64;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 41)//destroyed counter
					{
						srcRect.x += 14*16;
						srcRect.y = 64;
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 44)//food processor
					{
						srcRect.x += 16*16;
						srcRect.y = 64;
					}
					
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
				}
				////////////////////////////////////////////////////////////////////SERVER
				else if (engine.map->tileType(xM,yM) == 6)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.y=32;
					}else{
						//dark/*commet*/
						srcRect.y=48;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 45)//server
					{
						
						srcRect.x = 18*16;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 46)//server
					{
						
						srcRect.x = 19*16;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 47)//server
					{
						
						srcRect.x = 20*16;
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 100)//server destroyed
					{
						
						srcRect.x = 22*16;
					}
					//if(engine.map->tiles[xM+yM*engine.map->width].decoration != 0)
					//{
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					//}
					TCODRandom *rng =TCODRandom::getInstance();
					if((engine.map->tiles[xM+yM*engine.map->width].decoration == 45 ||
					   engine.map->tiles[xM+yM*engine.map->width].decoration == 46 ||
					   engine.map->tiles[xM+yM*engine.map->width].decoration == 47) )//&& engine.mapcon->getCharForeground(xM,yM) == TCODColor::white)
					{
						
						if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
							SDL_SetAlpha( serverLight, SDL_SRCALPHA, 255*.75);
						}else{
							SDL_SetAlpha( serverLight, SDL_SRCALPHA, 255*.5);
						}
						
						srcRect.y = 0;
						srcRect.x = 0;
						srcRect.x += (rng->getInt(0,2))*16;
						SDL_BlitSurface(serverLight,&srcRect,floorMap,&dstRect);
					}
					
				}
				//////////////////////////////////////////////////////////////////////MESSHALL
				else if (engine.map->tileType(xM,yM) == 7)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.y=0;
					}else{
						//dark/*commet*/
						srcRect.y=16;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 49)//chair
					{
						
						srcRect.x = 18*16;
						
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 50)//table
					{
						
						srcRect.x = 19*16;
						
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 51)//table
					{
						
						srcRect.x = 20*16;
						
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 52)//table
					{
						
						srcRect.x = 21*16;
						
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 53)//trash can
					{
						
						srcRect.x = 22*16;
						
					}
					SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
				}
				////////////////////////////////////////////////////////////////////ARMORY
				else if (engine.map->tileType(xM,yM) == 8)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=0;
					}else{
						//dark/*commet*/
						srcRect.x=16;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 54)//gun rack
					{
						
						srcRect.y = 5*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 55)//battery rack
					{
						
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 56)//vault
					{
						
						srcRect.y = 7*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 57)//vault (open)
					{
						
						srcRect.y = 8*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 85)//empty rack
					{
						srcRect.x += 32;
						srcRect.y = 8*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					//SANDBAGS!
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=32;
					}else{
						//dark/*commet*/
						srcRect.x=48;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 58)//sandbag wall
					{
						
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 59)//sandbag wall upper
					{
						
						srcRect.y = 5*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 60)//sandbag wall lower
					{
						
						srcRect.y = 7*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					
				}
				////////////////////////////////////////////////////////////////////HYDROPONICS
				else if (engine.map->tileType(xM,yM) == 10)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.map->isInFov(xM,yM)){
						//light
						srcRect.y=32;
					}else{
						//dark/*commet*/
						srcRect.y=48;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 48)//hydro rack
					{
						
						srcRect.x = 21*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					
				//}
				}
				////////////////////////////////////////////////////////////////////DEFENDED_ROOM
				else if (engine.map->tileType(xM,yM) == 11)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=32;
					}else{
						//dark/*commet*/
						srcRect.x=48;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 58)//sandbag wall
					{
						
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 76)//sandbag wall LR
					{
						srcRect.x += 32;
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 77)//TL corner
					{
						srcRect.x += 32+32;
						srcRect.y = 5*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 78)//TR corner
					{
						srcRect.x += 32+32;
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 79)//BL corner
					{
						srcRect.x += 32+32;
						srcRect.y = 7*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if (engine.map->tiles[xM+yM*engine.map->width].decoration == 80)//BR corner
					{
						srcRect.x += 32+32;
						srcRect.y = 8*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					
					
					if (engine.map->isInFov(xM, yM)){
						//light
						srcRect.x=0;
					}else{
						//dark
						srcRect.x=32;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 32)//pallet
					{
						srcRect.x +=240;
						srcRect.y = 48;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
				}
				////////////////////////////////////////////////////////////////////BAR
				else if (engine.map->tileType(xM,yM) == 12)//|| engine.map->tileType(xM,yM) == 1)
				{
					if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
						//light
						srcRect.x=8*16;
					}else{
						//dark/*commet*/
						srcRect.x=9*16;
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 81 || engine.mapcon->getChar(xM,yM) == 15)//display
					{
						//srcRect.x +=240;
						srcRect.y = 5*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 82)//bar
					{
						//srcRect.x +=240;
						srcRect.y = 6*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 83)//bar
					{
						//srcRect.x +=240;
						srcRect.y = 7*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					else if(engine.map->tiles[xM+yM*engine.map->width].decoration == 84)//bar
					{
						//srcRect.x +=240;
						srcRect.y = 8*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
					if(engine.map->tiles[xM+yM*engine.map->width].decoration == 49)//chair
					{
						if (engine.mapcon->getCharForeground(xM,yM) == TCODColor::white){
							//light
							srcRect.y=0;
						}else{
							//dark/*commet*/
							srcRect.y=16;
						}
						srcRect.x = 18*16;
						SDL_BlitSurface(decor,&srcRect,floorMap,&dstRect);
					}
				}
				
				
			}
			if (engine.mapcon->getChar(xM,yM) == 157)
			{
				static int altitude = 0;
				SDL_Rect srcRectTemp={9*16,13*16,16,16};
				SDL_Rect dstRectTemp = dstRect;
				dstRectTemp.y+=altitude;
				SDL_BlitSurface(terminal,&srcRectTemp,floorMap,&dstRectTemp);
				static bool DownW = true;
				static int slows = 0;
				int amt = 2;
				if (slows%8 == 0)
				{
					if (altitude > -amt && DownW)
					{
						altitude--;
						if (altitude < -amt)
							altitude = -amt;
					}
					else if (altitude == -amt && DownW)
					{
						DownW = false;
					}
					else if (altitude < amt)
					{
						altitude++;;
					}
					else if (altitude >= amt)
					{
						DownW = true;
					}
				}	
				slows++;
			}
			
			
			//cout << "decor end" << endl;
			//TCODRandom *rng = TCODRandom::getInstance();
			//fireInt = rng->getInt(0,3);
			//add environment stuffs-> over things here (fire)
			//cout << "fire start" << endl;
			if ((engine.gameStatus == engine.IDLE || engine.gameStatus == engine.NEW_TURN) && engine.map->tiles[xM+yM*engine.map->width].envSta != 0)
			{
				//fire
				if (engine.map->tiles[xM+yM*engine.map->width].envSta == 1)
				{
					srcRect.x = (fireInt%3) * 16;
					srcRect.y = 0;
					//srcRect.x += (rng->getInt(0,2))*16;
					//if (slowing%5 == 0)
					if (engine.map->isInFov(xM,yM))
						{SDL_BlitSurface(fire,&srcRect,floorMap,&dstRect);}
					if (engine.map->tiles[xM+yM*engine.map->width].temperature == 0)
					{
						engine.map->tiles[xM+yM*engine.map->width].envSta = 2;
						for (int i = -1; i <= 1;i++)
						{
							for (int j = -1; j <= 1;j++)
							{
								if (engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].num == 0)
								engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].lit = false;
								//engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].num++;
							}
						}
					}
					else if (engine.gameStatus == engine.NEW_TURN)
					{
						engine.map->tiles[xM+yM*engine.map->width].temperature--;
						for (int i = -1; i <= 1;i++)
						{
							for (int j = -1; j <= 1;j++)
							{
								if (i != j || (j == 0 && i == 0))
								engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].lit = true;
								
								
								
								//if (engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].temperature < engine.map->tiles[(xM)+(yM)*engine.map->width].temperature)
								//{
								//	engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].temperature ++;
								//	if (engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].envSta != 1)
								//	engine.map->tiles[(xM+i)+(yM+j)*engine.map->width].envSta = 1;
								//}
							}
						}
						//engine.gui->message(TCODColor::red, "tempreature at %d,%d is %d",xM,yM,engine.map->tiles[xM+yM*engine.map->width].temperature);
					}
					 
					 
							
					
						
				}
				else if (engine.map->tiles[xM+yM*engine.map->width].envSta == 2 && engine.map->tiles[xM+yM*engine.map->width].decoration == 0 && engine.map->tiles[xM+yM*engine.map->width].explored)
				{
					srcRect.x = 4 * 16;
					srcRect.y = 0;
					if (engine.map->isInFov(xM,yM)){
						//light
						SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.5);
					}else{
						//dark/*commet*/
						SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.175);
					}
					
					SDL_BlitSurface(fire,&srcRect,floorMap,&dstRect);
					SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.9);
				}
				else if (engine.map->tiles[xM+yM*engine.map->width].envSta == 3 && engine.map->tiles[xM+yM*engine.map->width].explored)
				{//bloody mess
					SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.9);
					srcRect.y = 0;
					if (engine.map->isInFov(xM,yM)){
						//light
						//SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.9);
						srcRect.x = 5 * 16;
					}else{
						//dark/*commet*/
						//SDL_SetAlpha(fire, SDL_SRCALPHA, 255*.5);
						srcRect.x = 6 * 16;
					}
					
					SDL_BlitSurface(fire,&srcRect,floorMap,&dstRect);
					
				}
				
				
			}
			//cout << "fire end" << endl;
			
			
			
			
			
			y++;
		}
		y=0;
		x++;
	}
	//cout << "ending main loop" << endl;
	//fire animating
	slowing++;
	if (slowing%5 == 0)
	fireInt++;
	
	//cout << "beginning shadows" << endl;
	//OPTIMIZE ME?  INCLUDE IN PREVIOUS LOOP?, CAN'T, MUST BE AFTER ALL TILES :(
	int x1 = 0, y1 = 0;
	for (int xM = engine.mapx1; xM < engine.mapx2+16; xM++) {
		for (int yM = engine.mapy1; yM < engine.mapy2+16; yM++) {
			//Human Shadows
			int yN = yM - 1;
			if (engine.mapcon->getChar(xM,yN) == 64 || engine.mapcon->getChar(xM,yN) == 164 || engine.mapcon->getChar(xM,yN) == 148 || engine.mapcon->getChar(xM,yN) == 132)  
			{
				int y2 = y1;
				y2 = y2*16;
				y2 += 16;
				SDL_Rect dstRectOffset={x1*16,y1*16,16,16};
				srcRect.x = 32;
				srcRect.y = 0;
				SDL_BlitSurface(shadows,&srcRect,floorMap,&dstRectOffset);
				
			}
			//flare glow
			else if (engine.mapcon->getChar(xM,yM) == 171)//this is the light itself
			{
				//FlareAi *l = (FlareAi*)engine.getAnyActor(xM,yM);
				//LightAi *light = (LightAi*)l->light->ai;
				//LightAi *l = (LightAi*)engine.getAnyActor(xM,yM)->ai;
				//l->onOff = false;
				//if (l->onOff)
				//{
				SDL_Rect srcRectTemp={0,0,48,48};
				SDL_Rect dstRectTemp={x1*16-16,y1*16-16,48,48};
				SDL_BlitSurface(flareGlow,&srcRectTemp,floorMap,&dstRectTemp);
				static int wobble = 0;
				SDL_Rect srcRectTemp2={160,160+16,16,16};
				SDL_Rect dstRectTemp2={x1*16+wobble,y1*16,16,16};
				SDL_BlitSurface(terminal,&srcRectTemp2,floorMap,&dstRectTemp2);
				static bool DownW = true;
				static int slow = 0;
				if (slow%6 == 0)
				{
					if (wobble > -4 && DownW)
					{
						wobble--;
						if (wobble < -4)
							wobble = -4;
					}
					else if (wobble == -4 && DownW)
					{
						DownW = false;
					}
					else if (wobble < 4)
					{
						wobble++;;
					}
					else if (wobble >= 4)
					{
						DownW = true;
					}
				}	
				slow++;
				//}
			}
			
			
			y1++;
		}
		y1=0;
		x1++;
	}
	//cout << "beginning lights" << endl;
	//flicker lights
	//CAUSES CRASHES IN TUTORIAL LEVEL
	//cout << "running through actors list" << endl;
	for (Actor **it = engine.actors.begin(); it != engine.actors.end(); it++) {
		//cout << "initializing actor" << endl;
		Actor *actor = *it;
		//if (actor->x ==x && actor->y == y) {
		//	return actor;
		//}
		//cout << "processing new light" << endl;
		if (actor->ch == 224){
		//cout << actor->name << endl;
		//cout << "light x  " << actor->x << endl;
		//cout << "player x " << actor->y << endl;
		//cout << "light y  " << engine.player->x << endl;
		//cout << "player y " << engine.player->y << endl;
		//cout << "distance: " << engine.distance(actor->x,engine.player->x,actor->y,engine.player->y) << endl;
		float dist = 11.0;
		//cout << "t/f " << (engine.distance(actor->x,engine.player->x,actor->y,engine.player->y) < dist) << endl;
		//if (engine.map->levelType != Param:TUTORIAL)
		//{
		if (engine.level > 0 && engine.distance(actor->x,engine.player->x,actor->y,engine.player->y) < dist){
			//cout << "initializing light" << endl;
			
			LightAi *l = (LightAi*)actor->ai;
			//cout << "initialized ai" << endl;
			//if (!l->onOff)
			//{
			//	l->onOff = true;
			//	l->update(actor);
			//}
			//TCODRandom *myRandom = new TCODRandom();
			//float rng = myRandom->getFloat(0.0000f,1.0000f);
			//l->flicker(actor,rng);
			TCODRandom *myRandom = new TCODRandom();
			float rng = myRandom->getFloat(0.0000f,1.0000f,0.65000f);
			if(l->flkr < rng || l->onAgn)
			{
				l->onOff = !l->onOff;
				l->update(actor);
				l->onAgn = !l->onAgn;
			}
		}
		}
		//}
		//cout << "done with light" << endl;
		
	}
	
	
/*
	legacy error messages
	engine.gui->message(TCODColor::red, "player x is  %d",plyx);
	engine.gui->message(TCODColor::red, "player y is %d",plyy);
	engine.gui->message(TCODColor::red, "calc player x is  %d",plyx*16);
	engine.gui->message(TCODColor::red, "calc player y is  %d",plyy*16);
*/
	
	
	//SDL_Rect srcRect1={mapx1*16,mapy1*16,(mapx2-mapx1+16)*16,(mapy2-mapy1+16)*16};
	//SDL_BlitSurface(screen,&dstRect1,floorMap,&srcRect1);
	SDL_BlitSurface(screen,&dstRect1,floorMap,NULL);
	
	
	//COULD OPTIMIZE --> CONTAINER/INVENTORY/PLAYER CONTAINS A LIST OF JUST EQUIPMENT TO RENDER, instead of scanning all items
	
	bool MLRTrue = false;
	bool FlmrTrue = false;
	SDL_Rect dstRectEquip={plyx*16,plyy*16,16,16};
	if ((engine.gameStatus == engine.IDLE || engine.gameStatus == engine.NEW_TURN) && engine.armorState == 0){
		for (Actor **it = engine.player->container->inventory.begin();it != engine.player->container->inventory.end();it++)
		{
		
			Actor *a = *it;
			if ((a->pickable->type == Pickable::EQUIPMENT || a->pickable->type == Pickable::WEAPON || a->pickable->type == Pickable::FLAMETHROWER) && ((Equipment*)(a->pickable))->equipped && !engine.player->destructible->isDead())//add case to not blit if inventory is open
			{
				//equipment
				/*if (strcmp(a->name,"Mylar-Lined Boots") == 0)//legacy first thing!
				{
					srcRect.x = 0;
					srcRect.y = 0;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}*/
				//art values for armor
				int artVal = ((Equipment*)(a->pickable))->armorArt;
				switch (artVal)
				{
					case 0:
						break;
					case 1://mylar helmet
						srcRect.x = 0;
						srcRect.y = 3*16;
						break;
					case 2://mylar vest
						srcRect.x = 16;
						srcRect.y = 3*16;
						break;
					case 3://mylar greaves
						srcRect.x = 2*16;
						srcRect.y = 3*16;
						break;
					case 4://mylar boots
						srcRect.x = 3*16;
						srcRect.y = 3*16;
						break;
					case 5://titan helmet
						srcRect.x = 0;
						srcRect.y = 4*16;
						break;
					case 6://titan vest
						srcRect.x = 16;
						srcRect.y = 4*16;
						break;
					case 7://titan greaves
						srcRect.x = 2*16;
						srcRect.y = 4*16;
						break;
					case 8://titan boots
						srcRect.x = 3*16;
						srcRect.y = 4*16;
						break;
					case 9://kevlar helmet
						srcRect.x = 4*16;
						srcRect.y = 3*16;
						break;
					case 10://kevlar vest
						srcRect.x = 5*16;
						srcRect.y = 3*16;
						break;
					case 11://kevlar greaves
						srcRect.x = 6*16;
						srcRect.y = 3*16;
						break;
					case 12://kevlar boots
						srcRect.x = 7*16;
						srcRect.y = 3*16;
						break;
					case 13:
						MLRTrue = true;
						break;
					case 14:
						FlmrTrue = true;
						break;
					default:break;
				}
				if (artVal > 0 && artVal != 13)
				{
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				
				//marine starter stuff
				if (strcmp(a->name,"Marine Fatigue Pants") == 0)
				{
					srcRect.x = 16;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Marine Fatigue Jacket") == 0)
				{
					srcRect.x = 48;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Marine Medical Jacket") == 0)
				{
					srcRect.x = 64;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Marine Quarter-Master Jacket") == 0)
				{
					srcRect.x = 80;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Combat Boots") == 0)
				{
					srcRect.x = 0;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Marine Ballistic Helmet") == 0)
				{
					srcRect.x = 32;
					srcRect.y = 16;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				//explorer starter stuff
				else if (strcmp(a->name,"T-Shirt (red)") == 0)
				{
					srcRect.x = 0;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Boarding Vest") == 0)
				{
					srcRect.x = 16;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Boarding Boots") == 0)
				{
					srcRect.x = 32;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				//mercenary starter stuff
				else if (strcmp(a->name,"Skinsuit Leggings") == 0)
				{
					srcRect.x = 80;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Skinsuit Jacket") == 0)
				{
					srcRect.x = 64;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Balaclava") == 0)
				{
					srcRect.x = 48;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Bruiser Gloves") == 0)
				{
					srcRect.x = 96;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"Tech Helmet") == 0)
				{
					srcRect.x = 112;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name,"T-Shirt (grey)") == 0)
				{
					srcRect.x = 128;
					srcRect.y = 32;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				
				//rando
				/*else if (strcmp(a->name,"Titan-mail") == 0)
				{
					srcRect.x = 32;
					srcRect.y = 0;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}
				else if (strcmp(a->name, "MLR") == 0)
				{
					srcRect.x = 16;
					srcRect.y = 0;
					SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
				}*/
				
			}
		}
	}
	
	if (MLRTrue)
	{
		srcRect.x = 0;
		srcRect.y = 0;
		SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
	}
	if (FlmrTrue)
	{
		srcRect.x = 16;
		srcRect.y = 0;
		SDL_BlitSurface(equipment,&srcRect,floorMap,&dstRectEquip);
	}
	
	SDL_UpdateRect(floorMap, plyx*16, plyy*16, 16, 16);
	
	
	//if the game is idle or new turn, blit onto screen
	if (engine.gameStatus == engine.IDLE || engine.gameStatus == engine.NEW_TURN){
		SDL_BlitSurface(floorMap,NULL,screen,&dstRect1);	
	}
	//set everything to dirty
	TCODConsole::root->setDirty(22*16,0,(engine.mapx2-engine.mapx1)*16+16,(engine.mapy2-engine.mapy1)*16+16);
	//free all surfaces
	SDL_FreeSurface(floorMap);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(serverLight);
	SDL_FreeSurface(fire);
	SDL_FreeSurface(floorTiles);
	SDL_FreeSurface(itemsGlow);
	SDL_FreeSurface(flareGlow);
	SDL_FreeSurface(shadows);
	SDL_FreeSurface(equipment);
	SDL_FreeSurface(terminal);
	SDL_FreeSurface(decor);
	SDL_FreeSurface(flowers);
	SDL_FreeSurface(alcohol);
	
	//engine.menuState = engine.transfer;
	
	}
	else if (engine.menuState == 1)//character screen
	{
		//blitting
		SDL_Surface *character;
		//deciding which to use
		if (engine.player->ch == 143)//human
			character = SDL_LoadBMP("tile_assets/character_screen_char.bmp");
		else if (engine.player->ch == 175)//alien
			character = SDL_LoadBMP("tile_assets/character_screen_char_alien.bmp");
		else
			character = SDL_LoadBMP("tile_assets/character_screen_char_robot.bmp");
		
		
		SDL_SetColorKey(character,SDL_SRCCOLORKEY,255);
		SDL_Rect dstRect={35*16,5*16,400,256};
		SDL_BlitSurface(character,NULL,screen,&dstRect);
		SDL_FreeSurface(character);
		engine.menuState = 2;
		
	}
	else if (engine.menuState == 2)
	{
		//nothing
		
	}
	else if (engine.menuState == 3)//main menu!
	{
		SDL_Surface *highRes = SDL_LoadBMP("titleScreenHiRes.bmp");
		SDL_BlitSurface(highRes,NULL,screen,NULL);
		SDL_FreeSurface(highRes);
		engine.menuState = 2;
		
		
	}
	else if (engine.menuState == 4)//map console
	{
		//engine.gui->message(TCODColor::yellow,"Map Key: light grey = room, dark grey = furnishings, red = player.");
		SDL_Surface *map = SDL_LoadBMP("tile_assets/consoleMap.bmp");
		SDL_Surface *mapPix = SDL_LoadBMP("tile_assets/mapPix.bmp");
		SDL_Surface *mapPixRed = SDL_LoadBMP("tile_assets/mapPixRed.bmp");
		SDL_Surface *mapPixBlue = SDL_LoadBMP("tile_assets/mapPixBlue.bmp");
		SDL_Surface *mapPixDarker = SDL_LoadBMP("tile_assets/mapPixDarker.bmp");
		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 100; y++)
			{
				if (!engine.map->isWall(x,y)){
					if (engine.player->x == x && engine.player->y == y)
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixRed,NULL,map,&dstRect1);
					}
					else if (engine.stairs->x == x && engine.stairs->y == y)//teleporter
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixBlue,NULL,map,&dstRect1);
					}
					else if (engine.map->tiles[x+y*engine.map->width].decoration != 0)
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixDarker,NULL,map,&dstRect1);
					}
					else
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPix,NULL,map,&dstRect1);
					}
					
				}
			}
		}
		
		SDL_Rect dstRect={(engine.screenWidth*16)/2-200,(engine.screenHeight*16)/2-200,400,500};
		SDL_BlitSurface(map,NULL,screen,&dstRect);
		SDL_FreeSurface(map);
		SDL_FreeSurface(mapPix);
		SDL_FreeSurface(mapPixRed);
		SDL_FreeSurface(mapPixBlue);
		SDL_FreeSurface(mapPixDarker);
		engine.menuState = 2;
	}
	else if (engine.menuState == 5)//map from 'm' key (printed map)
	{
		//engine.gui->message(TCODColor::yellow,"Map Key: light grey = room, dark grey = furnishings, red = player.");
		SDL_Surface *map = SDL_LoadBMP("tile_assets/consoleMapPrint.bmp");
		SDL_Surface *mapPix = SDL_LoadBMP("tile_assets/mapPixPrint.bmp");
		SDL_Surface *mapPixDarker = SDL_LoadBMP("tile_assets/mapPixPrintDarker.bmp");
		//SDL_Surface *mapPixRed = SDL_LoadBMP("tile_assets/mapPixRed.bmp");
		//SDL_Surface *mapPixDarker = SDL_LoadBMP("tile_assets/mapPixDarker.bmp");
		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 100; y++)
			{
				if (!engine.map->isWall(x,y)){
					if (engine.map->tiles[x+y*engine.map->width].decoration != 0)
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixDarker,NULL,map,&dstRect1);
					}
					else
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPix,NULL,map,&dstRect1);
					}
				}
				
			}
		}
		
		SDL_Rect dstRect={(engine.screenWidth*16)/2-200,(engine.screenHeight*16)/2-200,400,500};
		SDL_BlitSurface(map,NULL,screen,&dstRect);
		SDL_FreeSurface(map);
		SDL_FreeSurface(mapPix);
		SDL_FreeSurface(mapPixDarker);
		//SDL_FreeSurface(mapPixRed);
		//SDL_FreeSurface(mapPixDarker);
		engine.menuState = 2;
	}
	else if (engine.menuState == 6)//ONBOARD-LOCATOR
	{
		//engine.gui->message(TCODColor::yellow,"Map Key: light grey = room, dark grey = furnishings, red = player.");
		SDL_Surface *map = SDL_LoadBMP("tile_assets/consoleMapLoc.bmp");
		SDL_Surface *mapPix = SDL_LoadBMP("tile_assets/mapPix.bmp");
		SDL_Surface *mapPixRed = SDL_LoadBMP("tile_assets/mapPixRed.bmp");
		SDL_Surface *mapPixDarker = SDL_LoadBMP("tile_assets/mapPixDarker.bmp");
		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 100; y++)
			{
				if (!engine.map->isWall(x,y)){
					if (engine.player->x == x && engine.player->y == y)
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixRed,NULL,map,&dstRect1);
					}
					else if (engine.map->tiles[x+y*engine.map->width].decoration != 0)
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPixDarker,NULL,map,&dstRect1);
					}
					else
					{
						SDL_Rect dstRect1={x*4,y*4,4,4};
						SDL_BlitSurface(mapPix,NULL,map,&dstRect1);
					}
					
				}
			}
		}
		
		SDL_Rect dstRect={(engine.screenWidth*16)/2-200,(engine.screenHeight*16)/2-200,400,500};
		SDL_BlitSurface(map,NULL,screen,&dstRect);
		SDL_FreeSurface(map);
		SDL_FreeSurface(mapPix);
		SDL_FreeSurface(mapPixRed);
		SDL_FreeSurface(mapPixDarker);
		engine.menuState = 2;
	}
	
	
	
	
	}
	//if inventory is open begin animation
	else if (engine.invState == 1)
	{
	TCODSystem::setFps(60);
	//TCODConsole::root->clear();
	engine.invFrames++;
	first=true;
		// set blue(255) as transparent for the root console
		// so that we only blit characters
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,0);
	//SDL_Surface *floorMap = SDL_LoadBMP("starmap2.bmp");
	SDL_Surface *backpack = SDL_LoadBMP("tile_assets/backpack.bmp");
	SDL_SetColorKey(backpack,SDL_SRCCOLORKEY,255);
	SDL_Surface *bg = SDL_LoadBMP("tile_assets/invBG.bmp");
	//SDL_Rect dstRect1={22*16,0,(engine.mapx2-engine.mapx1)*16+16,(engine.mapy2-engine.mapy1)*16+16};
	SDL_Rect dstBack={(engine.screenWidth*16)/2-375,engine.screenHeight*16-48,750,750};
	SDL_Rect dstBack1={0,0,750,750};
	//int second = TCODSystem::getFps();
	if (engine.invFrames > 30)
	{
		//SDL_BlitSurface(screen,NULL,bg,NULL);
		//TCODConsole::flush();
		//SDL_BlitSurface(bg,NULL,screen,NULL);
		engine.invState = 2;
		SDL_FreeSurface(backpack);
		SDL_FreeSurface(bg);
	}
	else
	{
		SDL_BlitSurface(bg,NULL,screen,NULL);
		//TCODConsole::root->clear();
		dstBack.y -= (engine.invFrames*16);
		//dstBack.y = 20;
		SDL_BlitSurface(backpack,&dstBack1,screen,&dstBack);
		SDL_FreeSurface(backpack);
	    SDL_FreeSurface(bg);
	}
	}
	else if (engine.invState == 2)
	{
	TCODSystem::setFps(60);
	//TCODConsole::root->clear();
	engine.invFrames++;
	first=true;
		// set blue(255) as transparent for the root console
		// so that we only blit characters
	SDL_SetColorKey(screen,SDL_SRCCOLORKEY,0);
	//SDL_Surface *floorMap = SDL_LoadBMP("starmap2.bmp");
	SDL_Surface *tab = SDL_LoadBMP("tile_assets/tablet.bmp");
	SDL_SetColorKey(tab,SDL_SRCCOLORKEY,255);
	SDL_Surface *tabBig = SDL_LoadBMP("tile_assets/tablet-big.bmp");
	SDL_SetColorKey(tabBig,SDL_SRCCOLORKEY,255);
	SDL_Surface *backpack = SDL_LoadBMP("tile_assets/backpack.bmp");
	SDL_Surface *flap = SDL_LoadBMP("tile_assets/backpack-flap.bmp");
	SDL_SetColorKey(flap,SDL_SRCCOLORKEY,255);
	SDL_Surface *bg = SDL_LoadBMP("tile_assets/invBG.bmp");
	SDL_SetColorKey(backpack,SDL_SRCCOLORKEY,255);
	//SDL_Rect dstRect1={22*16,0,(engine.mapx2-engine.mapx1)*16+16,(engine.mapy2-engine.mapy1)*16+16};
	SDL_Rect dstBack={(engine.screenWidth*16)/2-375,engine.screenHeight*16-48,750,750};
	SDL_Rect dstBack1={0,0,750,750};
	SDL_Rect dstTab={0,0,250,250};
	SDL_Rect dstTabS={(engine.screenWidth*16)/2-125,250,250,250};
	//int second = TCODSystem::getFps();
	SDL_BlitSurface(bg,NULL,screen,NULL);
	dstBack.y -= (30*16);
	static int y = 0;
	if (engine.invFrames > 60+15)
	{
		//SDL_BlitSurface(screen,NULL,bg,NULL);
		//TCODConsole::flush();
		//SDL_BlitSurface(bg,NULL,screen,NULL);
		engine.invState = 4;
		y = 0;
		SDL_BlitSurface(backpack,&dstBack1,screen,&dstBack);
		SDL_Rect screenTab ={(engine.screenWidth*16)/2-(708/2),48,708,650};
		SDL_BlitSurface(tabBig,NULL,screen,&screenTab);
		SDL_FreeSurface(flap);
		SDL_FreeSurface(backpack);
		SDL_FreeSurface(tab);
		SDL_FreeSurface(tabBig);
		SDL_FreeSurface(bg);
	}
	else if (engine.invFrames > 45)
	{
		
		TCODSystem::setFps(45);
		SDL_BlitSurface(backpack,&dstBack1,screen,&dstBack);
		
		SDL_Rect bigTab    ={0,750-(engine.invFrames-45)*22,708,((engine.invFrames-45)*22)};//(16+((engine.invFrames*16)-45))};
		
		//SDL_Rect screenTab ={(engine.screenWidth*16)/2-(708/2),48,708,750};
		if (((engine.invFrames-45)*22) >= 650)
			{y += 28;}
			
		SDL_Rect screenTab ={(engine.screenWidth*16)/2-(708/2),y,708,650};
		
		SDL_BlitSurface(tabBig,&bigTab,screen,&screenTab);
		
		SDL_FreeSurface(flap);
		SDL_FreeSurface(backpack);
		SDL_FreeSurface(tab);
		SDL_FreeSurface(tabBig);
		SDL_FreeSurface(bg);
		
	}
	else
	{
		TCODSystem::setFps(45);
		//TCODConsole::root->clear();
		
		dstTabS.y -= (engine.invFrames-30)*25;
		//dstBack.y = 20;
		SDL_BlitSurface(backpack,&dstBack1,screen,&dstBack);
		SDL_BlitSurface(tab,&dstTab,screen,&dstTabS);
		SDL_BlitSurface(flap,&dstBack1,screen,&dstBack);
		SDL_FreeSurface(flap);
		SDL_FreeSurface(backpack);
		SDL_FreeSurface(tab);
		SDL_FreeSurface(tabBig);
		SDL_FreeSurface(bg);
		
	}
	}
	else if (engine.invState == 4)
	{
		TCODSystem::setFps(60);
		TCODConsole::root->clear();
		//SDL_SetColorKey(screen,SDL_SRCCOLORKEY,0);
		first=true;
		//static bool Rend = true;
		//SDL_Rect dstBack={(engine.screenWidth*16)/2-375,(engine.screenHeight*16-48)-(30*16),750,750};
		//SDL_Rect dstBack1={0,0,750,750};
		
		SDL_Surface *tabBig = SDL_LoadBMP("tile_assets/tablet-big.bmp");
		SDL_Surface *bg = SDL_LoadBMP("tile_assets/invBG.bmp");
		//SDL_Surface *pointer = SDL_LoadBMP("tile_assets/finger.bmp");
		SDL_SetColorKey(tabBig,SDL_SRCCOLORKEY,255);
		//SDL_SetColorKey(pointer,SDL_SRCCOLORKEY,255);
		//SDL_Surface *bg = SDL_LoadBMP("tile_assets/invBG.bmp");
		SDL_SetColorKey(screen,SDL_SRCCOLORKEY,0);
		SDL_Rect screenTab ={(engine.screenWidth*16)/2-(708/2),48,708,650};
		SDL_Rect screenTab1 ={(engine.screenWidth*16)/2-(708/2),48,708,650};//only the screen itself
		
		//SDL_Surface *backpack = SDL_LoadBMP("tile_assets/backpack.bmp");
		//SDL_SetColorKey(backpack,SDL_SRCCOLORKEY,255);
		
		
		
		
		//SDL_UpdateRect(bg, 0, 0, 0,0);
		
		/*static int oldX = 0;
		static int oldY = 0;
		int x = engine.selX*16;
		int y = engine.selY*16;
		
		if (oldX != x || oldY != y)
		{
			//TCODConosle *temp = new TCODConsole(85,44);
			TCODConsole::blit(TCODConsole::root, 0, 0, 85, 44, engine.temporary , 0,0);
			
			SDL_Surface *tempy =(SDL_Surface *)engine.temporary;
			SDL_SetColorKey(tempy,SDL_SRCCOLORKEY,0);
			SDL_BlitSurface(tempy,&screenTab,tabBig,NULL);
			SDL_BlitSurface(backpack,&dstBack1,bg,&dstBack);
			SDL_BlitSurface(bg,NULL,screen,NULL);
			
			SDL_BlitSurface(tabBig,NULL,screen,&screenTab);
			oldX = x;
			oldY = y;
		}*/
		//
		
		//
		static bool frst = true;
		if (frst)
		{
			SDL_BlitSurface(screen,&screenTab1,tabBig,NULL);
		}
		else
		{
			frst = false;
			//TCODConsole::flush();
		}
		SDL_BlitSurface(bg,NULL,screen,NULL);
		SDL_BlitSurface(tabBig,NULL,screen,&screenTab);
		//SDL_UpdateRect(screen, 0, 0, 85*16,35*16);
		//TCODConsole::root->setDefaultBackground(TCODColor::black);
		//TCODConsole::root->setDefaultBackground(TCODColor::blue);
		//TCODConsole::root->clear();
		//engine.gui->render();
		//SDL_BlitSurface(screen,NULL,bg,NULL);
		
		//SDL_Rect pointerBox ={x,y,375,800};
		//SDL_BlitSurface(pointer,NULL,screen,&pointerBox);
		
		
		//SDL_BlitSurface(bg,NULL,screen,NULL);
		SDL_FreeSurface(bg);
		SDL_FreeSurface(tabBig);
		//SDL_FreeSurface(backpack);
		//SDL_FreeSurface(pointer);
	}
	else if (engine.invState == 5)
	{
		static int cnt = 0;
		cnt++;
		//TCODConsole::flush();
		if (cnt == 5)
		{
			engine.invState = 4;
		}
	}
	
}
	

