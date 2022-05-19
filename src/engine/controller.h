#pragma once

class Engine;

class Controller {
public:
    virtual ~Controller() = default;

    virtual void OnAttach(Engine *engine) {}

    /**
     * Updates controller.
     *
     * @param ts Time step in seconds.
     */
    virtual void Update(float ts) {}
};