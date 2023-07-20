#include "contiki.h"
#include "coap-engine.h"
#include "dev/leds.h"

#include <string.h>

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_DBG

static void res_post_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(led,
	"title=\"LED Color: ?color=r|g|y, POST mode=low|moderate|high\";rt=\"Control\"",
	 NULL,
         res_post_put_handler,
         res_post_put_handler,
         NULL);


static void res_post_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){
	if(request != NULL) {
		LOG_DBG("POST Request Sent\n");
	}
	
  size_t len = 0;
  const char *mode = NULL;
  int success = 1;

  if((len = coap_get_post_variable(request, "mode", &mode))) {
    LOG_DBG("mode %s\n", mode);

    if(strncmp(mode, "low", len) == 0) {
      leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
    } else if(strncmp(mode, "moderate", len) == 0) {
      leds_off(LEDS_ALL);
      leds_on(LEDS_RED);  
      printf("Activation of air conditioning.\n"); 
    } else if(strncmp(mode, "high", len) == 0) {
      leds_set(LEDS_NUM_TO_MASK(LEDS_RED));   
      printf("Setting air conditioning at maximum speed.\n");
    }else {
      success = 0;
    }
  } else {
    success = 0;
  } if(!success) {
    coap_set_status_code(response, BAD_REQUEST_4_00);
  }
}