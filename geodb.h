//
//  main.cpp
//  proj4_GeoDatabase
//
//  Created by Hunter Kang on 3/11/24.
//

#ifndef GEODB_H
#define GEODB_H

#include <iostream>
#include <string>
#include "hashmap.h"
#include "geopoint.h"
#include "base_classes.h"

class GeoDatabase: public GeoDatabaseBase {
    public:
        GeoDatabase();
        virtual ~GeoDatabase();
        virtual bool load(const std::string& map_data_file); 
        virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
        virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
        virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
    private:
        // member HashMaps
        HashMap<GeoPoint> m_poi;
        HashMap<std::vector<GeoPoint>> m_connections;
        HashMap<std::string> m_streetName;
};

#endif // GEODB_H
