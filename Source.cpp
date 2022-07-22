//
// Created by Иван Ильин on 22.09.2021.
//

#include <fstream>
#include <utility>
#include <iostream>
#include "network/ShooterServer.h"
#include "engine/utils/Time.h"
#include "engine/utils/Log.h"
#include "engine/Consts.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

void InitServer(ShooterServer& server)
{
    sf::Uint16 serverPort;
    std::ifstream connectFile("server.txt", std::ifstream::in);

    if (!connectFile.is_open() || !(connectFile >> serverPort))
    {
        connectFile.close();
        // Create file and write default settings
        serverPort = 54000;
        std::ofstream temp("server.txt", std::ofstream::out);
        temp << serverPort;
        temp.close();
    }
    connectFile.close();

    server.start(serverPort);
    if(server.isWorking())
        server.generateBonuses();
}

int main() {
    Time::init();
    ShooterServer server{};

    Log::log("Initializing the server...");
    InitServer(server);

    std::cout << "Server started!" << std::endl;
  
    auto bot = server.addBot();
    
    bot->translate(Vec3D(-1000, -1000, -1000));
    

    double lastTryReconnecting = 0;
    long aliveTime = 0;
    long tick = 0;

    while (true) {
        if(!server.isWorking() && (Time::time() - lastTryReconnecting > Consts::NETWORK_TIMEOUT)) {
            lastTryReconnecting = Time::time();
            server.stop();
            Log::log("Restarting the server...");
            InitServer(server);
        }
        Time::update();
        server.update();

        bot->setPlayerNickName("Server alive: "+std::to_string(aliveTime / 1000 / 60 / 60) + " hours");
        bot->setKills(-1);
        bot->setHealth(-1);
        bot->rotateToAngle( Vec3D{0,(double)tick/100,0});

        bot->setDeaths(0);
        

        aliveTime += 10;
        tick++;

        sleep_for(10ms);

        std::cout << "\x1B[2J\x1B[H";
        string output = "";
        for (auto& [playerId, player] : server._players) {
            
            output += std::to_string(playerId) + ") ";
            output += player->playerNickName() + "\t";
            output += std::to_string( player->kills()) + ":" + std::to_string( player->deaths() ) + "\t";
            output += "Ping: "+std::to_string(player->ping);
            output += "\n";
            
        }
        std::cout << output;

    }

    return 0;
}
