#include "rtl_433.h"


// buffer to hold localized timestamp YYYY-MM-DD HH:MM:SS
#define LOCAL_TIME_BUFLEN	32

void local_time_str();

static int dsc_contact_callback(uint8_t bb[BITBUF_ROWS][BITBUF_COLS],int16_t bits_per_row[BITBUF_ROWS]) {
    /* Two bits map to 2 states, 0 1 -> 0 and 1 1 -> 1 */
    int i;
    uint8_t nb[3] = {0};
    time_t time_now;
    char time_str[LOCAL_TIME_BUFLEN];

    time(&time_now);
    local_time_str(time_now, time_str);
    fprintf(stdout,"%s Possible DSC Contact: ",time_str);
    debug_callback(bb, bits_per_row);

    return 1;

}

/*
 * regular contacts, 2.5 ms wake up pulse
 * Short limit needs to be an accurate representation of 
 * the actual short pulse length without excessive padding.
 */
r_device dsc_contact = {
    /* .id             = */ 6,
    /* .name           = */ "DSC Security Contact",
    /* .modulation     = */ OOK_PWM_S0,
  // very level limit dependant, 90 is good for 7000, 105 is better for 5000 or lower 
    /* .short_limit    = */ 90,
    /* .long_limit     = */ 775,
    /* .reset_limit    = */ 7000,
    /* .json_callback  = */ &dsc_contact_callback,
    /* .disabled       = */ 0
};

/*
 * Smoke and carbon monoxide detectors, 5.6 ms wake up pulse
 */
r_device dsc_contact2 = {
    /* .id             = */ 6,
    /* .name           = */ "DSC Smoke/Fire Contact",
    /* .modulation     = */ OOK_PWM_S0,
    /* .short_limit    = */ 90,
    /* .long_limit     = */ 1500, // 1736 too long?
    /* .reset_limit    = */ 7000,
    /* .json_callback  = */ &dsc_contact_callback,
    /* .disabled       = */ 0
};


/*
 *  waveman false positives,
 *  - Original Values:  1000/4,8000/4,30000/4 
 *  - 250Khz Samples:  (250, 2000, 7500) 
 *  - real time:       1 ms, 8 ms, 1875 ms?
 *   
 * 
 *  Inital pulse is 2.5 ms - 625 samples for most contacts.
 *  Smoke detector wake-up pulse is 5.6 ms, or 1400
 */
