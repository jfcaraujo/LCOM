#ifndef RTC_H
#define RTC_H 

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "i8042.h"

/** @defgroup Rtc Rtc
* @{
*/

typedef struct {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t hour;
  uint32_t minute;
  uint32_t second; 
} Date;

/**
 * @brief Check if the rtc is being updated
 * 
 * @return True if the rtc is being updated, false otherwise
 */
bool (update_in_progress)();

/**
 * @brief Tranforms the number in BCD format specified in its argument to a binary number
 * 
 * @param bcd_number Number in BCD format
 * @return Number tranformed to binary format
 */
int (bcd_to_binary)(uint32_t bcd_number);

/**
 * @brief Checks if the rtc is using binary format
 * 
 * @return True if the RTC is using binary format, False if it is using BCD format
 */
bool (binary_format)();

/**
 * @brief Reads the RTC register and return in its argument the date read
 * 
 * @param date Pointer to struct of type Date, that is updated to the date read in the RTC
 */
void (readDate)(Date* date);

/**
 * @brief Waits untill the RTC is not being updated to read its read
 * 
 * @return Struct of type Date with the information about the current date and time
 */
Date (getCurrentDate)();

/** @} end of Rtc */
#endif 
