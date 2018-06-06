#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

#ifndef GAMELOGIC_H
#define GAMELOGIC_H
using std::vector;

class gameLogic
{
public:
	gameLogic();
  void update();
  void controls();
  void colourchanger();
  void setPlayerPosition(int x, int y);
  sf::CircleShape getPlayer();
  vector <sf::CircleShape> getBalls();
  sf::RectangleShape getRect();
  sf::RectangleShape getRect2();
private:
	//sf::RenderWindow window;
	sf::CircleShape player;
	sf::CircleShape player2;
	sf::RectangleShape rect;
	sf::RectangleShape rect2;
	vector <sf::CircleShape> paintballs;
	sf::CircleShape paintballinit;
	int fireRate = 0;
};




#endif
