#ifndef TCP_HEADER_HPP
#define TCP_HEADER_HPP

#include <string>
#include <cstdint>

#define HEADER_SIZE 12

class tcp_header
{
  private:
    void set_seq_num(uint32_t seq_num);
    void set_ack_num(uint32_t ack_num);
    void set_ID(uint16_t ID);
    void set_flag(uint16_t flag);

  public:
    uint32_t seq_num = 0;
    uint32_t ack_num = 0;
    uint16_t ID = 0;
    uint16_t flag = 0;
    tcp_header();
    tcp_header(uint32_t seq_num, uint32_t ack_num,
               uint16_t ID, uint16_t flag);
    static uint32_t give_seq(uint32_t network_seq_num);
    static uint32_t give_ack(uint32_t network_ack_num);
    static uint16_t give_id(uint16_t network_ID);
    static uint16_t give_flag(uint16_t network_flag);
};
#endif