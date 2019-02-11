#include "levelloader.h"
#include <iostream>
#include <fstream>
#include <QDebug>


void levelLoader::setLevelData(std::vector< std::vector<unsigned char> > levelDataIn){
    levelData = levelDataIn;
    return;
}


//convert leveldata to QtGraphic Items to display in editor. currently only does PLATFORM/EMPTY data
void levelLoader::toQItems(){

    unsigned int levelIndex = 0;
    unsigned char numberOfTiles = 0;
    unsigned int levelSize = floorQItems.size()*floorQItems[0].size();

    //


    for (std::vector<unsigned char>::const_iterator i=levelData[PLATFORMS].begin(); i<levelData[PLATFORMS].end();i++){
        //qDebug() << *i <<endl;
        if(*i){

            //7th bit represents solid tile
            if(*i >> 7){
                numberOfTiles = (*i & 0x3F);
                //6th bit represents if tiles are vertical or horizontal
                if(!((*i >> 6) & 0x1)){
                    for(unsigned char k=0; levelIndex < levelSize && k < numberOfTiles; k++,levelIndex++){
                        //its possible vertical blocks were already created in this space, dont create another block AKA memoryleak
                        if(!(floorQItems[levelIndex%32][levelIndex/32]))
                            floorQItems[levelIndex%32][levelIndex/32] = new levelTile(PLATFORMS);
                    }
                }else{
                    //when dealing with vertical tile creation levelIndex only increases by 1 while setting vertical blocks
                    for(unsigned char k=0; (levelIndex/32+k)<24 && levelIndex<levelSize && k < numberOfTiles; k++)
                    {
                        if(!(floorQItems[levelIndex%32][levelIndex/32+k]))
                            floorQItems[levelIndex%32][levelIndex/32+k] = new levelTile(PLATFORMS);
                    }
                    levelIndex++;
                }
            }else{
                numberOfTiles = (*i & 0x7F);

                levelIndex += numberOfTiles;
            }
        }
    }
}

levelLoader::levelLoader(){

    //a flicky level is made of 32 by 24 units not counting floors/ceilings
    for (int i=0; i<32; i++)
        floorQItems.push_back(std::vector<levelTile*>(24));
}

std::vector<unsigned char> levelLoader::loopUntilZero(std::vector<unsigned char>::const_iterator& rawLevelDataIterator, std::vector<unsigned char>::const_iterator& end){
    std::vector<unsigned char> levelChunk;

    for( ;rawLevelDataIterator != end; rawLevelDataIterator++){
            
        if( !*rawLevelDataIterator )
            break;
        levelChunk.push_back(*rawLevelDataIterator);

    }
    levelChunk.push_back(0);

    if(rawLevelDataIterator != end)
        rawLevelDataIterator++;

    return levelChunk;
}

std::vector<unsigned char> levelLoader::loopForBytes(std::vector<unsigned char>::const_iterator& rawLevelDataIterator, char numOfBytes){
    std::vector<unsigned char> levelChunk;

    for(char k=0; k<numOfBytes; k++, rawLevelDataIterator++){
        levelChunk.push_back(*rawLevelDataIterator);
    }

    return levelChunk;
}

void levelLoader::levelParse(std::vector<unsigned char> rawLevelData){
    
    //no level loaded yet shouldnt ever happen
    if (rawLevelData.size() == 0)
        return;


    std::vector< std::vector<unsigned char> > filteredLevelData;
    std::vector<unsigned char> levelChunk;

    std::vector<unsigned char>::const_iterator i = rawLevelData.begin(), end=rawLevelData.end();
    
    //get platform level data
    levelChunk = loopUntilZero(i, end);
    filteredLevelData.push_back(levelChunk);

    //get exit door x and y coordinates
    levelChunk = loopForBytes(i,2);
    filteredLevelData.push_back(levelChunk);

    //get both cat spawners x and y
    levelChunk = loopForBytes(i,4);
    filteredLevelData.push_back(levelChunk);
    
    //get x and y for special red mirror background decoration
    levelChunk = loopForBytes(i,2);
    filteredLevelData.push_back(levelChunk);
    levelChunk.clear();

    //get number of regular  mirror background decorations
    unsigned char numOfObj = (*i);
    i++;
    
    //get x and y of each mirror
    levelChunk = loopForBytes(i,numOfObj*2);
    levelChunk.insert(levelChunk.begin(),numOfObj);

    filteredLevelData.push_back(levelChunk);
    levelChunk.clear();
    
    //get number of large extra background objects
    numOfObj = (*i);
    i++;

    //get x and y of each large object
    levelChunk = loopForBytes(i, numOfObj*2+1);
    levelChunk.insert(levelChunk.begin(),numOfObj);

    filteredLevelData.push_back(levelChunk);
    levelChunk.clear();

    //get throwing weapons x and y always 6 weapons per level
    levelChunk = loopForBytes(i, 12);

    filteredLevelData.push_back(levelChunk);
    levelChunk.clear();

    //get number of regular chirps
    numOfObj = (*i);
    i++;
    
    //get x and y of each chirp
    levelChunk = loopForBytes(i,numOfObj*2);
    levelChunk.insert(levelChunk.begin(),numOfObj);

    filteredLevelData.push_back(levelChunk);
    levelChunk.clear();

    //number of chirps with sunglasses and 0x00 representing end of level data
    numOfObj = (*i);
    i++;

    if(numOfObj)
        levelChunk=loopForBytes(i,numOfObj*2+1);
    levelChunk.insert(levelChunk.begin(),numOfObj);

    filteredLevelData.push_back(levelChunk);
    setLevelData(filteredLevelData);

}

