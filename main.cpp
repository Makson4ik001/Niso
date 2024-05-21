#include<iostream>
#include<iomanip>
#include<fstream>
#include"Land.h"
#include <SFML/Graphics.hpp>
#include<string>
#include"converter.h"
#include "A_star.h"
#include "CircuitConnector.h"
#include"GraphBuilder.h"
#include"circuit_filter.h"
#include "BenchmarkWrapper.hpp"



using namespace std;



std::vector<point_dec_d> converter_from_graph(std::vector<int> Path, DecGraph graph) {
    std::vector<point_dec_d> res;
    for (int i = 0; i < Path.size(); i++) {
        res.push_back(graph.coordinates[Path[i]]);
    }

    return res;

}

auto makeToVertex(sf::Color circuitColor) {
    return [circuitColor](point_dec_d point) {
        return sf::Vertex(sf::Vector2f(point.x, -point.y), circuitColor);
    };
}

std::vector<point_dec_d> circuitToDec(const std::vector<point>& circuit, const Converter& c) {
    std::vector<point_dec_d> res(circuit.size());
    for (size_t i = 0; i < circuit.size(); i++)
        res[i] = c.convertTo(circuit[i]);
    return res;
}

std::vector<sf::Vertex> transformCircuit(const std::vector<point_dec_d>& circuit, sf::Color circuitColor = sf::Color::White) {
    auto toVertex = makeToVertex(circuitColor);
    vector<sf::Vertex> result(circuit.size());
    for (size_t i = 0; i < circuit.size(); i++)
        result[i] = toVertex(circuit[i]);
    return result;
}

vector<sf::Vertex> transformPath(vector<point_dec_d>& dec_Path, sf::Color circuitColor = sf::Color::Red) {
    auto toVertex = makeToVertex(circuitColor);
    std::vector<sf::Vertex> result(dec_Path.size());
    for (size_t i = 0; i < dec_Path.size(); i++)
        result[i] = toVertex(dec_Path[i]);

    return result;
}

void showWindow(vector<vector<point_dec_d>>& circuits, vector<point_dec_d>& path) {
    constexpr float INITIAL_WIDTH = 1000, INITIAL_HEIGHT = 1000;
    sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "My window");

    vector<vector<sf::Vertex>> transformedCircuits(circuits.size());
    for (size_t i = 0; i < circuits.size(); i++)
        transformedCircuits[i] = transformCircuit(circuits[i]);

    vector<sf::Vertex> transformedPath = transformPath(path);

    bool eventHappened = true;

    sf::Vector2f center, size{ INITIAL_WIDTH, INITIAL_HEIGHT };
    float zoom = 0.7;
    sf::View view(center,size);
    view.zoom(zoom);
    window.setView(view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            eventHappened = true;
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                size.x = event.size.width;
                size.y = event.size.height;
                view.setCenter(center);
                view.setSize(size);
            }

            if (event.type == sf::Event::MouseWheelMoved) {
                zoom += event.mouseWheel.delta* 0.005;
                view.zoom(zoom);
                //TODO: resize viewport here
            }
        }

        if (!eventHappened)
            continue;
        window.setView(view);
        window.clear(sf::Color::Black);
        for (auto&& circuit : transformedCircuits)
            window.draw(&circuit[0], circuit.size(), sf::LineStrip);
        if (transformedPath.size() > 0)
            window.draw(&transformedPath[0], transformedPath.size(), sf::LineStrip);
        window.display();
        eventHappened = false;

    }

}

int main() {

    istream& read(char*, int);
    char buffer[37020] = {};
    ifstream fin("LAND", ios::in | ios::binary);
    fin.read(buffer, 37020);
    land l = readLand(buffer, 37020);
    fin.close();
    std::vector<std::vector<point>>& source = l.osus.contur_i;
    point anchor = { l.obsus.fi_0, l.obsus.lambda_0 };
    Converter converter(anchor);
    vector<vector<point_dec_d>> converted(source.size());
    for (size_t i = 0; i < converted.size(); i++) {
        converted[i] = circuitToDec(source[i], converter);
    }
    std::vector<std::vector<point_dec_d>> connected = connectedCircuits(converted);
   // connected = { connected[0] };
    std::vector<std::vector<point_dec_d>> simplifiedContours = connected;
  /*  for (int i = 0; i < connected.size(); i++) {
        simplifiedContours.push_back(DouglasPeucker(connected[i], 10));

    }*/



    
    
    //TODO: begin end = ? 
    point_dec_d begin = { 0., -170. }, end = { -30.,110. };
    DecGraph graph = makeGraph(simplifiedContours, begin, end);

    auto bench_res = benchmark_function(A_star, graph);
    vector<int> path = bench_res.second;
    auto time = bench_res.first;

    auto bench_res_2 = benchmark_function(Deikstra, graph);
    vector<int> path_2 = bench_res_2.second;
    auto time_2 = bench_res_2.first;

    auto convertedPath = converter_from_graph(path_2, graph);
    cout << time<<'\n'<<time_2;
    showWindow(simplifiedContours, convertedPath);
}
