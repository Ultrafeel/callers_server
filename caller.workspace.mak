#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WRKDIR = `pwd`
MAKE = make

all: caller_client_caller_client caller_server_caller_server settings_read_settings_read

caller_client_caller_client: 
	$(MAKE) -C caller_client all -f caller_client.cbp.mak

caller_server_caller_server: 
	$(MAKE) -C caller_server all -f caller_server.cbp.mak

settings_read_settings_read: 
	$(MAKE) -C settings_read all -f settings_read.cbp.mak

clean: clean_caller_client_caller_client clean_caller_server_caller_server clean_settings_read_settings_read

clean_caller_client_caller_client: 
	$(MAKE) -C caller_client clean -f caller_client.cbp.mak

clean_caller_server_caller_server: 
	$(MAKE) -C caller_server clean -f caller_server.cbp.mak

clean_settings_read_settings_read: 
	$(MAKE) -C settings_read clean -f settings_read.cbp.mak

.PHONY: clean_caller_client_caller_client clean_caller_server_caller_server clean_settings_read_settings_read

