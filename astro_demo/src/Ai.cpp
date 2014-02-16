#include <stdio.h>
#include <math.h>
#include "main.hpp"

Ai *Ai::create(TCODZip &zip) {
	AiType type = (AiType)zip.getInt();
	Ai *ai = NULL;
	switch(type) {
		case PLAYER: ai = new PlayerAi(); break;
		case MONSTER: ai = new MonsterAi(); break;
		case CONFUSED_ACTOR: ai = new ConfusedActorAi(0,NULL); break;
	    case EPICENTER: ai = new EpicenterAi(); break;
		case RANGED: ai = new RangedAi(); break;
	}
	ai->load(zip);
	return ai;
}

PlayerAi::PlayerAi() : xpLevel(1) {
}

const int LEVEL_UP_BASE = 20;
const int LEVEL_UP_FACTOR = 15;

int PlayerAi::getNextLevelXp() {
	return LEVEL_UP_BASE + xpLevel * LEVEL_UP_FACTOR;
}
	
void PlayerAi::load(TCODZip &zip) {
	xpLevel = zip.getInt();
}

void PlayerAi::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	zip.putInt(xpLevel);
}

void PlayerAi::update(Actor *owner) {
	int levelUpXp = getNextLevelXp();
	if (owner->destructible->xp >= levelUpXp) {
		bool choice_made = false, first = true;
		while (!choice_made) {
		if (first) {
			engine.gui->message(TCODColor::yellow,"As the monster falls to the floor, you feel yourself filled with some\n unknown strength!");
			TCODConsole::flush();
			xpLevel++;
			owner->destructible->xp -= levelUpXp;
		}
		engine.gui->menu.clear();
		engine.gui->menu.addItem(Menu::CONSTITUTION, "Constitution (+20hp)");
		engine.gui->menu.addItem(Menu::STRENGTH, "Strength (+1 attack)");
		//engine.gui->menu.addItem(Menu::AGILITY, "Agility (+1 defence)");
		Menu::MenuItemCode menuItem = engine.gui->menu.pick(Menu::PAUSE);
	
		switch (menuItem) {
			case Menu::CONSTITUTION	:
				owner->destructible->maxHp += 20;
				owner->destructible->hp += 20;
				choice_made = true;
				break;
			case Menu::STRENGTH :
				owner->attacker->basePower += 1;
				owner->attacker->totalPower += 1;
				choice_made = true;
				break;
			case Menu::AGILITY:
				owner->destructible->baseDefense += 1;
				owner->destructible->totalDefense += 1;
			/*case Menu::AGILITY:
				owner->destructible->defense += 1;
				choice_made = true;
				break;*/
			case Menu::NO_CHOICE:
				first = false;
				break;
			default: break;
		}
		}
	}
	if (owner->destructible && owner->destructible->isDead()) {
		return;
	}
	int dx =0, dy =0;
	switch (engine.lastKey.vk) {
	case TCODK_UP: case TCODK_KP8: dy = -1; break;
	case TCODK_DOWN: case TCODK_KP2: dy = 1; break;
	case TCODK_LEFT: case TCODK_KP4: dx =-1; break;
	case TCODK_RIGHT: case TCODK_KP6: dx = 1; break;
	case TCODK_KP7: dy = dx = -1; break;
	case TCODK_KP9: dy = -1; dx = 1; break;
	case TCODK_KP1: dx = -1; dy = 1; break;
	case TCODK_KP3: dx = dy = 1; break;
	case TCODK_TAB: 
		engine.save();
		engine.gui->message(TCODColor::pink, "saved"); break;
	case TCODK_CONTROL: 
		engine.player->x = engine.stairs->x;
		engine.player->y = engine.stairs->y;
		engine.map->computeFov(); break;
		
	case TCODK_PRINTSCREEN:
		TCODSystem::saveScreenshot(NULL);
		engine.gui->message(TCODColor::orange,"screenshot saved"); break;
	case TCODK_KP5: engine.map->computeFov(); engine.gameStatus = Engine::NEW_TURN; break;
	case TCODK_CHAR: handleActionKey(owner, engine.lastKey.c); break;
	default: break;
	}
	if (dx != 0 || dy != 0) {
		engine.gameStatus = Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x+dx, owner->y+dy)) {
			engine.map->computeFov();
		}
	}
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety) {
	if (engine.map->isWall(targetx, targety) ) return false;
	if (!owner->attacker) return false;
	
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->blocks && actor->x == targetx &&actor->y == targety) {
			if (actor->destructible && !actor->destructible->isDead()) {
				owner->attacker->attack(owner, actor);
			}
			return false;
		}
	}
	/* //display everything you step over
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		bool corpseOrItem = (actor->destructible && actor->destructible->isDead())
			|| actor->pickable;
			
		if (corpseOrItem
			&& actor->x == targetx && actor->y == targety) {
			engine.gui->message(TCODColor::white,"There's a %s here\n", actor->name);
		}
		
	} */
	owner->x = targetx;
	owner->y = targety;
	return true;
}

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
	switch(ascii) {
		case 'g': //pickup the item
		{
			engine.map->computeFov();
			bool found = false;
			for (Actor **iterator = engine.actors.begin();
				iterator != engine.actors.end(); iterator++) {
				Actor *actor = *iterator;
				if (actor->pickable && actor->x == owner->x && actor->y == owner->y) {
					if (actor->pickable->pick(actor,owner)) {
						found = true;
						engine.gui->message(TCODColor::lightGrey, "You pick up the %s.", actor->name);
						break;
					} else if (!found ) {
						found = true;
						engine.gui->message(TCODColor::red, "Your inventory is full.");
					}
				}
			}
			if (!found) {
				engine.gui->message(TCODColor::lightGrey,"There's nothing interesting here.");
			}
			if (engine.gameStatus != Engine::VICTORY) {
				engine.gameStatus = Engine::NEW_TURN;
			}
		}
		break;
		case 'i': //display inventory
		{ 
			engine.map->computeFov();
			engine.gui->menu.clear();
			engine.gui->menu.addItem(Menu::ITEMS, "Items");
			engine.gui->menu.addItem(Menu::TECH, "Tech");
			engine.gui->menu.addItem(Menu::ARMOR, "Armor");
			engine.gui->menu.addItem(Menu::WEAPONS, "Weapons");
			engine.gui->menu.addItem(Menu::EXIT, "Exit");
			//Menu::MenuItemCode menuItem = engine.gui->menu.pick(Menu::INVENTORY);
			Actor *actor;
			bool choice = true;
			while(choice){
			Menu::MenuItemCode menuItem = engine.gui->menu.pick(Menu::INVENTORY);
				switch (menuItem) {
					case Menu::ITEMS :
						actor = choseFromInventory(owner,1);
						if(actor)
							choice = false;
						break;
					case Menu::TECH :
						actor = choseFromInventory(owner,2);
						if(actor)
							choice = false;
						break;
					case Menu::ARMOR:
						actor = choseFromInventory(owner,3);
						if(actor)
							choice = false;
						break;
					case Menu::WEAPONS:
						actor = choseFromInventory(owner,4);
						if(actor)
							choice = false;
						break;
					case Menu::NO_CHOICE:
						break;
					case Menu::EXIT:
						choice = false;
					default: break;
				}
			}
			if (actor) {
				bool used;
				used = actor->pickable->use(actor,owner);
				if (used) {
					engine.gameStatus = Engine::NEW_TURN;
				}
			}
		}break;
		case 'd': //drop an item
		{
			engine.map->computeFov();
			engine.gui->menu.clear();
			engine.gui->menu.addItem(Menu::ITEMS, "Items");
			engine.gui->menu.addItem(Menu::TECH, "Tech");
			engine.gui->menu.addItem(Menu::ARMOR, "Armor");
			engine.gui->menu.addItem(Menu::WEAPONS, "Weapons");
			engine.gui->menu.addItem(Menu::EXIT, "Exit");
			//Menu::MenuItemCode menuItem = engine.gui->menu.pick(Menu::INVENTORY);
			Actor *actor;
			bool choice = true;
			while(choice){
			Menu::MenuItemCode menuItem = engine.gui->menu.pick(Menu::INVENTORY);
				switch (menuItem) {
					case Menu::ITEMS :
						actor = choseFromInventory(owner,1);
						if(actor)
							choice = false;
						break;
					case Menu::TECH :
						actor = choseFromInventory(owner,2);
						if(actor)
							choice = false;
						break;
					case Menu::ARMOR:
						actor = choseFromInventory(owner,3);
						if(actor)
							choice = false;
						break;
					case Menu::WEAPONS:
						actor = choseFromInventory(owner,4);
						if(actor)
							choice = false;
						break;
					case Menu::NO_CHOICE:
						break;
					case Menu::EXIT:
						choice = false;
					default: break;
				}
			}
			if (actor) {
				actor->pickable->drop(actor,owner);
				engine.gameStatus = Engine::NEW_TURN;
			}
		}break;
		case 'l':
		{
			engine.map->computeFov();
			engine.gui->renderKeyLook();
		}break;
		case '>':
		if (engine.stairs->x == owner->x && engine.stairs->y == owner->y) {
			engine.player->attacker->lastTarget = NULL;
			engine.nextLevel();
		} else {
			engine.gui->message(TCODColor::lightGrey, "There are no stairs here. Perhaps you are disoriented?");
		} break;
		case 'v':
		int w, h;
		if (!TCODConsole::isFullscreen()){
			TCODSystem::getCurrentResolution(&w,&h);
			TCODConsole::initRoot(w/16,h/16,"Astro",true);
		} else {
			engine.gui->message(TCODColor::darkerPink,"minimizing");
			TCODConsole::initRoot(engine.screenWidth,engine.screenHeight, "Astro", false);
		}
		break;
		case 'f':
			//shooty shooty bang bang -Mitchell
			//need to figure out how to check if the user has a gun
			if(owner->container->ranged){
				//engine.gui->message(TCODColor::darkerOrange,"You fire your MLR");
				Actor *closestMonster = engine.getClosestMonster(owner->x, owner->y,3);
				if (!closestMonster) {
					engine.gui->message(TCODColor::lightGrey, "No enemy is close enough to shoot.");
					//return false;
				}
				//hit the closest monster for <damage> hit points;
				else{
					if(false){
						/////////TO-DO Battery
					}
					else{
						owner->attacker->shoot(owner, closestMonster);
						engine.gameStatus = Engine::NEW_TURN;
					}
				}
				/*
				float damageTaken = closestMonster->destructible->takeDamage(closestMonster,damage);
				if (!closestMonster->destructible->isDead()) {
				engine.gui->message(TCODColor::lightBlue,
					"A lightning bolt strikes the %s with a loud crack"
					"for %g damage.",
					closestMonster->name,damageTaken);
				} else {
					engine.gui->message(TCODColor::orange,"The %s crackles with electricity, twitching slightly.",closestMonster->name);
				}
				return Pickable::use(owner,wearer);
				*/
			}
			else{
				engine.gui->message(TCODColor::lightGrey,"You do not have a ranged weapon equipped");
			}
		break;
	}
}

