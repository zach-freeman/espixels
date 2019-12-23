//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef TASK_REGISTRY_INCLUDED
#define TASK_REGISTRY_INCLUDED

#include <list>

#include "Singleton.hpp"
//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================
class OsTask;
//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================
class TaskRegistry : public Singleton<TaskRegistry>
{
    friend Singleton<TaskRegistry>;

public:
    void AddTask(OsTask *task);
    void StartTasks();

private:
    TaskRegistry() = default;
    std::list<OsTask *> mTaskList;
};
#endif    // TASK_REGISTRY_INCLUDED
