#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
#include <vector>
#include "gameLogic.h"
#include "Network.h"
#include <thread>

//global variables for use in network class
gameLogic player1;
vector <sf::CircleShape> paintballs;
vector <sf::CircleShape> paintballs2;
Network networking;

sf::Vector2<float> latestPos;
sf::Vector2<float> previosPos;

void netloop()
{
	networking.networkLoop();//constant loop in the network class
}

sf::CircleShape interpolate()
{
	
	sf::Vector2<float> renderPos;
	float timeStamp;
	sf::Time time;
	sf::CircleShape player = networking.getPlayer2();
	float y_change;
	float x_change;
	float plax;
	float play;
	
	//get all required data from network thread
	timeStamp = networking.getTimestamp();
	latestPos = networking.getLatestpos();
	previosPos = networking.getPreviouspos();
	time = networking.getTimecurrent();//res

	
	x_change = latestPos.x - previosPos.x;//find difference in x and y pos
	y_change = latestPos.y - previosPos.y;

	float time_change = time.asSeconds() - timeStamp; //find latency by calculating difference in the local clock and the networked clock

	x_change /= time_change;//calculate velocity
	y_change /= time_change;

	time_change = time.asSeconds() - timeStamp;//reset time

	plax = latestPos.x + (time_change * x_change);//update predictions
	play = latestPos.y + (time_change * y_change);

	renderPos.x = plax;
	renderPos.y = play;

	
	player.setPosition(renderPos);//render
	return player;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(564, 564), "Game!");
	window.setFramerateLimit(30);
	bool update = true;

	std::thread netwrk(netloop);//begin network thread

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				netwrk.join();//safely close thread
			}
				
			else if (event.type == sf::Event::GainedFocus)//for use in local testing, not required when fully networked
				update = true;
			else if (event.type == sf::Event::LostFocus)
				update = false;
		}

		if (update)
		{
			player1.update();//update player in gameLogic class
		}
		networking.updateLocalplayer(player1.getPlayer(), player1.getBalls());//send latest positions to network thread
		window.clear();
				//draw all info to screen
		window.draw(player1.getPlayer());
		window.draw(player1.getRect());
		window.draw(player1.getRect2());
		window.draw(interpolate());
		paintballs = player1.getBalls();//retrieve vector from logic class
		for (unsigned int i = 0; i < paintballs.size(); i++)
		{
			window.draw(paintballs[i]);//draw all paintballs on screen
		}
		paintballs2 = networking.getPlayer2Balls();
		for (unsigned int i = 0; i < paintballs2.size(); i++)
		{
			window.draw(paintballs2[i]);//draw all paintballs on screen
			std::cout << paintballs2[i].getFillColor().r << paintballs2[i].getFillColor().g << std::endl;
		}
		window.display();

		
	}
	return 0;
}


