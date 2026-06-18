#include <eMenu.h>



eMenu::eMenu(const Item* items, uint8_t size)
    : m_items(items), m_size(size), m_current_item(0), m_active_item(nullptr) {
}

void eMenu::reset() {
    m_current_item = 0;
    m_active_item = nullptr;
}
