#include <stddef.h>
<<<<<<< HEAD
#include <stdint.h>
#include <stdio.h>
=======
#include <string.h>
>>>>>>> 79fe7c1 (Skeleton of step9)

#include "util.h"
#include "ip.h"
#include "icmp.h"

#define ICMP_BUFSIZ IP_PAYLOAD_SIZE_MAX

#define icmp_type com.type
#define icmp_code com.code
#define icmp_sum  com.sum

struct icmp_common {
    uint8_t type;
    uint8_t code;
    uint16_t sum;
};

struct icmp_hdr {
    struct icmp_common com;
    uint32_t dep; /* message dependent field*/
};

struct icmp_echo {
    struct icmp_common com;
    uint16_t id;
    uint16_t seq;
};

struct icmp_dest_unreach {
    struct icmp_common com;
    uint32_t unused; /* zero */
};

/**

#define ICMP_TYPE_ECHO_REPLY       0
#define ICMP_TYPE_DEST_UNREACH     3
#define ICMP_TYPE_SOURCE_QUENCH    4
#define ICMP_TYPE_REDIRECT         5
#define ICMP_TYPE_ECHO             8
#define ICMP_TYPE_TIME_EXCEEDED   11
#define ICMP_TYPE_PARAM_PROBLEM   12
#define ICMP_TYPE_TIMESTAMP       13
#define ICMP_TYPE_TIMESTAMP_REPLY 14
#define ICMP_TYPE_INFO_REQUEST    15
#define ICMP_TYPE_INFO_REPLY      16*/
static char *icmp_type_ntoa(uint8_t type) {
  switch (type) {
  case ICMP_TYPE_ECHO_REPLY:
    return "EchoReply";
  case ICMP_TYPE_DEST_UNREACH:
    return "DestinationUnreachable";
  case ICMP_TYPE_SOURCE_QUENCH:
    return "SourceQuench";
  case ICMP_TYPE_REDIRECT:
    return "Redirect";
  case ICMP_TYPE_ECHO:
    return "Echo";
  case ICMP_TYPE_TIME_EXCEEDED:
    return "TimeExceeded";
  case ICMP_TYPE_PARAM_PROBLEM:
    return "ParameterProblem";
  case ICMP_TYPE_TIMESTAMP:
    return "Timestamp";
  case ICMP_TYPE_TIMESTAMP_REPLY:
    return "TimestampReply";
  case ICMP_TYPE_INFO_REQUEST:
    return "InfoRequest";
  case ICMP_TYPE_INFO_REPLY:
    return "InfoReply";
  default:
    return "Unknown";
  }
}

static void icmp_print(const uint8_t *data, size_t len) {
  struct icmp_hdr *hdr;
  struct icmp_echo *echo;
  struct icmp_dest_unreach *unreach;

  flockfile(stderr);
  hdr = (struct icmp_hdr *)data;
  fprintf(stderr, "        type: %u (%s)\n", hdr->icmp_type,
          icmp_type_ntoa(hdr->icmp_type));
  fprintf(stderr, "        code: %u\n", hdr->icmp_code);
  fprintf(stderr, "        sum: 0x%04x\n", hdr->icmp_sum);
  switch (hdr->icmp_type) {
  case ICMP_TYPE_ECHO_REPLY:
  case ICMP_TYPE_ECHO:
    echo = (struct icmp_echo *)data;
    fprintf(stderr, "        id: %u\n", echo->id);
    fprintf(stderr, "        seq: %u\n", ntoh16(echo->seq));
    break;
  case ICMP_TYPE_DEST_UNREACH:
    unreach = (struct icmp_dest_unreach *)data;
    fprintf(stderr, "        unused: 0x%08x\n", unreach->unused);
    break;
  default:
    fprintf(stderr, "        dep: 0x%08x\n", ntoh32(hdr->dep));
    break;
  }
#ifdef HEXDUMP
  hexdump(stderr, data, len);
#endif
  funlockfile(stderr);
}

static void icmp_input(const struct ip_hdr *iphdr, const uint8_t *data,
                       size_t len, struct ip_iface *iface) {
  struct icmp_hdr *hdr;
  char addr1[IP_ADDR_STR_LEN];
  char addr2[IP_ADDR_STR_LEN];
  if (len < sizeof(*hdr)) {
    errorf("too short");
    return;
  }
  if (cksum16((uint16_t *)data, len, 0) != 0) {
    errorf("checksum error");
    return;
  }
  debugf("%s => %s, len=%zu", ip_addr_ntop(iphdr->src, addr1, sizeof(addr1)),
         ip_addr_ntop(iphdr->dst, addr2, sizeof(addr2)), len);
  icmp_print(data, len);
}

int
icmp_output(uint8_t type, uint8_t code, uint32_t val, const uint8_t *data, size_t len, ip_addr_t src, ip_addr_t dst)
{
}

int
icmp_init(void)
{
    if (ip_protocol_register(IP_PROTOCOL_ICMP, icmp_input) == -1) {
        errorf("ip_protocol_register() failure");
        return -1;
    }
    return 0;
}
