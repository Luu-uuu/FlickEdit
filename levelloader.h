#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <vector>
#include "leveltile.h"

class levelLoader {
    private:

        std::vector< std::vector<unsigned char> > levelData;


        void setLevelData(std::vector< std::vector<unsigned char> > levelData);
        void levelParse(std::vector<unsigned char> rawLevelData);
        std::vector<unsigned char> levelAssemble(std::vector< std::vector<levelTile*> > floorQItems);

        //helper functions for decoding level binary makes it easier on the eyes in implemintaion file
        std::vector<unsigned char> loopUntilZero(std::vector<unsigned char>::const_iterator& rawLevelDataIterator
                                                ,std::vector<unsigned char>::const_iterator& end);
        std::vector<unsigned char> loopForBytes(std::vector<unsigned char>::const_iterator& rawLevelDataIterator,
                                                char numOfBytes);


    public:
        levelLoader();
        std::vector< std::vector<levelTile*> > floorQItems;
        bool loadLevel(std::string const& filepath);
        bool saveLevel(std::string const& filepath);
        std::vector< std::vector<unsigned char> > getLevelData();
        void toQItems();

};

#endif //LEVELLOADER_H