Actor *PlayerAi::choseFromInventory(Actor *owner,int type) {
	static const int INVENTORY_WIDTH = 38;
	static const int INVENTORY_HEIGHT = 28;
	static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	
	//display the inventory frame
	con.setDefaultForeground(TCODColor(200,180,50));
	con.printFrame(0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,true,TCOD_BKGND_DEFAULT);
	
	//display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int shortcut = 'a';
	int y = 1;
	for (Actor **it = owner->container->inventory.begin();
		it != owner->container->inventory.end(); it++) {
		Actor *actor = *it;
		if(actor->sort == type){
			if(actor->pickable->type == Pickable::EQUIPMENT && ((Equipment*)(actor->pickable))->equipped){
				con.print(2,y,"(%c) %s(E)",shortcut,actor->name);
			}else{
				con.print(2,y,"(%c) %s",shortcut,actor->name);
			}
			owner->container->select[shortcut] = actor->name;
			if (actor->pickable->stacks) {
				con.print(17, y, "(%d)",actor->pickable->stackSize);
			}
			y++;
			shortcut++;
		}
	}
	//blit the inventory console on the root console
	TCODConsole::blit(&con,0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,
		TCODConsole::root, engine.screenWidth/2 - INVENTORY_WIDTH/2,
		engine.screenHeight/2 - INVENTORY_HEIGHT/2 - 4);
	TCODConsole::flush();
	
	//wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	
	if (key.vk == TCODK_CHAR) {
		int actorIndex = 0;
		if(owner->container->select[key.c]){
			for(Actor **it = owner->container->inventory.begin(); it != owner->container->inventory.end(); ++it){
				Actor *actor = *it;
					if(strcmp(actor->name,owner->container->select[key.c]) == 0){
						return owner->container->inventory.get(actorIndex);
					}
				actorIndex++;
			}
		}
	}
	return NULL;
}


