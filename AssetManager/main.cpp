#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "ResourcePath.hpp"
#include "TileMap.cpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "json11.hpp"


void saveMap(int (&level)[128]);
void loadMap(int (&level)[128], TileMap &map);

void gui();

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");
    int currTile = 0;
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    
    sf::Texture blueSquare;
    if (!blueSquare.loadFromFile(resourcePath() + "blueSquare.png")){ std::cout<<"DIDNT LOAD"<< std::endl;}
    sf::Sprite selection;
    selection.setTexture(blueSquare);
    selection.scale(.15, .15);
    
    selection.setPosition(0, 0);
    
    // define the level with an array of tile indices
    int level[128];
    for(int i = 0; i < 256; i++)
    {
        level[i] = 0;
    }
    
    TileMap map;
    // create the tilemap from the level definition

    if (!map.load(resourcePath()+"tileset.png", sf::Vector2u(32, 32), level, 16, 8))
        return -1;
    // run the main loop
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (currTile -16 >= 0){
                        currTile -= 16;
                        selection.move(0,-32);
                    }
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    if (currTile +16 < 256){
                        currTile += 16;
                        selection.move(0,32);
                    }
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    if (currTile -1 >= 0){
                        currTile -= 1;
                        selection.move(-32,0);
                    }
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    if (currTile +1 < 256){
                        currTile += 1;
                        selection.move(32,0);
                    }
                }
                if (event.key.code == sf::Keyboard::Enter)
                {
                    level[currTile] +=1;
                    if (level[currTile] > 3){
                        level[currTile] = 0;
                    }
                    // Update tilemap from the level definition
                    if (!map.load(resourcePath()+"tileset.png", sf::Vector2u(32, 32), level, 16, 8))
                        return -1;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    saveMap(level);
                    std::cout << "Level saved" << std::endl;
                }
                if (event.key.code == sf::Keyboard::L)
                {
                    loadMap(level, map);
                }
            }
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        ImGui::SFML::Update(window, deltaClock.restart());
        
        //guibutton code
        ImGui::Begin("Save/Load");
        ImGui::Text("Arrow Keys to select tile.");
        ImGui::Text("Enter key to change tile");
        if (ImGui::Button("Save Map")) {
            saveMap(level);
            std::cout << "Map Saved";
        }
        if (ImGui::Button("Load Map")) {
            loadMap(level, map);
            std::cout << "Map Loaded";
        }
        // draw the map
        window.clear();
        window.draw(map);
        window.draw(selection);
        ImGui::End();
        ImGui::SFML::Render(window);
        window.display();
    }
    return 0;
}

void saveMap(int (&level)[128]){
    std::ofstream out("/Users/seankearney/Unimportant/C++GameDev/data.csv");
    for(int i = 0; i < 128; i++){
        out << level[i] << ",";
    }
}

void loadMap(int (&level)[128], TileMap &map){
    std::ifstream dataFile("/Users/seankearney/Unimportant/C++GameDev/data.csv");
    if(!dataFile.is_open()) {
        std::cout << "Error opening data file" << std::endl;
    }
    int i = 0;
    while(i < 128)
    {
        std::string currentvalue;
        std::getline(dataFile, currentvalue, ',');
        level[i] = std::stoi(currentvalue);
        i++;
    }
    dataFile.close();
    if (!map.load(resourcePath()+"tileset.png", sf::Vector2u(32, 32), level, 16, 8))
        return -1;
}
