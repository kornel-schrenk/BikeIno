#include "screens/BikeInoScreen.h"

BikeInoScreen::BikeInoScreen() {
    _screen = nullptr;
    _contentPane = nullptr;
}

BikeInoScreen::~BikeInoScreen() {
    destroyUI();
}

void BikeInoScreen::createUI() {
    // Base implementation - creates the screen and content pane
    createBaseLayout();
}

void BikeInoScreen::destroyUI() {
    if (_screen != nullptr) {
        lv_obj_delete(_screen);
        _screen = nullptr;
        _contentPane = nullptr;
    }
}

void BikeInoScreen::update() {
    // Base implementation does nothing
    // Derived classes override this to update their content
}

lv_obj_t* BikeInoScreen::getScreen() {
    return _screen;
}

lv_obj_t* BikeInoScreen::getContentPane() {
    return _contentPane;
}

bool BikeInoScreen::isCreated() {
    return _screen != nullptr;
}

void BikeInoScreen::createBaseLayout() {
    // Create the LVGL screen
    _screen = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(_screen, COLOR_BACKGROUND, 0);

    // Create the content pane (left side, 240px)
    _contentPane = lv_obj_create(_screen);
    lv_obj_set_size(_contentPane, CONTENT_PANE_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_pos(_contentPane, 0, 0);
    lv_obj_set_style_bg_color(_contentPane, COLOR_BACKGROUND, 0);
    lv_obj_set_style_border_width(_contentPane, 0, 0);
    lv_obj_set_style_radius(_contentPane, 0, 0);
    lv_obj_set_style_pad_all(_contentPane, 8, 0);
    lv_obj_clear_flag(_contentPane, LV_OBJ_FLAG_SCROLLABLE);
}
