//
//  router.cpp
//  Proj4_Router
//
//  Created by Hunter Kang on 3/12/24.
//
//
#include <stdio.h>
#include <cmath>
#include <queue>
#include <vector>
#include <set>
#include "router.h"
#include <map>
#include "geotools.h"

// Constructor that initializes pointer member to the address of the geo_db
Router::Router(const GeoDatabaseBase& geo_db)
: m_database(&geo_db)
{
}

// Destructor no dynamically allocated object so no need for deletion
Router::~Router()
{}


std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    // Data structures
    std::priority_queue<Node, std::vector<Node>, lessThanOperator> pointsToCheck; // priority queue w/ lowest f_value in front
    HashMap<GeoPoint> visitedPoints; // stores the GeoPoints that have been checked
    HashMap<GeoPoint> locationOfPrevWayPoint; // maps to the prev GeoPoint or the parent GeoPoint
    HashMap<double> gValues;
    
    // Starting node
    Node startNode(0, 0, pt1);
    gValues[pt1.to_string()] = 0;
    pointsToCheck.push(startNode);
    
    // if there are still points to check
    while (!pointsToCheck.empty())
    {
        // Gets the top node
        Node current = pointsToCheck.top();
        pointsToCheck.pop();
        visitedPoints[current.m_point.to_string()] = current.m_point;
        
        // if the top node is the end then trace back and return the vector after reformatting
        if (current.m_point.to_string() == pt2.to_string()) {
            return this->formatRoute(pt1, pt2, locationOfPrevWayPoint);
        }
        
        // generate children
        std::vector<GeoPoint> connections = m_database->get_connected_points(current.m_point);
        for (int i = 0; i != connections.size(); i++)
        {
            // if already visited then continue to next children, shortest route to these children has alr been determined
            if (visitedPoints.find(connections[i].to_string()) != nullptr)
            {
                continue;
            }
            
            // calculate the g-values and h-value of from the current to the child
            Node child(current.m_g + distance_earth_miles(current.m_point, connections[i]), distance_earth_miles(connections[i], pt2), connections[i]);
            
            // if the gValue does not exist push it in. If it does check if the current one is smaller and if it is replace it with the better route && update g value to the smaller one
            if (gValues.find(connections[i].to_string()) != nullptr)
            {
                if (child.m_g >= *gValues.find(connections[i].to_string()))
                {
                    continue;
                }
            }
            
            // insert and update respective data structures
            locationOfPrevWayPoint[child.m_point.to_string()] = current.m_point;
            gValues[child.m_point.to_string()] = child.m_g;
            pointsToCheck.push(child);
        }
    }
    return std::vector<GeoPoint>(); // return empty if no route is found
}

std::vector<GeoPoint> Router::formatRoute(const GeoPoint& pt1, const GeoPoint& pt2, const HashMap<GeoPoint>& shortestDist) const
{
    std::vector<GeoPoint> shortestRoute;
    std::list<GeoPoint> auxillaryList;
    
    // pushes in the last GeoPoint
    auxillaryList.push_front(pt2);
    
    // from the last GeoPoint map to the first GeoPoint pushing it all to a list in the sorted order then pushing that into a vector effectively reversing it
    GeoPoint prevNodeOf = pt2;
    while (shortestDist.find(prevNodeOf.to_string()) != nullptr)
    {
        prevNodeOf = *shortestDist.find(prevNodeOf.to_string());
        auxillaryList.push_front(prevNodeOf);
    }
    
    for (std::list<GeoPoint>::iterator p = auxillaryList.begin(); p != auxillaryList.end(); p++)
    {
        shortestRoute.push_back(*p);
    }
    return shortestRoute;
}




