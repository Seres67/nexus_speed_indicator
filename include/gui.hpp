#ifndef GUI_HPP
#define GUI_HPP

#include <deque>

extern std::deque<std::pair<double, float>> speed_data;

void render_window();
void render_options();

#endif // GUI_HPP
