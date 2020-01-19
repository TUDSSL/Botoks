#ifndef INCLUDE_MESSAGE_H
#define INCLUDE_MESSAGE_H

#define PAYLOAD_LEN 2

/**
 * Message structure.
 *
 * @member dst_id   ID of the destination node
 * @member msg_type message type
 * @member data     payload
 *
 * WARNING: attribute packed might not work with TI compiler!
 */
typedef struct __attribute__((packed)) {
  uint8_t dst_id;
  uint8_t msg_type;
  uint8_t data[PAYLOAD_LEN];
} message_t;

#endif /* INCLUDE_MESSAGE_H */
