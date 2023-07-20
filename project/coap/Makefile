CONTIKI_PROJECT = coap-server
all: $(CONTIKI_PROJECT)
CONTIKI = ../..


PLATFORMS_EXCLUDE = nrf52dk z1
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
MODULES_REL += ./resources


include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_APP_LAYER_DIR)/coap


MODULES_REL += webserver
# Include optional target-specific module
include $(CONTIKI)/Makefile.identify-target
MODULES_REL += $(TARGET)


include $(CONTIKI)/Makefile.include

