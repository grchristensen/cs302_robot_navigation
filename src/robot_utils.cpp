#include "robot.h"

// Expects angle to be between 0-360 degrees, call localize on
// your angle first
unsigned int Robot::Quartile(const int degrees) //Checks which quartile the degree is in
{
  unsigned int quartile;

  if (degrees >= 0 && degrees < 90) //First quartile
  {
    quartile = 0;
  }
  else if (degrees >= 90 && degrees < 180) //Second quartile
  {
    quartile = 1;
  }
  else if (degrees >= 180 && degrees < 270) //3rd quartile
  {
    quartile = 2;
  }
  else if (degrees >= 270 && degrees < 360) //4th quartile
  {
    quartile = 3;
  }
  else
  {
    throw; //This shouldn't happen if we properly made it between 0-360 from other function
  }

  return quartile;
}



SignPair Robot::Steps(const unsigned int quartile) //Makes the first and second value equal to whether or not it is positive or negative as a result of the quartile
{
  SignPair steps;

  if (quartile == 0)
  {
    steps.first = Sign::Positive;
    steps.second = Sign::Negative;
  }
  else if (quartile == 1)
  {
    steps.first = Sign::Negative;
    steps.second = Sign::Negative;
  }
  else if (quartile == 2)
  {
    steps.first = Sign::Negative;
    steps.second = Sign::Positive;
  }
  else if (quartile == 3)
  {
    steps.first = Sign::Positive;
    steps.second = Sign::Positive;
  }
  else
  {
    throw;
  }

  return steps;
}



bool Robot::Horizontal(const int read_y, const float ray_y, const Sign step_y)
{
  bool horizontal;

  const int proposed_y = read_y + (-1 * step_y);

  switch (step_y)
  {
  case Sign::Negative:
  {
    if (ray_y > proposed_y)
    {
      horizontal = false;
    }
    else if (ray_y < proposed_y)
    {
      horizontal = true;
    }
    else
    {
      throw;
    }
    
    break;
  }
  case Sign::Positive:
  {
    if (ray_y < proposed_y)
    {
      horizontal = false;
    }
    else if (ray_y > proposed_y)
    {
      horizontal = true;
    }
    else
    {
      throw;
    }
    
    break;
  }
  default:
    throw;
  }

  return horizontal;
}



// An obstacle has no edges while a free vertex does
bool Robot::IsFree(const VertexType vertex)
{
  return HasEdges(vertex);
}



bool Robot::HasEdges(const VertexType vertex)
{
  bool has_edges = true;

  const auto edge_range = boost::out_edges(vertex, map_.get_map());

  if (edge_range.first == edge_range.second)
  {
    has_edges = false;
  }

  return has_edges;
}



// Determining the relative coordinate of the read vertex given the robot's vertex and orientation
CoordinateType Robot::RelativeReadPos(const VertexType read_vertex)
{  
  CoordinateType robot_pos = map_.Coordinate(occupied_vertex_);
  CoordinateType read_pos = map_.Coordinate(read_vertex);
  
  return std::make_pair(
    read_pos.first - robot_pos.first,
    robot_pos.second - read_pos.second);
}



bool Robot::XIncrement(const CardinalDirection direction)
{
  bool x_increment = true;
  
  switch (direction)
  {
  case CardinalDirection::North:
  case CardinalDirection::South:
    x_increment = false;
    break;
  default:
    break;
  }
  
  return x_increment;
}