std::vector<unsigned char> levelLoader::levelAssemble(std::vector< std::vector<levelTile*> > floorQItems){

    unsigned int numberOfTiles = 0;
    unsigned int levelSize = floorQItems.size()*floorQItems[0].size();
    unsigned int levelIndex = 0;

    unsigned char maxSolidTilesPerByte = 0x3F;
    unsigned char maxEmptyTilesPerByte = 0x7F;

    dataType typeOfTile;

    std::vector<unsigned char> levelBinaryData;
    typeOfTile = floorQItems[levelIndex%32][levelIndex/32]->getTileType();

    for(;levelIndex < levelSize; levelIndex++){
        if(typeOfTile == floorQItems[levelIndex%32][levelIndex/32]->getTileType())
        {
            numberOfTiles++;
        }else{
            //we got a different type of tile
            if(typeOfTile == PLATFORMS)
            {
                if (numberOfTiles <= maxSolidTilesPerByte){
                    levelBinaryData.push_back(numberOfTiles | 0x80);
                }else{
                    while(numberOfTiles > maxSolidTilesPerByte){
                        levelBinaryData.push_back(maxSolidTilesPerByte | 0x80);
                        numberOfTiles-=maxSolidTilesPerByte;
                    }
                    //if there is still some tiles left push them back
                    if(numberOfTiles)
                        levelBinaryData.push_back(numberOfTiles | 0x80);
                }
                typeOfTile = floorQItems[levelIndex%32][levelIndex/32]->getTileType();
                numberOfTiles = 1;
            }else if(typeOfTile == EMPTY){
                if(numberOfTiles <= maxEmptyTilesPerByte){
                    levelBinaryData.push_back(numberOfTiles);
                }else{
                    while(numberOfTiles > maxEmptyTilesPerByte){
                        levelBinaryData.push_back(maxEmptyTilesPerByte);
                        numberOfTiles-=maxEmptyTilesPerByte;
                    }
                    if(numberOfTiles)
                        levelBinaryData.push_back(numberOfTiles);
                }
                typeOfTile = floorQItems[levelIndex%32][levelIndex/32]->getTileType();
                numberOfTiles = 1;
            }

        }
    }
    levelBinaryData.push_back(0);

    return levelBinaryData;

}

bool levelLoader::loadLevel(std::string const& filepath){
    std::fstream levelFile;
    int levelFileSize = 0;
    levelFile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
    levelFile.unsetf(std::ios::skipws);
    levelFile.seekg(0, std::ios::end);
    levelFileSize = levelFile.tellg();
    levelFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> rawLevelData;
    rawLevelData.insert(rawLevelData.begin(),
                  std::istream_iterator<unsigned char>(levelFile),
                  std::istream_iterator<unsigned char>());


    levelParse(rawLevelData);
    levelFile.close();
    return true;
}

bool levelLoader::saveLevel(std::string const& filepath){

    std::fstream saveFile;
    saveFile.open(filepath, std::fstream::out|std::ios::binary|std::ios::trunc);
    std::vector<unsigned char> levelBinaryData = levelAssemble(floorQItems);
    levelData[PLATFORMS] = levelBinaryData;

    //:^) better this implementation

    for(int i=0; i<levelData[PLATFORMS].size(); i++){
        saveFile.write( (char*)&levelData[PLATFORMS][i], sizeof(char) );
    }

    for(int i=0; i<levelData[EXIT_DOOR].size(); i++){
        saveFile.write( (char*)&levelData[EXIT_DOOR][i], sizeof(char) );
    }

    for(int i=0; i<levelData[CATFLAP].size(); i++){
        saveFile.write( (char*)&levelData[CATFLAP][i], sizeof(char) );
    }

    for(int i=0; i<levelData[SPECIAL_BG_DECOR].size(); i++){
        saveFile.write( (char*)&levelData[SPECIAL_BG_DECOR][i], sizeof(char) );
    }

    for(int i=0; i<levelData[BG_DECOR].size(); i++){
        saveFile.write( (char*)&levelData[BG_DECOR][i], sizeof(char) );
    }

    for(int i=0; i<levelData[LARGE_BG_DECOR].size(); i++){
        saveFile.write( (char*)&levelData[LARGE_BG_DECOR][i], sizeof(char) );
    }

    for(int i=0; i<levelData[WEAPONS].size(); i++){
        saveFile.write( (char*)&levelData[WEAPONS][i], sizeof(char) );
    }

    for(int i=0; i<levelData[CHIRPS].size(); i++){
        saveFile.write( (char*)&levelData[CHIRPS][i], sizeof(char) );
    }

    for(int i=0; i<levelData[COOL_CHIRPS].size(); i++){
        saveFile.write( (char*)&levelData[COOL_CHIRPS][i], sizeof(char) );
    }
    //
    saveFile.close();
    return true;
}


std::vector< std::vector<unsigned char> > levelLoader::getLevelData(){
    return levelData;
}
