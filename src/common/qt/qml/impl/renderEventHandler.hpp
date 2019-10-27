//
//  Created by Bradley Austin Davis on 2018-01-04
//  Copyright 2013-2018 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#pragma once

#if defined(HAVE_QT)

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLContext>
#include <QtGui/qevent.h>

namespace xr_examples { namespace qml { namespace impl {

class SharedObject;

class OffscreenEvent : public QEvent {
public:
    enum Type {
        Initialize = QEvent::User + 1,
        Render,
        RenderSync,
        Quit
    };

    OffscreenEvent(Type type) : QEvent(static_cast<QEvent::Type>(type)) {}
};

/* The render event handler lives on the QML rendering thread for a given surface
 * (each surface has a dedicated rendering thread) and handles events of type 
 * OffscreenEvent to do one time initialization or destruction, and to actually 
 * perform the render.  
 */
class RenderEventHandler : public QObject {
public:
    RenderEventHandler(SharedObject* shared, QThread* targetThread);

private:
    bool event(QEvent* e) override;
    void onInitalize();
    void onRender();
    void onRenderSync();
    void qmlRender(bool sceneGraphSync);
    void onQuit();

    SharedObject* const _shared;
    QOpenGLContext _context;
    uint32_t _fbo{ 0 };
    uint32_t _depthBuffer{ 0 };
    QOffscreenSurface _surface;
    bool _initialized { false };
};

}}}  // namespace hifi::qml::impl

#endif