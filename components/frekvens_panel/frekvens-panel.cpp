#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"

#include "frekvens-panel.h"


namespace esphome {
namespace frekvenspanel {

static const char *const TAG = "frekvenspanel";


void Panel::initialize() {
  this->init_internal_(this->get_buffer_length_());
  this->panel = new FrekvensPanel(this->p_latch, this->p_clock, this->p_data);
}

int Panel::get_width_internal() { return 16; }
int Panel::get_height_internal() { return 16; }

size_t Panel::get_buffer_length_() {
  return size_t(this->get_width_internal()) * size_t(this->get_height_internal());
}

void HOT Panel::display() {
  uint8_t x, y, cell;
  ESP_LOGI(TAG, "Displaying...");

  this->panel->clear();
  for (x = 0; x < this->get_width_internal(); x++) {
    for (y = 0; y < this->get_height_internal(); y++) {
      cell = this->buffer_[(this->get_width_internal() * y) + x];
      this->panel->drawPixel(x, y, cell);
    }
  }
  this->panel->scan();
}

void HOT Panel::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x >= this->get_width_internal() || y >= this->get_height_internal() || x < 0 || y < 0) {
    return;
  }

  uint16_t pos = x + (y) * this->get_width_internal();
  uint8_t subpos = y % 8;

  if (color.is_on()) {
    this->buffer_[pos] |= 1;
  } else {
    this->buffer_[pos] &= ~1;
  }
}

void Panel::dump_config() {
  LOG_DISPLAY("", "Panel", this);
  LOG_PIN("  DC Pin: ", this->dc_pin_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  LOG_UPDATE_INTERVAL(this);
}

void Panel::update() {
  this->do_update_();
  this->display();
}

void Panel::fill(Color color) {
  uint8_t fill = color.is_on() ? 0xFF : 0x00;
  for (uint32_t i = 0; i < this->get_buffer_length_(); i++)
    this->buffer_[i] = fill;
}

}  // namespace frekvenspanel
}  // namespace esphome
