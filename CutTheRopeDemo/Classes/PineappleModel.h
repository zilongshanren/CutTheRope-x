//
//  PineappleModel.h
//  CutTheRopeDemo
//
//  Created by guanghui on 5/10/13.
//
//

#ifndef __CutTheRopeDemo__PineappleModel__
#define __CutTheRopeDemo__PineappleModel__

#include "AbstractModel.h"

#define kDefaultDamping 0.3


class PineappleModel : public AbstractModel
{
public:
    PineappleModel();
    CCPoint position;
    float damping;
};

#endif /* defined(__CutTheRopeDemo__PineappleModel__) */
