#pragma once

#include "MonoBehaviour.h"
#include <string>

class UILayer : public MonoBehaviour
{
    DTGENERATED_BODY(UILayer);

public:
    UILayer() = default;
    ~UILayer() override = default;

    void SetLayerName(const std::string& name);
    const std::string& GetLayerName() const { return m_layerName; }

    void SetLayerOrder(int order);
    int GetLayerOrder() const { return m_layerOrder; }

private:
    std::string m_layerName = "Default";
    int m_layerOrder = 0;
};
