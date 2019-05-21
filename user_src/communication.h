#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include "../auxiliary_src/constants.h"
#include "../auxiliary_src/types.h"

int open_connection_to_srv();

int open_local_fifo();

int send_request(tlv_request_t request);

int read_response(tlv_reply_t *buffer);

int create_local_fifo();

int close_connection_to_srv();
