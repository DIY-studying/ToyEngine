#include "Physics.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 只适用于横屏竖直（没有旋转角度）的矩形碰撞体积，并且只能判断相交的情况，有很大的局限性。
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static vec2 CheckBoundBox(const vec2& aPos, const vec2& bPos, const CBoundingBox& aBox, const CBoundingBox& bBox)
{
    float aLeft = aPos.x - aBox.halfSize.x, aRight = aPos.x + aBox.halfSize.x;
    float bLeft = bPos.x - bBox.halfSize.x, bRight = bPos.x + bBox.halfSize.x;
    float vDistance = std::abs(aPos.y - bPos.y);
    float hDistance = std::abs(aPos.x - bPos.x);
    if (aLeft > bRight && bLeft > aRight && vDistance < (aBox.halfSize.y + bBox.halfSize.y))
    {
        float xSize = (aBox.halfSize.x + bBox.halfSize.x) - hDistance;
        float ySize = (aBox.halfSize.y + bBox.halfSize.y) - vDistance;
        xSize = std::min({ xSize,aBox.size.x,bBox.size.x });
        ySize = std::min({ ySize,aBox.size.y,aBox.size.y });
        return vec2(xSize, ySize);
    }

    float aTop = aPos.y - aBox.halfSize.y, aDown = aPos.y + aBox.halfSize.y;
    float bTop = bPos.y - bBox.halfSize.y, bDown = bPos.y + bBox.halfSize.y;
    if (aTop < bDown && bTop < aDown && hDistance < (aBox.halfSize.x + bBox.halfSize.x))
    {
        float xSize = (aBox.halfSize.x + bBox.halfSize.x) - hDistance;
        float ySize = (aBox.halfSize.y + bBox.halfSize.y) - vDistance;
        xSize = std::min({ xSize,aBox.size.x,bBox.size.x });
        ySize = std::min({ ySize,aBox.size.y,aBox.size.y });
        return vec2(xSize, ySize);
    }

    return vec2(0, 0);
}

vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // TODO: return the overlap rectangle size of the bounding boxes of entity a and b

    const CTransform& aTransform = a->getComponent<CTransform>(),bTransform= b->getComponent<CTransform>();
    const CBoundingBox& aBox = a->getComponent<CBoundingBox>(), bBox = b->getComponent<CBoundingBox>();

    if (aTransform.has && aBox.has && bTransform.has && bBox.has)
    {
        return CheckBoundBox(aTransform.pos,bTransform.pos,aBox,bBox);
    }

    return vec2(0, 0);
}

vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
    //       previous overlap uses the entity's previous position

    const CTransform& aTransform = a->getComponent<CTransform>(), bTransform = b->getComponent<CTransform>();
    const CBoundingBox& aBox = a->getComponent<CBoundingBox>(), bBox = b->getComponent<CBoundingBox>();

    if (aTransform.has && aBox.has && bTransform.has && bBox.has)
    {
        return CheckBoundBox(aTransform.pos, bTransform.pos, aBox, bBox);
    }

    return vec2(0, 0);
}


