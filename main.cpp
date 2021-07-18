// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Tilemap.h"

constexpr int WINDOW_WITH   {450};
constexpr int WINDOW_HEIGHT {450};
float speed = 1.0f;
bool  pause = false;


auto get_tile_color(int x, int y) -> sf::Color 
{
  // top left, top right, bottom left, bottom right quadrants
  if(x < WINDOW_WITH / 3 || x >= WINDOW_WITH - (WINDOW_WITH/3) )
    if(y < WINDOW_HEIGHT / 3 || y >= WINDOW_HEIGHT - (WINDOW_HEIGHT/3) )
      return sf::Color::White;
  
  // center quadrant
  if(x >= WINDOW_WITH / 3 && x < WINDOW_WITH - (WINDOW_WITH/3) )
    if(y >= WINDOW_HEIGHT / 3 && y < WINDOW_HEIGHT - (WINDOW_HEIGHT/3) )
      return sf::Color::White;

  return sf::Color(230,230,230); // light gray
}

auto draw_tilemap(sf::RenderWindow& renderWindow, Map& map, sf::Font& font) -> void 
{
  // clear canvas
  renderWindow.clear();

  // draw white background
  sf::RectangleShape background(sf::Vector2f(WINDOW_WITH,WINDOW_HEIGHT));
  background.setFillColor(sf::Color::White);
  renderWindow.draw(background);

  // iterate over tilemap to draw sudoku grid
  for(auto i : map.Tilemap){
      for(auto j : i){
        // drawing background tile
        sf::RectangleShape tile;
        tile.setFillColor(get_tile_color(j.x,j.y));
        tile.setOutlineColor(sf::Color::Black);
        tile.setOutlineThickness(1.0f);
        tile.setSize(sf::Vector2f(j.width-4,j.height-4)); // -4 for outline
        tile.setPosition(j.x+2,j.y+2);                    // +2 for outline
        renderWindow.draw(tile);
      
        // drawing text into tile
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(j.value));
        text.setCharacterSize(j.height/2); // in pixels
        j.is_given ?
          text.setFillColor(sf::Color::Black) : text.setFillColor(sf::Color::Blue);
        if(j.value == 0)
          text.setFillColor(sf::Color::Transparent);
        text.setPosition(j.x+j.width/3, j.y+j.height/5);
        renderWindow.draw(text);
      }
  } 
  
  // display canvas
  renderWindow.display();
}

auto handle_events(sf::RenderWindow& renderWindow, Map& map) -> void 
{
  sf::Event event;
  while(renderWindow.pollEvent(event))
  {
    // get mouse position relative to window
    sf::Vector2i mpos = sf::Mouse::getPosition(renderWindow);

    switch(event.type)
    {
    case sf::Event::EventType::Closed:
        renderWindow.close();
        break;

    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Add:
            std::cout << "Speed++\n";
            speed *= 5; break;

        case sf::Keyboard::Subtract:
            std::cout << "Speed--\n";
            speed /= 5; break;

        case sf::Keyboard::Space:
            std::cout << "(un)pause\n";
            pause = !pause; break;

        // delete all values
        case sf::Keyboard::C:
            std::cout << "Clear\n";
            pause = true;
            map.clear(); break;

        case sf::Keyboard::E:
            std::cout << "Example\n";
            pause = true;
            map.clear();
            map.set_example(); break;

        // value input
        case sf::Keyboard::Num0:
        case sf::Keyboard::Numpad0:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,0); break;
          
        case sf::Keyboard::Num1:
        case sf::Keyboard::Numpad1:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,1); break;
          
        case sf::Keyboard::Num2:
        case sf::Keyboard::Numpad2:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,2); break;
          
        case sf::Keyboard::Num3:
        case sf::Keyboard::Numpad3:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,3); break;
          
        case sf::Keyboard::Num4:
        case sf::Keyboard::Numpad4:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,4); break;
          
        case sf::Keyboard::Num5:
        case sf::Keyboard::Numpad5:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,5); break;
          
        case sf::Keyboard::Num6:
        case sf::Keyboard::Numpad6:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,6); break;
          
        case sf::Keyboard::Num7:
        case sf::Keyboard::Numpad7:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,7); break;
          
        case sf::Keyboard::Num8:
        case sf::Keyboard::Numpad8: 
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,8); break;
          
        case sf::Keyboard::Num9:
        case sf::Keyboard::Numpad9:
            pause = true;
            map.clear(false);
            map.set(mpos.x,mpos.y,9); break;
        }
        break;
    } // end switch
  } // end while
}


auto main(int argc, char ** argv) -> int 
{  
  // init tilemap
  Map map(9,9);
  map.set_example();

  // init font
  sf::Font font;
  if(!font.loadFromFile("Gidole.ttf")){
    std::cout << "Error loading font\n";
  }

  // init clock
  sf::Clock clock;

  // init window
  sf::RenderWindow renderWindow(
    sf::VideoMode(WINDOW_WITH, WINDOW_HEIGHT), "Sudoku Solver");
  
  while (renderWindow.isOpen())
  {
    // solve the sudoku
    auto time = clock.getElapsedTime().asSeconds();
    if(time >= (0.2f / speed) ){
        clock.restart();
        if(!pause)
            map.step();
    }

    draw_tilemap(renderWindow,map,font);
    handle_events(renderWindow,map);
  }
}