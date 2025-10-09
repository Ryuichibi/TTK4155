#include "can.h"

void can_init()
{

}

void can_send(can_message_t message)
{
    uint8_t id[2];

    id[0] = (message.id >> 3);
    id[1] = (message.id % 8) << 5;

    mcp2515_sequential_write(MCP_TXB0SIDH, id, 2);

    mcp2515_write(MCP_TXB0DLC, message.data_count);

    mcp2515_sequential_write(MCP_TXB0D0, message.data, message.data_count);

    mcp2515_request_send(0x01);
}

void can_receive(can_message_t *message)
{
    uint8_t data;

    mcp2515_read(MCP_RXB0SIDH, &data);
    message->id = data << 3;

    mcp2515_read(MCP_RXB0SIDL, &data);
    message->id |= (data >> 5);

    mcp2515_read(MCP_RXB0DLC, &(message->data_count));

    for (uint8_t i = 0; i < message->data_count; i++)
    {
        mcp2515_read(MCP_RXB0D0 + i, message->data + i);
    }
}