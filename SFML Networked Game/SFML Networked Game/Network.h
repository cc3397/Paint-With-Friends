#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "gameLogic.h"
#include <vector>
#include <iostream>

using std::vector;

#ifndef NETWORK_H
#define NETWORK_H
class Network
{
public:
	Network();
	void networkLoop();
	sf::CircleShape getPlayer2();//return player 2 to game loop
	vector <sf::CircleShape> getPlayer2Balls();//return paintballs to game loop
	void updateLocalplayer(sf::CircleShape, vector <sf::CircleShape>);
	sf::Clock clock;//clock to find latency between 2 players
	sf::Time tim;//time to calculate latency for interpolation
	sf::Vector2<float> latestPaintballpos, latestPlayerpos, previousPlayerpos;//latest paintball pos to go through if statements to ensure two arent drawn on top of each other
	vector <int> paintballLocal;
	bool startClock = true;//enable clock to be started once first msg is received
	void updateLocalinfo();
	//return info for interpolation
	float getTimestamp();
	sf::Vector2<float> getLatestpos();
	sf::Vector2<float> getPreviouspos();
	sf::Time getTimecurrent();
	float timeStamp;

private:
	sf::UdpSocket playerSocket;
	sf::IpAddress playerIP;
	unsigned short port = 5556;//setup 2 different ports for local testing, internet comms would not require this. The same port on both machines could be used
	unsigned short port2 = 5555;

};
struct playerinfo//for storing player info
	{
		sf::CircleShape player;
		vector <sf::CircleShape> paintballs;

	};

struct sendinfo //struct of all relevant info to be sent over the network
{
	sf::Vector2 <float> playerPos;
	sf::Color playerColor;

	//ability to send last three paintball locations
	sf::Color paintballColor[3];
	sf::Vector2 <float> paintballpos[3];
	sf::Time networkedTime;//time of message send for latency calcs
	float timeStamp;
};

//Overload SFML packets class to accept user specified data
static sf::Packet& operator <<(sf::Packet& packet, const sendinfo& info)
{
	return packet << info.playerPos.x << info.playerPos.y << info.playerColor.r << info.playerColor.g << info.playerColor.b << info.paintballpos[0].x << info.paintballpos[0].y << info.paintballpos[1].x << info.paintballpos[1].y << info.paintballpos[2].x << info.paintballpos[2].y << info.paintballColor[0].r << info.paintballColor[0].g << info.paintballColor[0].b << info.paintballColor[1].r << info.paintballColor[1].g << info.paintballColor[1].b << info.paintballColor[2].r << info.paintballColor[2].g << info.paintballColor[2].b << info.networkedTime.asSeconds();
}

static sf::Packet& operator >>(sf::Packet& packet, sendinfo& info)
{
	return packet >> info.playerPos.x >> info.playerPos.y >> info.playerColor.r >> info.playerColor.g >> info.playerColor.b >> info.paintballpos[0].x >> info.paintballpos[0].y >> info.paintballpos[1].x >> info.paintballpos[1].y >> info.paintballpos[2].x >> info.paintballpos[2].y >> info.paintballColor[0].r >> info.paintballColor[0].g >> info.paintballColor[0].b >> info.paintballColor[1].r >> info.paintballColor[1].g >> info.paintballColor[1].b >> info.paintballColor[2].r >> info.paintballColor[2].g >> info.paintballColor[2].b >> info.timeStamp;
}
#endif

