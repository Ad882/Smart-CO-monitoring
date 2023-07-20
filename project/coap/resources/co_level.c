#include "contiki.h"
#include "coap-engine.h"
#include <string.h>

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP
static char units_avl[6][15] = {
        "unit_A ",
        "unit_B ",
        "unit_C ",
    };
static int actual_units = 3;
static int max_units = 7;
static void res_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
/* A simple actuator example, depending on the color query parameter and post variable mode, corresponding led is activated or deactivated */
RESOURCE(factory,
         "title=\"Factory: ?unit=0..\" POST/PUT name=<name>&co_value=<co_value>\";rt=\"Control\"",
		 res_get_handler,
         res_post_handler,
         res_put_handler,
         NULL);

static void res_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){
  const char *name = NULL;
  if(coap_get_post_variable(request, "name", &name)&& actual_units <=max_units) {
    char new_unit[15];
    sprintf(new_unit, "%s, ", name);
    strcpy(units_avl[actual_units],new_unit);
    actual_units +=1;
    coap_set_status_code(response,CREATED_2_01);
  }else{
	  coap_set_status_code(response, BAD_REQUEST_4_00);
  }
}

static void res_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){
	size_t len = 0;
	const char *text = NULL;
	char unit[15];
    memset(unit, 0, 15);
	char co_level[32];
    memset(co_level, 0, 32);
	int success_1 = 0;
	int success_2 = 0;

	len = coap_get_post_variable(request, "name", &text);
	if(len > 0 && len < 15) {
	    memcpy(unit, text, len);
	    success_1 = 1;
	}

	len = coap_get_post_variable(request, "co_value", &text);
	if(len > 0 && len < 32 && success_1 == 1) {
		memcpy(co_level, text, len);
		char msg[500];
	    memset(msg, 0, 500);
		sprintf(msg, "CO level in %s set to %s", unit, co_level);
		int length=sizeof(msg);
		coap_set_header_content_format(response, TEXT_PLAIN);
		coap_set_header_etag(response, (uint8_t *)&length, 1);
		coap_set_payload(response, msg, length);
		success_2=1;
		coap_set_status_code(response, CHANGED_2_04);
	}
	if (success_2 == 0){
		coap_set_status_code(response, BAD_REQUEST_4_00);
	}
}

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){
  const char *unit = NULL;
  int length;
  int index;
  /* The query string can be retrieved by rest_get_query() or parsed for its key-value pairs. */
  if(coap_get_query_variable(request, "unit", &unit)) {
    index = atoi(unit);
    if(index > 0 && index < actual_units+1) {
      length = sizeof(units_avl[index-1]);
      memcpy(buffer, units_avl[index-1], length);
    } else {
    length = sizeof(units_avl);
    memcpy(buffer, units_avl, length);
    }
  }else{
	 length = sizeof(units_avl);
	 memcpy(buffer, units_avl, length);
  }
  coap_set_header_content_format(response, TEXT_PLAIN); /* text/plain is the default, hence this option could be omitted. */
  coap_set_header_etag(response, (uint8_t *)&length, 1);
  coap_set_payload(response, buffer, length);
}
