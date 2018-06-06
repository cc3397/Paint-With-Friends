#include "gameLogic.h"
#include <iostream>
gameLogic::gameLogic()
{
	//init player
	player.setRadius(10.0f);
	player.setFillColor(sf::Color::Green);
	player.setOutlineColor(sf::Color::White);
	player.setOutlineThickness(1);

	//init 2 rectangles for colour changing
	rect.setPosition(100, 100);
	rect.setSize(sf::Vector2<float>(20, 20));
	rect.setFillColor(sf::Color::Blue);

	rect2.setPosition(300, 100);
	rect2.setSize(sf::Vector2<float>(20, 20));
	rect2.setFillColor(sf::Color::Magenta);

};
void gameLogic::update()//called every game loop
{
	
	gameLogic::controls();//handle keyboard inputs
	gameLogic::colourchanger();//handle collision detection to change player colour

		//make sure the player stays in the bounds of the window
		if (player.getPosition().x > 1260)
		{
			player.setPosition(1260, player.getPosition().y);
		}
		if (player.getPosition().x < 0)
		{
			player.setPosition(0, player.getPosition().y);
		}
		if (player.getPosition().y > 700)
		{
			player.setPosition(player.getPosition().x, 700);
		}
		if (player.getPosition().y < 0)
		{
			player.setPosition(player.getPosition().x, 0);
		}


		
}

void gameLogic::controls()
{	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && fireRate == 0)
	{
		//draw a paintball if space if pressed
		paintballinit.setRadius(5.0f);
		paintballinit.setFillColor(player.getFillColor());
		paintballinit.setPosition(player.getPosition());
		paintballs.push_back(paintballinit);
		fireRate++;//allow the player to only draw a certain amount of paintballs on the screen at once

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		fireRate++;
	}
	//get keyboard input for diagonal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		player.move(1, -1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		player.move(-1, -1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		player.move(-1, 1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		player.move(1, 1);
	}
	//get keyboard input for horizontal and vertical movement
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		player.move(1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		player.move(-1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		player.move(0, 1);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		player.move(0, -1);
	}

	if (fireRate >= 5)
	{
		fireRate = 0;
	}
	


}

void gameLogic::colourchanger()
{
	//detect a collision between the player and a paintball
	
	
		for (unsigned int i = 0; i < paintballs.size(); i++)
		{
			if (player.getGlobalBounds().intersects(paintballs[i].getGlobalBounds()))
			{
				player.setFillColor(paintballs[i].getFillColor());
			}

		}
		//detect collision between player and colour changer rectangles
		if (player.getGlobalBounds().intersects(rect.getGlobalBounds()))
		{
			player.setFillColor(rect.getFillColor());
		}

		if (player.getGlobalBounds().intersects(rect2.getGlobalBounds()))
		{
			player.setFillColor(rect2.getFillColor());
		}

	
}

void gameLogic::setPlayerPosition(int x, int y)
{
	player.setPosition(x, y);//set position
}

	//return all info to game loop for drawing to screen
sf::CircleShape gameLogic::getPlayer()
{
	return player;
}
vector <sf::CircleShape> gameLogic::getBalls()
{
	return paintballs;
}

sf::RectangleShape gameLogic::getRect()
{
	return rect;
}

sf::RectangleShape gameLogic::getRect2()
{
	return rect2;
}
