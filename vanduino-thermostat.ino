

#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MenuSystem.h>


#include <ExtendedDHT.h>
#include <VanDuino.h>
#include <ThermostatHardware.h>

#define DHTPIN 2
#define I2C_LCD_ADDR 0x27
#define UP_BUTTON_ADDR 13
#define DOWN_BUTTON_ADDR 12
#define LEFT_BUTTON_ADDR 11
#define RIGHT_BUTTON_ADDR 10

ThermostatHardware* thermostat_hardware;
MenuSystem menu_system;
Menu main_menu("VanDuino");

//*********************************************
// Menu and associated callbacks
//*********************************************

// Menu set temp callback
void on_menu_set_temp(MenuItem* p_menu_item) {
  Serial.println("set temp");
  thermostat_hardware->runTemperatureSet();
}

void on_menu_turn_off(MenuItem* p_menu_item) {
  Serial.println("Turning off");
  thermostat_hardware->runShutdown();
}

void on_menu_exit(MenuItem* p_menu_item) {
  thermostat_hardware->startScreenSaver();

}

// Menu setup
void thermostat_menu_setup() {
  MenuItem *mm_set_temp = new MenuItem("Set Temp");
  main_menu.add_item(mm_set_temp, &on_menu_set_temp);
  MenuItem *mm_turn_off = new MenuItem("Turn off");
  main_menu.add_item(mm_turn_off, &on_menu_turn_off);
  Menu *m_programs = new Menu("Programs");
  main_menu.add_menu(m_programs);
  for() {
  }
  MenuItem *mm_exit = new MenuItem("Exit");
  main_menu.add_item(mm_exit, &on_menu_exit);
  menu_system.set_root_menu(&main_menu);
}

void display_menu() {
  thermostat_hardware->displayMenu();
}

void init_thermostat_hardware(int lcd_address, const int up_button_pin, const int down_button_pin, const int left_button_pin, const int right_button_pin, const int dhtpin) {
  thermostat_hardware = new ThermostatHardware(lcd_address, up_button_pin, down_button_pin, left_button_pin, right_button_pin, dhtpin, &menu_system);

  thermostat_menu_setup();
}

//*********************************************
// Arduino Setup
//*********************************************
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet)
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");

  init_thermostat_hardware(I2C_LCD_ADDR, UP_BUTTON_ADDR, DOWN_BUTTON_ADDR, LEFT_BUTTON_ADDR, RIGHT_BUTTON_ADDR, DHTPIN);
  //menuSetup();
  thermostat_hardware->displaySplashScreen();
}

//*********************************************
// Arduino program loop
//*********************************************
void loop() {
  thermostat_hardware->loop();
}

/*




void on_item2_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Item2 Selected  ");
  delay(1500); // so we can look the result on the LCD
}

void on_item3_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Item3 Selected  ");
  delay(1500); // so we can look the result on the LCD
}
*/

