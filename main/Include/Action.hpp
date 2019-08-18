//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef ACTION_HPP_INCLUDED
#define ACTION_HPP_INCLUDED

#include <cstdint>
#include <cstdlib>
#include <list>
//==============================================================================
// Public defines and constants
//==============================================================================
class Store;

//==============================================================================
// Public variables
//==============================================================================

class Action
{
public:
    //==========================================================================
    // Public typedefs
    //==========================================================================
    enum class Source
    {
        NA,
        Auto,
        Motion,
        Internal,
        Ble,
        Cloud,
        Ir,
        WiFi,
        Light,
        Host
    };

    //==========================================================================
    // Public function prototypes
    //==========================================================================
    virtual void Accept(Store &) = 0;
    Source       GetSource() const;
    void         SetSource(Source source);
    bool         GetIsUpdate() const;
    void         SetIsUpdate(bool update);

    virtual void ReleaseResources();
    virtual void AddAllocatedResource(void *);
    char *       AddAllocatedStringResource(const char *source);
    void         SetSequenceNumber(uint32_t);
    uint32_t     GetSequenceNumber() const;
    virtual ~Action() = default;

protected:
    Action() = default;

    static constexpr bool isUserChange(const Source source)
    {
        return source > Source::Internal;
    }

private:
    std::list<void *> *mAllocatedResources{nullptr};
    Source             mSource{Source::NA};
    bool               mIsUpdate{false};
    uint32_t           mSequenceNumber{0};
};

#endif    // ACTION_HPP_INCLUDED
