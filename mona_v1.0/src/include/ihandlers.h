/*!
    \file  ihandlers.h
    \brief interrupt handlers

    definition for IDT
    Copyright (c) 2003 HigePon
    WITHOUT ANY WARRANTY

    \author  HigePon
    \version $Revision$
    \date   create:2003/02/24 update:$Date$
*/
#ifndef _MONA_IHANDLER_
#define _MONA_IHANDLER_

#include <kthread.h>

extern "C" void arch_fdchandler(void);
extern "C" void arch_timerhandler(void);
extern "C" void arch_keystrokehandler(void);
extern "C" void arch_dummyhandler(void);
extern "C" void arch_kthread_switch(void);
extern "C" void arch_fault0dhandler(void);
extern "C" void arch_syscall_handler(void);

extern "C" void MFDCHandler(void);
extern "C" void timerHandler(void);
extern "C" void keyStrokeHandler(void);
extern "C" void dummyHandler(void);
extern "C" void fault0dHandler(void);

/* expr:IRQ Handler */
#define IRQHANDLERHEADER(x) extern "C" void irqHandler_##x(void); extern "C" void arch_irqhandler_##x(void)
IRQHANDLERHEADER(0);
IRQHANDLERHEADER(1);
IRQHANDLERHEADER(2);
IRQHANDLERHEADER(3);
IRQHANDLERHEADER(4);
IRQHANDLERHEADER(5);
IRQHANDLERHEADER(6);
IRQHANDLERHEADER(7);
IRQHANDLERHEADER(8);
IRQHANDLERHEADER(9);
IRQHANDLERHEADER(10);
IRQHANDLERHEADER(11);
IRQHANDLERHEADER(12);
IRQHANDLERHEADER(13);
IRQHANDLERHEADER(14);
IRQHANDLERHEADER(15);



#endif
