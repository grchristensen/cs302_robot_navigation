#include "PhysicalMap.h"
#include <iostream>
#include <algorithm>
#include <cmath>

PhysicalMap::PhysicalMap()
{
}

/*PhysicalMap::PhysicalMap(const size_t obsXMin, const size_t obsXMax, const size_t obsYMin, const size_t obsYMax)
{
	

	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		for (size_t j = 0; j < MAP_SIZE; j++)
		{
			if ((j >= obsXMin && j <= obsXMax) && (i >= obsYMin && i <= obsYMax))
				map[j][i] = true;
			else
				map[j][i] = false;
		}
	}
}*/

PhysicalMap::PhysicalMap(const Obstacle obs)
{
	auto g = MyGraphType(MAP_SIZE * MAP_SIZE);
	
	auto iter_pair = boost::vertices(g);
	
	int i = 0;
	for (auto iter = iter_pair.first; iter != iter_pair.second; iter++) {
		if (!pointIsObstacle(i, obs)) {
			if(!pointIsLeftEdge(i, MAP_SIZE)) {
				attemptConnection(i, i - 1, g, obs);
				
				if (!pointIsBottom(i, MAP_SIZE)) {
					attemptConnection(i, i + MAP_SIZE - 1, g, obs);
				}
			}
			
			if(!pointIsBottom(i, MAP_SIZE)) {
				attemptConnection(i, i + MAP_SIZE, g, obs);
			}
			
			if(!pointIsRightEdge(i, MAP_SIZE)) {
				attemptConnection(i, i + 1, g, obs);
				
				if(!pointIsBottom(i, MAP_SIZE)) {
					attemptConnection(i, i + MAP_SIZE + 1, g, obs);
				}
			}
		}
		
		i++;
	}
	
	map = g;
}

/*PhysicalMap::PhysicalMap(const Obstacle * obsArray, const size_t numObstacles)
{
	//creates first obstacle and the non-occupied spaces
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		for (size_t j = 0; j < MAP_SIZE; j++)
		{
			if ((j >= obsArray[0].xMin && j <= obsArray[0].xMax) &&
				(i >= obsArray[0].yMin && i <= obsArray[0].yMax))
				map[j][i] = true;
			else
				map[j][i] = false;
		}
	}

	//creates all remaining obstacles
	for (size_t obsIndex = 1; obsIndex < numObstacles; obsIndex++)
	{
		for (size_t i = 0; i < MAP_SIZE; i++)
		{
			for (size_t j = 0; j < MAP_SIZE; j++)
			{
				if ((j >= obsArray[0].xMin && j <= obsArray[0].xMax) &&
					(i >= obsArray[0].yMin && i <= obsArray[0].yMax))
					map[j][i] = true;
			}
		}
	}
}*/

PhysicalMap::PhysicalMap(const PhysicalMap & other)
{
	map = other.map;
}

/*PhysicalMap & PhysicalMap::operator=(const PhysicalMap & rhs)
{
	if (this != &rhs)
	{
		for (size_t i = 0; i < MAP_SIZE; i++)
		{
			for (size_t j = 0; j < MAP_SIZE; j++)
			{
				map[j][i] = rhs.map[j][i];
			}
		}
	}

	return *this;
}*/

/*bool PhysicalMap::isOccupied(const size_t x, const size_t y) const
{
	//array indexing causes this to be (y, x)
	return map[y][x];
}*/

void PhysicalMap::visualize() const
{
	visualize(std::cout);
}

void PhysicalMap::visualize(std::ostream & os) const
{
	auto iter_pair = boost::vertices(map);
	
	int i = 0;
	for(auto iter = iter_pair.first; iter!= iter_pair.second; iter++) {
		auto adj_v = boost::adjacent_vertices(*iter, map);
		
		int count = 0;
		for(auto iter = adj_v.first; iter != adj_v.second; iter++) {
			count++;
		}
		
		// os << count << " ";
		os << map[*iter].probTaken << " ";
		
		if ((i+1) % MAP_SIZE == 0) {
			os << std::endl;
		}
		
		i++;
	}
}

std::ostream & operator<<(std::ostream & os, const PhysicalMap & map)
{
	map.visualize(os);
	return os;
}

std::pair<int, int> PhysicalMap::coordinate(const int i, const int map_size)
{
	int x = 0, y = 0;
	
	int index = i;
	while (index >= map_size) {
		y++;
		index -= map_size;
	}
	x = index;
	
	std::pair<int, int> coord;
	coord.first = x;
	coord.second = y;
	
	return coord;
}

