#include "Network.h"


// The IP address of the player
#define SERVERIP "127.0.0.1"

playerinfo player2;//networked player
playerinfo player1;//local player
sendinfo packetSend;
sendinfo packetReceive;

Network::Network()
{
	player2.player.setRadius(10.0f);//init a player 2, to be overwritten by network data
	player2.player.setOutlineThickness(1);
	player2.player.setOutlineColor(sf::Color::White);
	//init a paintball to enable vector functrions to work
	paintballLocal.push_back(0);
	paintballLocal.push_back(0);
	paintballLocal.push_back(0);
	paintballLocal.push_back(0);
	paintballLocal.push_back(0);
};

void Network::networkLoop()
{
	
	playerIP = SERVERIP;//IP
	sf::Packet packetsend;

	sf::Packet packetreceive;//packets for sending and receiving position, colour
	sf::CircleShape paintPushback;//setup networked paintballs
	paintPushback.setRadius(5.0f);
	sf::IpAddress senderIP;
	std::size_t received;

	if (playerSocket.bind(port) != sf::Socket::Done)//bind socket
		{
			std::cout << "error" << std::endl;
		}
	sf::Time wait = sf::seconds(0.03f);//tickrate
	
	playerSocket.setBlocking(false);
	do { //infinite loop, constantly receiving/sending data
		updateLocalinfo();
		unsigned short senderport;
		if (playerSocket.receive(packetreceive, senderIP, senderport) != sf::Socket::Done)//receive data from other player
		{
			//error
			//std::cout << "Error has occured receiving data" << std::endl;
			
		}
		else
		{
			//std::cout << "Connected receiving" << std::endl;
		}
		
		if (startClock)
		{
			clock.restart();//start clock when first message received
		}
		startClock = false;//only start clock on the firs message, this will remain false for the rest of the game time
		
		if (packetreceive >> packetReceive)//receive packet over network
		{	
			timeStamp = packetReceive.timeStamp;//for use in interpolation
			previousPlayerpos = latestPlayerpos;//make the most current value being held the second most current
			latestPlayerpos = packetReceive.playerPos;//update most current position

			//unpack packet
			player2.player.setPosition(packetReceive.playerPos);
			player2.player.setFillColor(packetReceive.playerColor);
			if (packetReceive.paintballpos[0] != latestPaintballpos)//make sure a paintball will not be drawn ontop of another
			{
				for (int i = 0; i < 3; i++)
				{
					paintPushback.setPosition(packetReceive.paintballpos[i]);
					paintPushback.setFillColor(packetReceive.paintballColor[i]);
					player2.paintballs.push_back(paintPushback);
				}
				
													
			}
			
				latestPaintballpos = packetReceive.paintballpos[0];
			
		}
		packetSend.networkedTime = clock.getElapsedTime();//get timestamp to send over network
		packetsend << packetSend;//pack info into packet
		
		if (playerSocket.send(packetsend, playerIP, port2) != sf::Socket::Done)//send position of local player over network in packet
		{
			//error
			//std::cout << "Error has occured sending data" << std::endl;
			
		}
		else
		{
		}
		//clear packets for sending/recieving again
		packetsend.clear();
		packetreceive.clear();
		
		sf::sleep(wait);
		
	} while (true);
	
}
void Network::updateLocalinfo()//updates all local information. Called once per network loop
{
	//get all the info to send out to other players
	packetSend.playerPos = player1.player.getPosition();//get position of player to send
	
	packetSend.playerColor = player1.player.getFillColor();//get colour of player to send
	
//prepare the last 3 paintballs for sending over the network
	if (player1.paintballs.size() >= 3)
	{
		for (int i = (player1.paintballs.size() - 1); i < player1.paintballs.size(); i++)
		{
			packetSend.paintballColor[0] = player1.paintballs[i].getFillColor();
			packetSend.paintballColor[1] = player1.paintballs[i - 1].getFillColor();
			packetSend.paintballColor[2] = player1.paintballs[i - 2].getFillColor();

			packetSend.paintballpos[0] = player1.paintballs[i].getPosition();
			packetSend.paintballpos[1] = player1.paintballs[i - 1].getPosition();
			packetSend.paintballpos[2] = player1.paintballs[i - 2].getPosition();
		}
	}
	
}

void Network::updateLocalplayer(sf::CircleShape playerLocal, vector <sf::CircleShape> paintballs)//called in source, updates local player info for sending over network
{
	player1.player = playerLocal;
	player1.paintballs = paintballs;
}

sf::CircleShape Network::getPlayer2()
{
	return player2.player;//return networked player
}


vector <sf::CircleShape> Network::getPlayer2Balls()
{
	return player2.paintballs;//return network paintballs
}

//getters for use in interpolation
float Network::getTimestamp()
{
	return timeStamp;//timestamp of when message was sent
}

sf::Time Network::getTimecurrent()
{
	return clock.getElapsedTime();//get local clock time
}
//send last 2 positions for interpolation
sf::Vector2<float> Network::getLatestpos()
{
	return latestPlayerpos;
}

sf::Vector2<float> Network::getPreviouspos()
{
	return previousPlayerpos;
}

