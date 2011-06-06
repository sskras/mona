/*
Copyright (c) 2005 bayside

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "monagui.h"

namespace monagui {
    Container::Container()
    {
    }

    Container::~Container()
    {
        this->componentList.removeAll();
    }

    Component* Container::getComponent()
    {
        // NULLチェック
        int I = this->componentList.size();
        if (I == 0) return NULL;

        // 後ろからチェックしていく
        for (int i = I - 1; i >= 0; i--) {
            Component* c = (Component *)this->componentList.get(i);
            if (c->getFocused() == true) return c;
        }
        return NULL;
    }

    Component* Container::getComponentAt(int x, int y)
    {
        // NULLチェック
        int I = this->componentList.size();
        if (I == 0) return NULL;

        // 後ろからチェックしていく
        for (int i = I - 1; i >= 0; i--) {
            Component* c = (Component *)this->componentList.get(i);
            Rectangle* bounds = c->getBounds();
            // マウスカーソルがある範囲に部品があるかどうかチェック
            if (bounds->x <= x && x <= bounds->x + bounds->width &&
                bounds->y <= y && y <= bounds->y + bounds->height)
            {
                return c;
            }
        }
        return NULL;
    }

    void Container::add(Component* component)
    {
        component->setParent(this);
        component->addNotify();
        this->componentList.add(component);
    }

    void Container::remove(Component* component)
    {
        this->componentList.remove(component);
    }

    void Container::dispatchEvent(Event* event)
    {
        // 非活性の時はイベントを受け付けない
        if (getEnabled() == false) return;

        // 活性部品にキーイベントを投げる
        if (event->getType() == KeyEvent::KEY_PRESSED || event->getType() == KeyEvent::KEY_RELEASED) {
            Component* component = getComponent();
            // 部品でイベントが起こった
            if (component != NULL) {
                event->setSource(component);
                component->processEvent(event);
            }
            // 部品以外でイベントが起こった
            processEvent(event);
        // マウスクリック
        } else if (event->getType() == MouseEvent::MOUSE_PRESSED) {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            MouseEvent* me = (MouseEvent *)event;
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // マウスイベントが起こった部品を探す
            Component* component = getComponentAt(me->getX(), me->getY());
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // 部品でイベントが起こった
            if (component != NULL) {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                // イベントが起こった部品以外をフォーカスアウト状態にする
                int I = this->componentList.size();
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                for (int i = 0; i < I; i++) {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                    Component* c = (Component *)this->componentList.get(i);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                    if (c != component) {
                        c->setFocused(false);
                    }
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                }
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                component->setFocused(true);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                event->setSource(component);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                Rectangle* bounds = component->getBounds();
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                me->setX(me->getX() - bounds->x);
                me->setY(me->getY() - bounds->y);
                //syscall_print("MOUSE_PRESSED,");
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                component->processEvent(event);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // 部品以外でイベントが起こった
            } else {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                // 部品をフォーカスアウト状態にする
                int I = this->componentList.size();
                for (int i = 0; i < I; i++) {
                    Component* c = (Component *)this->componentList.get(i);
                    c->setFocused(false);
                }
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                event->setSource(this);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                processEvent(event);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            }
        // マウスクリック以外のマウスイベント
        } else if (event->getType() == MouseEvent::MOUSE_RELEASED ||
                    event->getType() == MouseEvent::MOUSE_DRAGGED ||
                    event->getType() == MouseEvent::MOUSE_MOVED)
        {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            MouseEvent* me = (MouseEvent *)event;
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // マウスイベントが起こった部品を探す
            Component* component = getComponentAt(me->getX(), me->getY());
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // 部品でイベントが起こった
            if (component != NULL) {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                event->setSource(component);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                Rectangle* bounds = component->getBounds();
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                me->setX(me->getX() - bounds->x);
                me->setY(me->getY() - bounds->y);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                component->processEvent(event);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            // 部品以外でイベントが起こった
            } else {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                event->setSource(this);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
                processEvent(event);
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            }
        } else {
            processEvent(event);
        }
    }

    void Container::repaint()
    {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        if (getBuffer() == NULL) return;
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        ASSERT(getGraphics()); // don't use getGraphics(), before add()
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        paint(getGraphics());
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        // 自分の領域を更新する
        update();
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        // 子部品を再描画する
        int I = this->componentList.size();
        for(int i = 0; i < I; i++) {
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            Component* component = (Component *)(componentList.get(i));
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
            component->repaint();
        _logprintf("%s %s:%d thread(%d)\n", __func__, __FILE__, __LINE__, MonAPI::System::getThreadID());
        }
    }
}
