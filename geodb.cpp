//
//  geodb.cpp
//  proj4_GeoDatabase
//
//  Created by Hunter Kang on 3/11/24.
//

#include "geodb.h"
#include "hashmap.h"
#include "geopoint.h"
#include "geotools.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <vector>

GeoDatabase::GeoDatabase() {
    // nothing to do here
}

GeoDatabase::~GeoDatabase() {
    // nothing to do here
}

bool GeoDatabase::load(const std::string& map_data_file) {
    std::ifstream inf(map_data_file);
    // test if the file can open
    if ( ! inf)
    {
        std::cerr << "Cannot open map_data_file!" << std::endl;
        return 1;
    }
    // declare string vars to read into
    std::string streetName, longitude, latitude, poi;
    
    // while there is still a line to read
    while (getline(inf, streetName)) // read in streetname
    {
        // read in longitude and latitude for the first GeoPoint
        getline(inf, longitude, ' ');
        getline(inf, latitude, ' ');
        GeoPoint start(longitude, latitude);
        
        // read in longitude and latitude for the second GeoPoint
        getline(inf, longitude, ' ');
        getline(inf, latitude, '\n');
        GeoPoint end(longitude, latitude);
        
        // Add the connections
        m_connections[start.to_string()].push_back(end);
        m_connections[end.to_string()].push_back(start);
        
        // Add the street names
        m_streetName[start.to_string() + end.to_string()] = streetName;
        m_streetName[end.to_string() + start.to_string()] = streetName;
        
        // read in the number of poi
        getline(inf, poi);
        int numPoi = stoi(poi);
 
        // if there are no poi continue back to the start of the while loop
        if (numPoi == 0)
        {
            continue;
        }
        // declare a mid point
        GeoPoint mid = midpoint(start, end);
        
        // add streetnames for every possiblity to the midpoint
        m_streetName[start.to_string() + mid.to_string()] = streetName;
        m_streetName[mid.to_string() + start.to_string()] = streetName;
        m_streetName[mid.to_string() + end.to_string()] = streetName;
        m_streetName[end.to_string() + mid.to_string()] = streetName;

        // add connections for every possibility to the midpoint
        m_connections[start.to_string()].push_back(mid);
        m_connections[end.to_string()].push_back(mid);
        m_connections[mid.to_string()].push_back(start);
        m_connections[mid.to_string()].push_back(end);
        
        // while there are still poi
        while (numPoi > 0)
        {
            // read in the coordinates / name of poi
            std::string poiName;
            getline(inf, poiName, '|');
            getline(inf, longitude, ' ');
            getline(inf, latitude, '\n');
            GeoPoint poiPoint(longitude, latitude);
            
            // push in every combination of the poi street name
            m_streetName[mid.to_string() + poiPoint.to_string()] = "a path";
            m_streetName[poiPoint.to_string() + mid.to_string()] = "a path";
            
            // push in every combination of the poi connections
            m_connections[mid.to_string()].push_back(poiPoint);
            m_connections[poiPoint.to_string()].push_back(mid);
            
            // insert the poi
            m_poi.insert(poiName, poiPoint);
            numPoi--;
        }
    }
    return true;
}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    // define apointer to a GeoPoint and try to find the poi
    const GeoPoint* poi_pointer = m_poi.find(poi);
    if (poi_pointer != nullptr) // if it does not exist (ie nullptr) return false otherwise set point to the value and return true
    {
        point = *poi_pointer;
        return true;
    }
    return false;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    // same as get_poi_location but return an empty vector
    const std::vector<GeoPoint>* vector_pointer = m_connections.find(pt.to_string());
    if (vector_pointer == nullptr)
    {
        return std::vector<GeoPoint>();
    }
    return *vector_pointer;
}


std::string GeoDatabase::get_street_name(const GeoPoint& pt1,
        const GeoPoint& pt2) const
{
    // same as get_poi_location and get_street_name but return an empty string if doesn't exist
    const std::string* streetName_pointer = m_streetName.find(pt1.to_string() + pt2.to_string());
    if (streetName_pointer == nullptr) {
        return "";
    }
    return *streetName_pointer;
}




