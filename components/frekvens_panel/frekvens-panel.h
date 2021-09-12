#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

#include "frekvens-driver.h"


namespace esphome {
namespace frekvenspanel {

class Panel : public PollingComponent,
                public display::DisplayBuffer {
 public:
  int p_latch;
  int p_clock;
  int p_data;

  void set_pins(int latch, int clock, int data) {
    this->p_latch = latch;
    this->p_clock = clock;
    this->p_data = data;
  }

  float get_setup_priority() const override { return setup_priority::PROCESSOR; }

  void data(uint8_t value);

  void initialize();
  void dump_config() override;
  void HOT display();

  void update() override;

  void fill(Color color) override;

  void setup() override {
    this->initialize();
  }

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

  void init_reset_();

  size_t get_buffer_length_();

  void start_command_();
  void end_command_();
  void start_data_();
  void end_data_();

  int get_width_internal() override;
  int get_height_internal() override;

  GPIOPin *reset_pin_;
  GPIOPin *dc_pin_;

  FrekvensPanel *panel;
};

}  // namespace pcd8544
}  // namespace esphome
