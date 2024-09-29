//
//  router.h
//  Proj4_Router
//
//  Created by Hunter Kang on 3/12/24.


#ifndef ROUTER_H
#define ROUTER_H

#include "base_classes.h"
#include "hashmap.h"

class Router: public RouterBase {
    public:
        Router(const GeoDatabaseBase& geo_db);
        virtual ~Router();
        virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
    private:
        // member pointer to the database const as only reading from it not modifying
        const GeoDatabaseBase* m_database;
        struct Node { // node that stores the g, h, f, and GeoPoint
            Node(double g, double h, GeoPoint point)
            : m_g(g), m_h(h), m_f(g+h), m_point(point)
            {}
            double m_f; // total cost of GeoPoint
            double m_g; // relative path => dist from current to prev GeoPoint + dist of prev to start (basically how far away from the start w/ respect to the path
            double m_h; // heuristic used a linear hueristic from the current GeoPoint to the end GeoPoint
            GeoPoint m_point; // GeoPoint held at eachh node
        };
    
        struct lessThanOperator { // defined lessThanOperator that helps sort the auxillary queue
            bool operator()(const Node& p1, const Node& p2)
            {
                return p1.m_f > p2.m_f; // if the f_value is greater than it will be given less priority w/ the smallest f-value at the front of the queue
            }
        };
    
        // helper function that reverses the from the end to the start
        std::vector<GeoPoint> formatRoute(const GeoPoint& pt1, const GeoPoint& pt2, const HashMap<GeoPoint>& shortestDist) const;

};

#endif // ROUTER_H
