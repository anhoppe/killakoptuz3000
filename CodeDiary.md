CommanderZZ Code Blog - You will love it

# Introduction #

Here we present our development diary. Some sort of improved bible in the future.


# Details #

We started in 22th of april 2007 (new era 0, a.k.k.), this blog is created on 27th of may 2007 (or, as we like it, 0:13, 5. Feb 0 a.k.k.). You get it, pal?

## Sat/sun 26/27 May 2007 ##
Kyra (drained 1.5h with esoteric mambo jambo), Hoppe, Fabian


  * Added explosions to player and objects, first time we and the enemy (a nice little truck) exploded.
  * Created/found new problems with collision
  * Added shots to the player
  * When many objects are present the game becomes veeery slow. We'll have to improve that!


## Sun, 27 May 2007 (Night) ##
Hoppe, was still in the mood for ~0:45h

  * An object does not only rotate about it's own angle, but also around the parent angle, if it has a parent. That must still be done for the collision polygon

Fabian too...

  * CEnemy's can not collide with CObject's anymore, which allows them to drive on the ground

## Sun, 27 May 2007 ##
Fabian has done some small things after swimming in the Werdersee:

  * Damage model will be applied for all objects
  * Speed of explosion depends on object hitpoints. The more hitpoints the more buum. Formula is inspired by [michaelis - menten](http://en.wikipedia.org/wiki/Michaelis-Menten_kinetics) enzyme kinetics.

## Sun, 27 May 2007 ##
Hoppe: Sunday, cruddy sunday...

  * I did stuff on the player gun: it stays attached to the player accuratly. The shots have a correct angle and start position (even when heli is flipped).

## Mon, 28 May 2007 ##
Fabian: in the morning nebula...

  * The player gun poly and rect are now rotated and flipped correctly
  * Discovered a bad thing: When two shots hit each other, the poly disappears, because both use the same polygon... (in CObject::isCollided) Ahhh. Possible solution:
> If (a\_polygonAPtr == a\_polygonBPtr) make a working copy of one... the code doesent get nicer by that. Tried it. It is too slow, because this case happens so often... Probably we have to give each object it's own polygon back.

## Sat/Sun, 2/3 Jun 2007 ##
Hoppe and Fabian: Some disturbance by Meike or Melanie (says Hoppe), Willy and other things.

  * Started to give each object it's polygon back
  * Lots of prob's... unsolved at 2:39... Puh!
  * Discovered (and were unabe to solve until 3:41) strange collision detection bug. Polygons seem always to collide, when box test is positive. Why????????????

Hoppe: after writing this text we considered all over and decided that this is the optimal time point for refactoring. Stuff became too difficult, we have to sort out things. My first proposal: Make a storage facility for objects in game class. Yeah, a game class proposed by Fabs. By the way, we all are a bit over the edge right now. Sitting with us are Flix & Willi.

**Commanderz market letter (06/2007):** _We hereby predict that at the end of next week the DAX will point downwards with a value < 8000. This is based on the unusual high slope of the DAX which will result in profit taking actions._

## Mon, 4 Jun 2007 ##

Hoppe and Fabian: Hoppe was a bit tired this day and Fabian has to get up early tomorrow, so this day we did not add too much code. Here some details
  * We thought about the key issues in the code for a redesign
  * Implemented CGame as the game flow control class. a function here is responible for assigning function pointers to glut methods. So Level etc. can have own update/render functions
  * Started on the Objcet Storage class that holds all objects, updates them and checks collision. Those taks create game events with are processed, too.
  * We want to check collision with a quad tree. Fabs showed some web pages about that stuff and we think we will try it out.

## Tue, 5 Jun 2007 ##

Hoppe & Fabian, Proceeded only a little, due to guests at Hoppes house and general lack of evening spare time:
  * Object Storage class (CObjectStorage) was developed a little further.
  * Object loading via xml file was polished.

## Fr/Sa, 8/9 June 2007 ##

Hoppe & Fabian without disturbance (except themself). Both a bit tired again (or as always). Birthday of M., Fabians sister.

  * Cleaned up loader methods in object classes. Everthing looks so clean and easy now.
  * Started to compile code, Could not finish through a lack of competency with function pointers. Really hard stuff.

## Sun, 10. June 2007 ##

Hoppe & Fabian, without interruption (only Flo complained about too annoying guests one time). Fabian came around 0:00, so this session counts as sunday, not saturday. I had a very lazy day (at least I cleand up my room), Fabian was on a birthday party and could play PS3 on a very big high def plasma TV. Great times!

  * KK3000 runs again (yeeaahh). Fixed remaining loading problems
  * restored players ability to shoot (enemy can't, no collision yet!)

We discovered that std::map::operator[.md](.md) adds an element to the hash table, not std::map::operator=. Or at least both. Kind of disturbing...

## Mon, 11. June 2007 ##

H & F. Fabs typed. Hoppe was lazy (cooking and so on):

  * Tracking for weapons works now (again..., but much better)
  * CEvent class for events
  * Shots generate a delete event when out of boundary.
  * Events are processed

## Tue / Wed, 12./13. June 2007 ##

Fabian: due to lack of sleeping ablility, worked on a real hashtable (std::map is some red black tree and NOT a hashtable as we thought). Hoppe supported me via Skype (Cleaned up my template mess).

  * CHashMap created. Is not used by the CObjectStorage class yet. Is supposed to replace std::map there.

Method for iteration needs still to be done... Tmrw... Now: 1:08am. Have to get up at 6:30.

Fabian (June, 13th. afternoon):

  * CHashMap has now a method to iterate the items. Seems to work, but has to be tested more precisely before KK3000 use... Have to go now.

## Thu, 14. June 2007 ##

Fabian, early at home due to hangover from yesterday. Afternoon proggin':

  * CObjectStorage::m\_objectMap is now using CHashMap. Works! Woha!!

Hoppe, Fabian, later this evening

  * Implemented CObjectStorage::m\_drawList and introduced draw layers to the objects and the XML description. We hope to solve the draw order problem once and for all.

## Fri, 15. June 2007 (Night) ##

H & F. Implemented the quad tree. Problems could not be vaporized... Some objects were inserted the wrong way. Hoppe implemented everyting and later on visualized the 50% working Quadtree - looks fancy. No remarkable slowdown of the execution speed at all. Fabian was brilliant in falling asleep. Watched a movie (Group of hot bikini girls in brazil were used by some very evil Dr. as organ donors).

  * CQuadTree implemented. Not fully working yet.

## Sun, 17. June 2007 ##

H & F, Michi L. as guest star.
Very informative for our guest star who gained massive C++ knowledge.
Otherwise as usual...
We reached the same state we had before the re-implementation phase.

  * Fixed the Quad Tree
  * Children are deleted as soon as parent start dying
  * Worked on game control, added menu class
  * Game starts with menu now! After you've been killed, you get back to the main menu
  * Added menu class CMenu and CMenuItem

## Fri, 22. June 2007 ##

H & F. Willy. A litte flabby evening. Project proceeds, but we really need a kick.

  * Kollision works now 100%
  * Hoppe has started a Level Editor
  * Menus are Texture based now

## Sun, 24. June 2007 ##

Hoppe after visiting my grandmother before playing Angband (Level 42 now!!!)
  * Proceeded with the level editor, started property grid

## Mo, 25. June 2007 ##

Of course motivation is not the tiniest problem for members of The CommanderZZ! To proof this to ourselfs we (especially Fab) integrated ballistic shots. It really kicks ass!
  * Shots have a type (normal, ballistic, guided is planned)
  * All sprites have a velocity (we believe so)
  * Code becomes a bit ... ummm ... sort of ugly again

## Di, 26 June 2007 ##

Hop: Motivated by Fab via Skype I continued programming on the level editor, otherwise I would have played Angband for the whole time (Level 44 now). Maybe I call it LE3000?
I don't know if I have mentioned it yet, but the LE provides a control panel to the left that holds the control and the property sheet for the objects and a GL panel for outout to the right. It is developed again in C++ with wxWidgets.

  * Fixed the wxSizer settings for control panel
  * stored object properties in the property sheet
  * if values are altered in the property sheet, the object is updated

The existing code was added to our repository today

## Di., 4. July 2007 ##

Hop: after a great party weekend at the Fusion Festival in east germany I am back in business. Continued on the LE3000 project.
  * LE3000 is compilable again
  * tga files are addable and selectable, altough selected them has no effect yet

## Sun., 8. July 2007 ##

Hop: Fab wasn't in town this weekend and I had to do some other stuff for a certain firm... But I had a bit time to work on LE3000 after a regular visit to my grandmother.
  * an added object can select it's texture.

The code is terrible. Still very buggy (only the first object can select a texture). But the success of seeing an added object with a texture was cool.

## Sun., 25. Jan 2009 ##

The project lives!!! We decided to re begin working KK3000! Fabs (With a little hangover from H's birthdayparty): Started to polish the google code project page a little.

## Thu., 29. Jan 2009 ##

Fabs: added sound effects. Each weapon has its own fireing sound and each object makes an explosion sound. It's all editable via the xml file...

Implementation of hostages is the next on the list.

## Fri., 30. Jan 2009 ##

Fabs: added lots of minor improvements... The chain menu -> game -> game over -> menu does work now. Ronald added two new songs!