bool PhysicalMap::pointIsObstacle(const int i, const Obstacle obs) 
{
	auto coord = coordinate(i, MAP_SIZE);
	
	if (coord.first >= obs.xMin && coord.first <= obs.xMax) {
		if (coord.second >= obs.yMin && coord.second <= obs.yMax) {
			return true;
		}
	}
	
	return false;
}

bool PhysicalMap::pointIsBottom(const int i, const int mapSize)
{
	auto coord = coordinate(i, mapSize);
	
	if (coord.second == mapSize - 1) {
		return true;
	}
	
	return false;
}

bool PhysicalMap::pointIsLeftEdge(const int i, const int mapSize)
{
	auto coord = coordinate(i, mapSize);
	
	if(coord.first == 0) {
		return true;
	}
	
	return false;
}

bool PhysicalMap::pointIsRightEdge(const int i, const int mapSize)
{
	auto coord = coordinate(i, mapSize);
	
	if(coord.first == mapSize - 1) {
		return true;
	}
	
	return false;
}

bool PhysicalMap::attemptConnection(const int i, const int other, MyGraphType& g, const Obstacle obs) {
	if (!pointIsObstacle(other, obs)) {
		if (!edge(other, i, g).second){
			add_edge(i, other, g);
		}
		
		return true;
	}
	
	return false;
}

int PhysicalMap::vertexCount() const {
	auto iter_pair = boost::vertices(map);
	
	int i = 0;
	for (auto iter = iter_pair.first; iter != iter_pair.second; iter++) {
		i++;
	}
	
	return i; 
}

int PhysicalMap::edgeCount() const {
	auto iter_pair = boost::edges(map);
	
	int i = 0;
	for (auto iter = iter_pair.first; iter != iter_pair.second; iter++) {
		i++;
	}
	
	return i;
}

int PhysicalMap::index(const int x_pos, const int y_pos) {
	return x_pos + (MAP_SIZE * y_pos);
}

bool PhysicalMap::inBounds(const int x, const int y) {
	return (x >= 0 && x < MAP_SIZE) && (y >= 0 && y < MAP_SIZE);
}

static inline float proximity(int locX, int locY) //proximity is going to be used a lot for updating the map, therefore inline
{
	
	if (locX != locY)
	{
		float x1 = locX % MAP_SIZE;
		float y1 = locX / MAP_SIZE;
		float x2 = locY % MAP_SIZE;
		float y2 = locY / MAP_SIZE;
		float d = sqrt(pow(x1-x2, 2) + pow(y1-y2));
		float result = 1.99-1.5*log10(d+3.642);
		return 1-result;
	}
	else //They are the same, therefore it will have a high chance for the reading to be accurate
		return 1-.999999999;
	
}

void PhysicalMap::updateMap(int locX, int locY, float sensorRead) //Based off of getting a reading for a specific location it will determine the likelihood of other points on the map having a that same probability
{
	int index = (MAP_SIZE * locY) + locX;
	// std::cout << map[index].probTaken << std::endl;
	//auto range = boost::in_edges(index, g);
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (map[index].probTaken != 0 && map[index].probEmpty != 0) //This should always be the case, but for safety
			{
				auto value = proximity(index, i+j) + map[(MAP_SIZE*i)+j].probTaken / map[index].probTaken; 
				auto value2 = proximity(index, i+j) + map[(MAP_SIZE*i)+j].probEmpty / map[index].probEmpty; 
				if (map[(MAP_SIZE*i)+j].probTaken > map[index].probTaken)
					map[(MAP_SIZE*i)+j].probTaken -= value;
				else
					map[(MAP_SIZE*i)+j].probTaken += value;
				if (map[(MAP_SIZE*i)+j].probEmpty > map[index].probEmpty)
					map[(MAP_SIZE*i)+j].probEmpty -= value2;
				else
					map[(MAP_SIZE*i)+j].probEmpty += value2;
			}
			
			// std::cout << map[(MAP_SIZE*i)+j].probTaken << std::endl;

			//std::cout << map[index].probEmpty << std::endl;
			
		}
	}

}

MyGraphType::vertex_descriptor PhysicalMap::vFromIndex(const int index) {
	auto iter_pair = boost::vertices(map);

	int count = 0;
	for (auto iter = iter_pair.first; iter != iter_pair.second; iter++) {
		if (count == index) {
			return *iter;
		}
		
		count++;
	}
}