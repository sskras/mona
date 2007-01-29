#include "Application.h"
#include "Kernel.h"
#include "scheme.h"

using namespace std;
using namespace monash;

Application::Application(Object* function, Objects* arguments, uint32_t lineno) : function_(function), arguments_(arguments), lineno_(lineno)
{
}

Application::~Application()
{
}

string Application::toString()
{
    return "Application : " + function_->toString();
}

int Application::type() const
{
    return Object::APPLICATION;
}

Object* Application::eval(Environment* env)
{
    Object* procedure = function()->eval(env);
    if (!procedure->isCompoundProcedure() && !procedure->isPrimitiveProcedure() && !procedure->isContinuation())
    {
        RAISE_ERROR(lineno(), "invalid application [%s]", procedure->toString().c_str());
    }

    //Objects* as = arguments();
    return Kernel::apply(procedure, arguments(), env, parent, this); // parent, thisは継続のために必要
}

Object* Application::getContinuation(Object* calledPoint, Environment* env)
{
    // todo
    if (parent != NULL && parent->isLambda())
    {
        // todo
        Lambda* lambda = (Lambda*)parent;
        return lambda->getContinuation(this, env);
    }
    return NULL;
}
