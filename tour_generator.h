//
//  tour_generator.h
//  Proj4_TourGenerator
//
//  Created by Hunter Kang on 3/13/24.
//

#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H

#include "base_classes.h"

class TourGenerator: public TourGeneratorBase 
{
    public:
        TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
        virtual ~TourGenerator();
        virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
    private:
        // pointers to the geo database and router
        const GeoDatabaseBase* m_database;
        const RouterBase* m_router;
        // helper functions to convert angle to direction
        std::string getDir(double angle) const;
        bool getTurnDir(double angle, std::string& dir) const;

};

#endif /* TOUR_GENERATOR_H */