static const int TRACKING_TURNS = 3;

MonsterAi::MonsterAi() : moveCount(0) {
}

void MonsterAi::load(TCODZip &zip) {
	moveCount = zip.getInt();
}

void MonsterAi::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	zip.putInt(moveCount);
}

void MonsterAi::update(Actor *owner) {
	if (owner->destructible && owner->destructible->isDead()) {
		return;
	}
	if (engine.map->isInFov(owner->x,owner->y)) {
		//can see the palyer, move towards him
		moveCount = TRACKING_TURNS;
	} else {
		moveCount--;
	}
	if (moveCount > 0) {
		moveOrAttack(owner, engine.player->x, engine.player->y);
	} else {
		moveCount = 0;
	}
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety){
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1:-1);
	int stepdy = (dy > 0 ? 1:-1);
	float distance = sqrtf(dx*dx+dy*dy);

	if (distance >= 2) {
		dx = (int) (round(dx / distance));
		dy = (int)(round(dy / distance));
		if (engine.map->canWalk(owner->x+dx,owner->y+dy)) {
			owner->x+=dx;
			owner->y+=dy;
		} else if (engine.map->canWalk(owner->x+stepdx,owner->y)) {
			owner->x += stepdx;
		} else if (engine.map->canWalk(owner->x,owner->y+stepdy)) {
			owner->y += stepdy;
		}
		if (owner->oozing) {
			engine.map->infectFloor(owner->x, owner->y);
		}
	} else if (owner->attacker) {
		owner->attacker->attack(owner,engine.player);
	}
	
}

