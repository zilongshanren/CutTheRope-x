//
//  LevelFileHandler.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#ifndef __CutTheRopeDemo__LevelFileHandler__
#define __CutTheRopeDemo__LevelFileHandler__

#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

class RopeModel;
class PineappleModel;
class AbstractModel;

class LevelFileHelper : public CCObject
{
public:
    LevelFileHelper(const char* levelFileName);
    virtual ~LevelFileHelper();
    
    CCArray *_pineapples;
    CCArray *_ropes;
    PineappleModel* getPineappleWithID(int id);
    AbstractModel* getModelWithIDArray(int id, CCArray* array);
    
    void sortArrayById(CCArray *array);
    int findUnusedIdInArray(CCArray *array);
    
    PineappleModel* addPineappleAt(CCPoint pt);
    void addRopeFromModel(RopeModel *rm);
    void removeRopeWithID(int id);
    void removePineappleWithID(int id);
    
    void moveRopeWithId(int ropeID, anchorType type,int anchorID, CCPoint pt);
    void movePineappleWithID(int pineappleID, CCPoint pt);
    
    void saveFile();
    void reset();
    
private:
    LevelFileHelper();
    const char* _levelFile;
    
    void loadLevel();
};

#endif /* defined(__CutTheRopeDemo__LevelFileHandler__) */
