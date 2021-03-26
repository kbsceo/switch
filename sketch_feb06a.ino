#include "Keyboard.h"
#include <stdarg.h>


#define btn_count 3
#define SW1 13
#define SW2 12
#define SW3 11

#define click_SW1 0
#define click_SW2 1
#define click_SW3 2

#define LED_1 7
#define LED_2 6
#define LED_3 5

int press_btn[btn_count];
unsigned int press_time[btn_count];
int click_event[btn_count];
int next_led_state[btn_count];
int led_pins[3] = {LED_1, LED_2, LED_3};
int btn_pins[3] = {SW1, SW2, SW3};

int push_sw1_count,push_sw2_count, push_sw3_count;

unsigned long compare_time_ms; //밀리초로 비교변수 선언

void check_state_sw(void)
{
  unsigned long current_time_ms = millis();
  
  if( current_time_ms - compare_time_ms >= 5)
  {
    for(int i = 0; i<sizeof(press_btn)/sizeof(press_btn[0]); i++)
    {
      press_btn[i] = digitalRead(btn_pins[i]);
      if(press_btn[i] == 0)
      {
        press_time[i] += 5;
      }
      else if(press_btn[i] == 1) 
      {
        if(press_time[i] > 50) 
        {
          click_event[i] = 1;
        }
        press_time[i] = 0; 
      }
    }
    compare_time_ms = current_time_ms; 
  }
}

void change_led();


void setup() {

pinMode(13,INPUT); 
pinMode(12,INPUT); 
pinMode(11, INPUT);

pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);

Serial.begin(9600);
Keyboard.begin();
}

void loop() {
  
 



  
  check_state_sw();
 // Serial.println((int)press_time[0]);
  if(click_event[click_SW1])
  {
    next_led_state[0] = next_led_state[0] == 0 ? 1 : 0;
    click_event[click_SW1] = 0; //이벤트 플래그를 초기화
    push_sw1_count += 1;
    Serial.write("SW1 클릭 : ");
    Serial.print(push_sw1_count);
    Serial.write("\n");
    Keyboard.press('l');
    Keyboard.release('l');
    
    
    
  }
  if(click_event[click_SW2])
  {
    click_event[click_SW2] = 0; //이벤트 플래그를 초기화
    next_led_state[1] = !next_led_state[1];
      push_sw2_count += 1;
      Serial.write("SW2 클릭 : ");
      Serial.print(push_sw2_count);
      Serial.write("\n");
      Keyboard.press('s');
      Keyboard.release('s');
  }

  if(click_event[click_SW3])
  {
    click_event[click_SW3] = 0; //이벤트 플래그를 초기화
    next_led_state[2] = !next_led_state[2];
      push_sw3_count += 1;
      Serial.write("SW3 클릭 : ");
      Serial.print(push_sw3_count);
      Serial.write("\n");
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('t');
      Keyboard.releaseAll();
  }

  change_led();
}


void change_led() {
    for (int i = 0; i < 3; i++)
    {
        int current_led_state = digitalRead(led_pins[i]);
        if (current_led_state != next_led_state[i]) {
            digitalWrite(led_pins[i], next_led_state[i]);
        }
    }
}
