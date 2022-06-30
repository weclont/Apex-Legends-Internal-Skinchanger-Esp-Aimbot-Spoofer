#include <iostream>
#include <Windows.h>

#include "Memory.h"
#include "Structs.h"
#include "Drawing.h"
#include "Offsets.h"

using namespace std;

int ScreenWidth = 1920;
int ScreenHeight = 1080;

bool Aimbot = false;

int main() {

	HDC hdc = GetDC(FindWindowA(NULL, "Apex Legends"));//Getting the Window to Draw Over

	process_id = get_process_id("r5apex.exe");//Getting the Process Id
	base_address = get_module_base_address("r5apex.exe");//Getting The Module Base

	cout << "Process id found at " << process_id << endl;//Printing the Process Id (For Debugging and check if all ok)
	cout << "Process Base Address found at 0x" << hex << base_address << endl;//Printing the Module Base (For Debugging and check if all ok)

	while (true) { // While loop so the esp doesnt stop at any point

		Matrix ViewMatrix = Read<Matrix>(base_address + Offsets::ViewMatrix);//Reading the ViewMatrix and Storing it as a 16 Matrix (Not a 4x4 matrix)
		uintptr_t LocalPlayer = Read<uintptr_t>(base_address + Offsets::LocalPlayer);//Reading LocalPlayer for Own TeamMates and Calculating Enemy Distance

		for (int Entity = 0; Entity < 100; Entity++) { // For Loop so that we can loop through every Entity in Game (Enemies and TeamMates)
			uintptr_t Base = Read<uintptr_t>(base_address + Offsets::EntityList + (Entity << 5));//Reading the EntityList and storing Each Enemy (its inside a for loop so it will store all 100 entity inside the 'Base' for max loop times)

			Vector3 LocalOrigin = Read<Vector3>(LocalPlayer + Offsets::Origin);//Locating Ourselves inside Game (Owr Own Position inside Game) and Storing it as a Vecto3 (Length, Height, Breadth)
			Vector3 Origin = Read<Vector3>(Base + Offsets::Origin);//Locating Enemy inside Game (Enemy Position inside Game) and Storing it as a Vecto3 (Length, Height, Breadth)

			float Distance = (Vector3::Distance(LocalOrigin, Origin)) / 36.5f; //Calculating the distance of each enemy from ourself (not Our teammate but only YOU)

			char Dist[100];
			snprintf(Dist, sizeof(Dist), "%f", Distance);//Storing Distance(float value) as a character so that we can print it

			DrawCrosshair(hdc, ScreenWidth, ScreenHeight, 60);//Draw CrossHasir
			if ((int)Distance <= 500) { //Emitting out the enemies who are 500 meters far away from us so we can save cpu usage and improve the fps
				int OwnTeam = Read<int>(LocalPlayer + Offsets::TeamId);//Reading Own Team id and storing it as integer
				int Team = Read<int>(Base + Offsets::TeamId);//Reading Enemy Team id and storing it as integer
				float Health = Read<int>(Base + Offsets::Health);//Reading Current Enemy Health and storing it as float
				float MaxHealth = Read<int>(Base + Offsets::MaxHealth);//Reading Maximum Enemy Health that they can have and storing it as float
				int Knocked = Read<int>(Base + Offsets::Knocked);//Reading if enemy is knocked or not and storing it as int but we will use it as a boolean by passing 0 or 1 argument
				int Dead = Read<int>(Base + Offsets::Dead);//Reading if enemy is Dead or not and storing it as int but we will use it as a boolean by passing 0 or 1 argument

				Vector3 HeadPos;//Making a variable for Enemy Head Location
				HeadPos.x = Origin.x;
				HeadPos.y = Origin.y;
				HeadPos.z = Origin.z + 75.f;//The height of every Object inside any game is somewhat the same as 75.f from their base of their foot (origin)

				Vector3 OriginPos = WorldToScreen(Origin, ViewMatrix, ScreenWidth, ScreenHeight);//Converting Enemy position to 2d so we can print it or calculate stuff easilywith it
				Vector3 HeadLocation = WorldToScreen(HeadPos, ViewMatrix, ScreenWidth, ScreenHeight);//Converting Enemy HeadLocation to 2d so we can print it or calculate stuff easilywith it

				/************************************************************************************************
				NOTE: I also have added the bone reading function and made it easy to understand,
				so u can also directly read the head position instead of adding 75.f to origin, 
				but the reason is this way the box created over enemy looks better 
				*************************************************************************************************/
				
				float height = HeadLocation.y - OriginPos.y;//Calculations for box height
				float width = height / 2.4f;//Calculations for box width

				//Draw Esp
				if (OriginPos.z >= 0.01f && Team != OwnTeam && Health > 0 && Health < 101 && Dead == 0) {//Ommiting the entities which doesnt agree to these parameters
					if (Knocked > 0) { // If enemy is knowcked then draw different color and stuff
						DrawKnockedString(hdc, OriginPos.x, OriginPos.y + 30, "Knocked");//Draw a string when enemy is knocked
						DrawKnockedString(hdc, OriginPos.x, OriginPos.y + 10, Dist);//Draw Ditance of enemy
						DrawKnockedRectangle(hdc, OriginPos.x - (width / 2), OriginPos.y, width, height, 1);//Draw a Rectangular box over enemy
						DrawKnockedLine(hdc, ScreenWidth / 2, ScreenHeight / 8, HeadLocation.x, HeadLocation.y);//Draw a line from screen top to enemy head
					} else { //if not knocked then do other stuff
						DrawNotSeenString(hdc, OriginPos.x, OriginPos.y + 10, Dist);//Draw Ditance of enemy
						DrawNotSeenRectangle(hdc, OriginPos.x - (width / 2), OriginPos.y, width, height, 1);//Draw a Rectangular box over enemy
						DrawNotSeenLine(hdc, ScreenWidth / 2, ScreenHeight / 8, HeadLocation.x, HeadLocation.y);//Draw a line from screen top to enemy head
					}
				}
				
				/************************************************************************************************
				NOTE: For those who wonder why i am reading like this 'if (Knocked > 0) 'because 
				i read the knocked as int so if it is false then it will return 1 which is actually true 
				(read details of boolean, 1 is true and 0 is false) so if knocked is more than 0 (not including 0) 
				then its a knocked, but if enemy is not knocked then it will be less than 1 so this is a better way 
				to do stuff
				************************************************************************************************/
			}
		}

		if (GetAsyncKeyState(VK_DELETE)) { //Pressing 'Delete' key will end the while loop, breaking the loop will result in closing down the hack
			break;
		}
	}
}
