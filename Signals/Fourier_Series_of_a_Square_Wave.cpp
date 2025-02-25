//IMGUI and SFML 2.6 are needed to compile this

//SFML is used to create the Window and draw the shapes and lines on top of that window
#include <SFML/Graphics.hpp>

//ImGUI is used to create the UI elements like the sliders in this program
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <cmath>
#include <ctime>

// double ended queue = deque
#include <deque>



int height = 800;
int width = 800;

sf::Vector2f u = { 1,1 };
float  r = 100;
float delta = 0.01;
float radius = 100;
int n = 3;
float t = 0;
std::deque<sf::Vertex> points;

const sf::Vector2f first_circle_center = sf::Vector2f(width / 2 - 200, height / 2);



int main()
{

//initializing the scene and background work
    sf::Clock deltaClock;
    sf::RenderWindow window(sf::VideoMode(height,width), "Fourier Series");
    ImGui::SFML::Init(window);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        

		t += delta;
        sf::Event event;
        std::srand(std::time(nullptr));
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
			else if (event.type == sf::Event::Resized)
			{
				height = event.size.height;
				width = event.size.width;
                u = sf::Vector2f{ 800 /(float)width, 800 / (float)height};

                
			}
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Window title");
		ImGui::SliderFloat("Radius", &radius, 0, 200);
		ImGui::SliderInt("Number of circles", &n, 1, 200);
		ImGui::SliderFloat("Speed", &delta, 0.01, 0.05);

        ImGui::End();

        window.clear(sf::Color::Black);

      //Wave Generation Begins here
	sf::Vector2f circle_center = first_circle_center;

        for (size_t i = 0; i < n-1; i++)
	{
		//here r is ak or FS(Fourier Series) Coefficient
           	r = radius / (2 * i + 1);
		//omega is the fundamental frequency
		float omega = 2 * i + 1;
		sf::CircleShape circle(r);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(1);
		circle.setOutlineColor(sf::Color::White);
		circle.setOrigin(r, r);
		circle.setScale(u);
		circle.setPosition(circle_center.x ,circle_center.y );
		window.draw(circle);

		sf::Vertex line[2];
		line[0].position = circle.getPosition();
		//the u.x and u.y have no relation to the function, strictly there for drawing purposes, basically to scale correctly when we resize the window
		line[1].position = circle.getPosition() + sf::Vector2f(r * std::cos(omega*t)*u.x, r * std::sin(omega*t)*u.y);
		line[0].color = sf::Color::White;
		line[1].color = sf::Color::White;
		window.draw(line, 2, sf::Lines);

		circle_center = circle.getPosition() + line[1].position - line[0].position;
			

        }
      
		r = radius / (2 * (n-1) + 1);
		sf::CircleShape circle(r);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(1);
		circle.setOutlineColor(sf::Color::White);
		circle.setOrigin(r, r);
		circle.setScale(u);
		circle.setPosition(circle_center.x, circle_center.y);
		window.draw(circle);

		sf::Vertex line[2];
		line[0].position = circle.getPosition();
		line[1].position = { first_circle_center.x + 200 ,circle_center.y };
		line[0].color = sf::Color::White;
		line[1].color = sf::Color::White;
		window.draw(line, 2, sf::Lines);

      //Drawing the wave happens here
    points.push_front(sf::Vertex({ first_circle_center.x + 200 ,circle_center.y}, sf::Color::White));
	if (points.size() > 2000)
	{
		points.pop_back();
	}
        for (sf::Vertex &point: points)
        {
		point.position.x += 0.2 * delta / 0.01;
			
        }
		for (size_t i = 0; i < points.size()-1; i++)
		{
			sf::Vertex line[2];
			
			line[0] = points[i];
			line[1] = points[i+1];
			window.draw(line, 2, sf::Lines);

		}

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
