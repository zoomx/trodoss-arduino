#include <avr/pgmspace.h>
#include <Gamby.h>
#include "monster.h"
#include "map.h"
#include "map_bitmap.h"
#include "monster_bitmap.h"
#include "room.h"

extern GambyGraphicsMode gamby;

#define STEP_LENGTH  4
#define SIZEOF_MONSTER_RECORD 10

char changeMonsterDirection()
{
  return ((random() % 4) + STATE_MOVE_UP);
}

RoomElement moveMonster(RoomElement element)
{
  //draw new monster bitmap
  if (element.state > STATE_HIDDEN)
  {
    //erase the old monster image (using blank map tile)
    gamby.drawSprite(element.x, element.y, map_bitmap);
    gamby.drawSprite(element.x, element.y + 8, map_bitmap);
  
    element.step++;
    if (element.step > 2) element.step = 1;
   
    switch (element.state)
    {
       case STATE_VISIBLE:
	    element.state = changeMonsterDirection();
		break;
	 
       case STATE_MOVE_UP:
           element.state = STATE_VISIBLE;
           if (element.y > 4)
           {
             if (checkMapRoomMove(element.x, element.y - 4) == 0)
             {
                if (checkMapRoomMove(element.x + 4, element.y - 4) == 0)
                {
                  element.y -= STEP_LENGTH;
                   element.state = STATE_MOVE_UP;
                }
             }
           }
	   break;
	   
	  case STATE_MOVE_DOWN:
           element.state = STATE_VISIBLE;
           if (element.y < 48)
           {
             if (checkMapRoomMove(element.x, element.y + 16) == 0)
             {
                if (checkMapRoomMove(element.x + 4, element.y + 16) == 0)
                {
                  element.y += STEP_LENGTH;
                  element.state = STATE_MOVE_DOWN;
                }
             } 
           }
	   break;	   

	  case STATE_MOVE_LEFT:
           element.state = STATE_VISIBLE;
           if (element.x > 4)
           {
             if (checkMapRoomMove(element.x - 4, element.y) == 0)
             {
                if (checkMapRoomMove(element.x - 4, element.y + 12) == 0)
                {
                    element.x -= STEP_LENGTH;
                    element.state = STATE_MOVE_LEFT;
                }
             }
           }
	   break;	   
	   
	  case STATE_MOVE_RIGHT:
           element.state = STATE_VISIBLE;
           if (element.x < 80)
           {
             if (checkMapRoomMove(element.x + 12, element.y) == 0)
             {
                if (checkMapRoomMove(element.x + 12, element.y + 12) == 0)
                {
                  element.x += STEP_LENGTH;
                  element.state = STATE_MOVE_RIGHT;
                }
             }
           }
	   break;		      
    }

    gamby.drawSprite(element.x, element.y, monster_bitmap, element.type );
    gamby.drawSprite(element.x, element.y+8, monster_bitmap, (element.type + element.step)); 
	
	//decrement counter, if active
	if (element.counter > 0) element.counter--;
  }
  return element;
}

RoomElement hitMonster(RoomElement element)
{
  //erase the old monster image (using blank map tile)
  gamby.drawSprite(element.x, element.y, map_bitmap);
  gamby.drawSprite(element.x, element.y + 8, map_bitmap);
  
  element.state = STATE_HIDDEN;
  
  //add heart element
  addRoomElement(ITEM_HEART, element.x, element.y, STATE_VISIBLE, COUNTER_START);
  
  return element;
}



