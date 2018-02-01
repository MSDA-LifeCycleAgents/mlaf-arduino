#pragma once

/**
 * @brief      Type of threshold to use
 */
enum PlanType : int
{
    Above = 0x1,
    Below = 0x2,
    Both = Above | Below
};

/**
 * @brief      Contains the definition of a plan
 */
struct Plan
{
    /**
     * @brief      The type of threshold to use
     */
    PlanType PlanThresholdType;
    /**
     * @brief      The below threshold for measurements
     */
    double ThresholdBelow;
    /**
     * @brief      The above threshold for measurements
     */
    double ThresholdAbove;
    /**
     * @brief      The message send when the plan conditions are met
     */
    String Message;
    /**
     * @brief      The agent to send via
     */
    String Via;
    /**
     * @brief      The recipient
     */
    String To;
    /**
     * @brief      The limit of messages send
     */
    size_t Limit;

    /**
     * @brief      Constructor
     *
     * @param[in]  pType           indicates which threshold to use (either or both)
     * @param[in]  thresholdBelow  the threshold for measurements below a value
     * @param[in]  thresholdAbove  the threshold for measurements above a value
     * @param[in]  msg             the message send when the plan conditions are met
     * @param[in]  via             the agent to send via
     * @param[in]  to              the recipient
     * @param[in]  limit           the limit of messages send
     *
     */
    Plan(PlanType pType, double thresholdBelow, double thresholdAbove, String msg, String via, String to, size_t limit)
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