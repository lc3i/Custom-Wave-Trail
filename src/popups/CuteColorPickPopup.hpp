#pragma once

#include "Includes.hpp"

#include <Geode/ui/Button.hpp>

class CuteColorPickPopup : public Popup, public ColorPickerDelegate {

private:

    Function<void(const ccColor3B&)> m_callback;
    Button* m_colorButton = nullptr;
    TextInput* m_input = nullptr;
    bool m_ignore = false;

    CuteColorPickPopup(Function<void(const ccColor3B&)> callback)
        : m_callback(std::move(callback)) {}

    bool init(const ccColor3B&);

    void colorValueChanged(ccColor3B) override;
    void onClose(CCObject*) override;

public:

static CuteColorPickPopup* create(const ccColor3B&, Function<void(const ccColor3B&)>);

};