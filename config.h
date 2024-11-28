/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/* Custom commands */
static const char ethernet_status[] = 
    "ethernet_iface=$(ip link show | grep -E '^[0-9]+: (en|eth)' | awk '{print $2}' | sed 's/://'); \
    if [ -n \"$ethernet_iface\" ]; then \
        if [ \"$(cat /sys/class/net/$ethernet_iface/operstate)\" = \"up\" ]; then \
            echo \"$ethernet_iface\"; \
        fi; \
    fi";

static const char wifi_status[] = "wifi_iface=$(iw dev | grep -oP 'Interface \\K\\w+' | head -n 1); \
if [ -n \"$wifi_iface\" ]; then \
    if [ \"$(cat /sys/class/net/$wifi_iface/operstate)\" = \"up\" ]; then \
        wifi_ssid=$(iw dev $wifi_iface link | grep -oP 'SSID: \\K.*'); \
        if [ -n \"$wifi_ssid\" ]; then \
            echo \"$wifi_ssid\"; \
        fi; \
    fi; \
fi";

static const char swap_status[] = 
    "swap_usage=$(free -h | grep Swap | awk '{print $3 \"/\" $2}'); echo $swap_usage";

static const char ram_status[] = 
    "ram_used=$(free -h | grep Mem | awk '{print $3}'); \
    ram_total=$(free -h | grep Mem | awk '{print $2}'); \
    echo \"$ram_used/$ram_total\"";

/* function format argument */
static const struct arg args[] = {
    { cpu_perc,           " %s%%",       NULL },  // CPU usage with percentage symbol
    { run_command,        " 󰈀 %s",         ethernet_status },  // Ethernet status if connected
    { run_command,        "  %s",         wifi_status },      // WiFi SSID if connected
    { run_command,        " 󰯍 %s",            swap_status },     // Swap used/total
    { run_command,        "  %s",            ram_status },      // RAM used/total
    { datetime,           "  %s",         "%a %d %B  %I:%M %p" }, // Date and time
};

