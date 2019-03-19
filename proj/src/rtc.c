#include "rtc.h"

bool (update_in_progress)()
{
  uint32_t data = 0;

  sys_outb(RTC_ADDR_REG, REGISTER_A);
  sys_inb(RTC_DATA_REG, &data);

  if(data & UIP_FLAG)
    return true;
  else return false;
}

int (bcd_to_binary)(uint32_t  bcd_number)
{
  return (bcd_number - 6 * (bcd_number >> 4));
}

bool (binary_format)()
{
  uint32_t data = 0;

  sys_outb(RTC_ADDR_REG, REGISTER_B);
  sys_inb(RTC_DATA_REG, &data);

  if(data & DM_FLAG) 
    return true;
  else return false;
}

void (readDate)(Date* date)
{
  sys_outb(RTC_ADDR_REG, DAY);
  sys_inb(RTC_DATA_REG, &(date->day));

  sys_outb(RTC_ADDR_REG, MONTH);
  sys_inb(RTC_DATA_REG, &(date->month));

  sys_outb(RTC_ADDR_REG, YEAR);
  sys_inb(RTC_DATA_REG, &(date->year));

  sys_outb(RTC_ADDR_REG, HOURS);
  sys_inb(RTC_DATA_REG, &(date->hour));

  sys_outb(RTC_ADDR_REG, MINUTES);
  sys_inb(RTC_DATA_REG, &(date->minute));

  sys_outb(RTC_ADDR_REG, SECONDS);
  sys_inb(RTC_DATA_REG, &(date->second));

  if(!binary_format()) {

    date->year = bcd_to_binary(date->year);
    date->month = bcd_to_binary(date->month);
    date->day = bcd_to_binary(date->day);
    date->hour = bcd_to_binary(date->hour);
    date->minute = bcd_to_binary(date->minute);
    date->second = bcd_to_binary(date->second);
  }
}

Date (getCurrentDate)()
{
  Date currentDate;

  do {

    if(!update_in_progress()) 
      readDate(&currentDate);
    
  } while (update_in_progress());

  return currentDate;
}

