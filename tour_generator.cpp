//
//  main.cpp
//  Proj4_TourGenerator
//
//  Created by Hunter Kang on 3/13/24.
//
#include "tour_generator.h"
#include "geotools.h"

// TourGenerator constructor that initializes the pointers to their respective types
TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
: m_database(&geodb), m_router(&router)
{}

// Destructor no dynamically alloated objects so need to delete
TourGenerator::~TourGenerator()
{}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
    std::vector<TourCommand> results;
    
    // loop through stops and read in every command
    for (int i = 0; i < stops.size(); i++)
    {
        // Read commands into local variables
        std::string poi = "";
        std::string talking_points = "";
        stops.get_poi_data(i, poi, talking_points);

        // generate a command and add to the tourcommand
        TourCommand command;
        command.init_commentary(poi, talking_points);
        results.push_back(command);
        
        // check if the next point of interest exists
        if (stops.get_poi_data(i+1, poi, talking_points))
        {
            // convert the strings to GeoPoints
            GeoPoint poi1;
            GeoPoint poi2;
            m_database->get_poi_location(command.get_poi(), poi1);
            m_database->get_poi_location(poi, poi2);

            // find a route between the two GeoPoints
            std::vector<GeoPoint> routeInstr = m_router->route(poi1, poi2);
            
            // Generate a proceed command
            std::string direction = "";
            std::string street_name = "";
            double distance = 0;
            
            for (int j = 0; j != routeInstr.size()-1; j++)
            {
                GeoPoint pt1 = routeInstr[j];
                GeoPoint pt2 = routeInstr[j+1];
                // calculate the direction
                double angle = angle_of_line(pt1, pt2);
                direction = getDir(angle);
                //calculate the street_name
                street_name = m_database->get_street_name(pt1, pt2);
                // calculate the distance
                distance = distance_earth_miles(pt1, pt2);
                
                // create / insert a proceed command
                command.init_proceed(direction, street_name, distance, pt1, pt2);
                results.push_back(command);
                
                // check if should add a turn command
                if (j + 2 != routeInstr.size()) // checks if there is a third GeoPoint
                {
                    GeoPoint pt3 = routeInstr[j+2];
                    if (street_name != m_database->get_street_name(pt2, pt3)) // checks for different streetnames
                    {
                        angle = angle_of_turn(pt1, pt2, pt3);
                        if (getTurnDir(angle, direction)) // checks the angle
                        {
                            // add a turn command
                            street_name = m_database->get_street_name(pt2, pt3);
                            command.init_turn(direction, street_name);
                            results.push_back(command);
                        }
                    }
                }
            }
        } // end of if statement
    } // end of for loop
    return results;
}

// converts angle to appropriate distance
std::string TourGenerator::getDir(double angle) const
{
    /* ○ 0 <= angle < 22.5: east
     ○ 22.5 <= angle < 67.5: northeast
     ○ 67.5 <= angle < 112.5: north
     ○ 112.5 <= angle < 157.5: northwest
     ○ 157.5 <= angle < 202.5: west
     ○ 202.5 <= angle < 247.5: southwest
     ○ 247.5 <= angle < 292.5: south
     ○ 292.5 <= angle < 337.5: southeast
     ○ angle >= 337.5: east */
    if (angle < 22.5) {
        return "east";
    }
    else if (angle < 67.5) {
        return "northeast";
    }
    else if (angle < 112.5) {
        return "north";
    }
    else if (angle < 157.5) {
        return "northwest";
    }
    else if (angle < 202.5) {
        return "west";
    }
    else if (angle < 247.5) {
        return "southwest";
    }
    else if (angle < 292.5) {
        return "south";
    }
    else if (angle < 337.5) {
        return "southeast";
    }
    else {
        return "east";
    }
}

// converts angle to appropriate direction for a turn
bool TourGenerator::getTurnDir(double angle, std::string& dir) const
{
    if (angle >= 1 && angle < 180) {
        dir = "left";
        return true;
    }
    else if (angle >= 180 && angle <= 360) {
        dir = "right";
        return true;
    }
    else {
        return false;
    }
}