EpicenterAi::EpicenterAi() {
}

void EpicenterAi::load(TCODZip &zip) {
}

void EpicenterAi::update(Actor *owner) {
	if (engine.turnCount % 500 == 0) {
		infectLevel(owner);
	}
}

void EpicenterAi::infectLevel(Actor *owner) {
	int width = engine.map->width;
	int height = engine.map->height;
	TCODRandom *rng = TCODRandom::getInstance();

	for (int i = 0; i < width*height; i++) {
		engine.map->tiles[i].infection += 1 / (rng->getDouble(.01,1.0)*owner->getDistance(i%width, i/width));
	}
	engine.gui->message(TCODColor::green,"You feel uneasy as the infection seems to spread.");
}

void EpicenterAi::save(TCODZip &zip) {
	zip.putInt(EPICENTER);
}

ConfusedActorAi::ConfusedActorAi(int nbTurns, Ai *oldAi)
	:nbTurns(nbTurns),oldAi(oldAi) {
}

void ConfusedActorAi::load(TCODZip &zip) {
	nbTurns = zip.getInt();
	oldAi = Ai::create(zip);
}

void ConfusedActorAi::save(TCODZip &zip) {
	zip.putInt(CONFUSED_ACTOR);
	zip.putInt(nbTurns);
	oldAi->save(zip);
}

void ConfusedActorAi::update(Actor *owner) {
	if (owner->destructible && !owner->destructible->isDead() ) {
		TCODRandom *rng = TCODRandom::getInstance();
		int dx = rng->getInt(-1,1);
		int dy = rng->getInt(-1,1);
	
		if (dx != 0 || dy!=0) {
			int destx = owner->x + dx;
			int desty = owner->y + dy;
			if (engine.map->canWalk(destx,desty)) {
				owner->x = destx;
				owner->y = desty;
				engine.map->computeFov();
			} else {
				Actor *actor = engine.getActor(destx, desty);
				if (actor) {
					owner->attacker->attack(owner,actor);
					engine.map->computeFov();
				}
			}
		}
	}
	nbTurns--;
	
	if(nbTurns == 0) {
		owner->ai = oldAi;
		delete this;
	}
		if (owner == engine.player) {
		engine.gameStatus = Engine::NEW_TURN;
		return;
	}
}

RangedAi::RangedAi() : moveCount(0), range(3){
}

void RangedAi::load(TCODZip &zip) {
	moveCount = zip.getInt();
	range = zip.getInt();
}

void RangedAi::save(TCODZip &zip) {
	zip.putInt(RANGED);
	zip.putInt(moveCount);
	zip.putInt(range);
}

void RangedAi::update(Actor *owner) {
	if (owner->destructible && owner->destructible->isDead()) {
		return;
	}
	if (engine.map->isInFov(owner->x,owner->y)) {
		//can see the palyer, move towards him
		moveCount = TRACKING_TURNS + 2; //give ranged characters longer tracking
	} else {
		moveCount--;
	}
	if (moveCount > 0) {
		moveOrAttack(owner, engine.player->x, engine.player->y);
	} else {
		moveCount = 0;
	}
}
void RangedAi::moveOrAttack(Actor *owner, int targetx, int targety)
{
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1:-1);
	int stepdy = (dy > 0 ? 1:-1);
	float distance = sqrtf(dx*dx+dy*dy);
	//If the distance > range, then the rangedAi will move towards the player
	//If the distance <= range, then the rangedAi will shoot the player unless the player is right next the rangedAi

	if (distance > range) {

		dx = (int) (round(dx / distance));
		dy = (int)(round(dy / distance));
		if (engine.map->canWalk(owner->x+dx,owner->y+dy)) {
			owner->x+=dx;
			owner->y+=dy;
		} else if (engine.map->canWalk(owner->x+stepdx,owner->y)) {
			owner->x += stepdx;
		} else if (engine.map->canWalk(owner->x,owner->y+stepdy)) {
			owner->y += stepdy;
		}
		if (owner->oozing) {
			engine.map->infectFloor(owner->x, owner->y);
		}
	} else if (distance !=1 && owner->attacker) {
		owner->attacker->shoot(owner,engine.player);
	}
	else if (owner->attacker) {
		owner->attacker->attack(owner,engine.player);
	}
	
}

