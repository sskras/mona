/*!
    \file  ProcessManager.h
    \brief class ProcessManager

    class ProcessManager

    Copyright (c) 2003 HigePon
    WITHOUT ANY WARRANTY

    \author  HigePon
    \version $Revision$
    \date   create:2003/07/06 update:$Date$
*/
#ifndef _MONA_PROCESS_MANAGER_
#define _MONA_PROCESS_MANAGER_

#include<types.h>
#include<global.h>
#include<ihandlers.h>
#include<PageManager.h>
#include<Scheduler.h>

/*!
    class ProcessManager
*/
class ProcessManager {

  public:
    ProcessManager(Process* idle);

    virtual ~ProcessManager() {
    }

  protected:
    PageEntry* allocatePageDir();
    virtual_addr allocateStack();
    virtual_addr allocateKernelStack(dword dpl);
    dword allocatePID();
    void printOneProcess(ProcessInfo* info) const;
    void switchProcess();

  public:
    void schedule();
    void tick();
    void sleep(ProcessInfo* process, dword tick);
    bool kill(ProcessInfo* process);
    dword getTick() const;
    void printAllProcesses() const;
    bool addProcess(Process* process, virtual_addr entry);
    bool addProcess(Process* process, PageEntry* directory, virtual_addr entry);

  private:
    dword pid_;
    dword pnum_;
    class Scheduler* scheduler_;
};

#endif
