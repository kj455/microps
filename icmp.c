#include <stdint.h>
#include <stddef.h>

#include "util.h"
#include "ip.h"
#include "icmp.h"

static void
icmp_input(const struct ip_hdr *iphdr, const uint8_t *data, size_t len, struct ip_iface *iface)
{
}

int
icmp_init(void)
{
}
