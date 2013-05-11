//
//  LevelFileHandler.cpp
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#include "LevelFileHandler.h"
#include "tinyxml2.h"
#include "PineappleModel.h"
#include "RopeModel.h"
#include "Constants.h"

using namespace tinyxml2;

LevelFileHelper::LevelFileHelper(const char* levelFileName)
:_levelFile(levelFileName),
_ropes(NULL),
_pineapples(NULL)
{
    this->loadLevel();
}

LevelFileHelper::~LevelFileHelper()
{
    CC_SAFE_RELEASE_NULL(_pineapples);
    CC_SAFE_RELEASE_NULL(_ropes);
    
    CCLOG("~LevelFileHelper");
}

void LevelFileHelper::loadLevel()
{
    XMLDocument doc;
    
//    doc.LoadFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("test.xml").c_str());
//    const char* content= doc.FirstChildElement( "Hello" )->GetText();
//    printf( "Hello,%s\n", content );
    
    _pineapples = CCArray::create();
    _pineapples->retain();
    
    _ropes = CCArray::create();
    _ropes->retain();
    
    doc.LoadFile(_levelFile);
    
    XMLElement *level = doc.RootElement();
    XMLElement *pineaplle = level->FirstChildElement("pineapple");
    while (pineaplle) {
        int id = (int)pineaplle->IntAttribute("id");
        float x = pineaplle->FloatAttribute("x");
        float y = pineaplle->FloatAttribute("y");
        float damping = pineaplle->FloatAttribute("damping");
        
        PineappleModel *pm = new PineappleModel();
        pm->id = id;
        pm->position = ccp(x,y);
        pm->damping = damping > 0 ? damping : kDefaultDamping;
        
        _pineapples->addObject(pm);
        pm->release();
        
        //CCLOG("id = %d",id);
        
        
        pineaplle = pineaplle->NextSiblingElement("pineapple");
    }
    
    XMLElement *rope = level->FirstChildElement("rope");
    int ropeID = 1;
    
    while (rope) {
        RopeModel *rm = new RopeModel;
        
        rm->id = ropeID;
        
        //achorA
        const XMLElement* ropeChild = rope->FirstChildElement();
        int bodyAId = ropeChild->IntAttribute("body");
        rm->bodyAId = bodyAId;
        
        float ax;
        float ay;
        
        if (rm->bodyAId == -1) {
            ax = ropeChild->FloatAttribute("x");
            ay = ropeChild->FloatAttribute("y");
        }else{
            PineappleModel *pm = this->getPineappleWithID(rm->bodyAId);
            ax = pm->position.x;
            ay = pm->position.y;
        }
        
        rm->achorA = ccp(ax,ay);
        
        //achorB
        ropeChild = ropeChild->NextSiblingElement();
        int bodyBId = ropeChild->IntAttribute("body");
        rm->bodyBId = bodyBId;

        
        if (rm->bodyBId == -1) {
            ax = ropeChild->FloatAttribute("x");
            ay = ropeChild->FloatAttribute("y");
        }else{
            PineappleModel *pm = this->getPineappleWithID(rm->bodyBId);
            ax = pm->position.x;
            ay = pm->position.y;
        }
        
        rm->achorB = ccp(ax,ay);
        
        float sagity = rope->FloatAttribute("sagity");
        if (sagity > 0) {
            rm->sagity = sagity;
        }
        
        _ropes->addObject(rm);
        rm->release();
        
        ropeID++;
        
        
        //next rope
        rope = rope->NextSiblingElement("rope");
    }
        
}

PineappleModel* LevelFileHelper::getPineappleWithID(int id)
{
    return (PineappleModel*)this->getModelWithIDArray(id, _pineapples);
}

AbstractModel* LevelFileHelper::getModelWithIDArray(int id, cocos2d::CCArray *array)
{
    CCObject *obj;
    CCARRAY_FOREACH(array, obj)
    {
        AbstractModel* model = (AbstractModel*)obj;
        if (model->id == id) {
            return model;
        }
    }
    return NULL;
}

static bool compareScores(const void* p1, const void* p2)
{
    AbstractModel* am1 = (AbstractModel*)p1;
    AbstractModel* am2 = (AbstractModel*)p2;
    bool result = am1->id < am2->id;
    return result;
}

//根据id来排序
void LevelFileHelper::sortArrayById(cocos2d::CCArray *array)
{
    std::sort(array->data->arr,
              array->data->arr + array->data->num,
              compareScores);
}

int LevelFileHelper::findUnusedIdInArray(cocos2d::CCArray *array)
{
    int firstUnusedID = -1;
    int lastID = 0;
    CCObject *obj;
    
    CCARRAY_FOREACH(array, obj)
    {
        AbstractModel *am = (AbstractModel*)obj;
        if (am->id - lastID > 1) {
            firstUnusedID = lastID + 1;
            break;
        }
        lastID++;
    }
    
    if (firstUnusedID == -1) {
        firstUnusedID = lastID + 1;
    }
    
    return firstUnusedID;
}


PineappleModel* LevelFileHelper::addPineappleAt(cocos2d::CCPoint pt)
{
    int firstUnusedID = this->findUnusedIdInArray(_pineapples);
    PineappleModel *newPineapple = new PineappleModel;
    newPineapple->id = firstUnusedID;
    newPineapple->position = pt;
    _pineapples->addObject(newPineapple);
    
    return newPineapple;
}

