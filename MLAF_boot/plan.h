#pragma once

enum PlanType : int
{
    Above = 0x1,
    Below = 0x2,
    Both = Above | Below
};

struct Plan
{
    PlanType PlanThresholdType;
    double ThresholdBelow;
    double ThresholdAbove;
    const char* Message;
    const char* Via;
    const char* To;
    size_t Limit;

    Plan(PlanType pType, double thresholdBelow, double thresholdAbove, const char* msg, const char* via, const char* to, size_t limit)
    //: this()
    {
        PlanThresholdType = pType;
        ThresholdBelow = thresholdBelow;
        ThresholdAbove = thresholdAbove;
        Message = msg;
        Via = via;
        To = to;
        Limit = limit;
    }
};