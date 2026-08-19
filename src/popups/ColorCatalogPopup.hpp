#pragma once

#include "Includes.hpp"

class ColorCatalogPopup : public Popup {

private:

    Function<void(const ccColor3B&)> m_onSelect;

    ColorCatalogPopup(Function<void(const ccColor3B&)> onSelect)
        : m_onSelect(std::move(onSelect)) {}

    bool init() override;

public:

    static ColorCatalogPopup* create(Function<void(const ccColor3B&)>);

};