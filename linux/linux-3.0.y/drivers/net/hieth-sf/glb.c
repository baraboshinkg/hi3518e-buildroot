#include "hieth.h"
#include "mdio.h"
#include "mac.h"
#include "ctrl.h"
#include "glb.h"

int hieth_port_init(struct hieth_netdev_local *ld, int port)
{
	/* HW DEFAULT RX-PKT-LEN-RANGE [42,1518]	*/
	/* HW MAC FILTER TABLE DISABLE			*/

	hieth_assert(port == ld->port);

	hieth_set_endian_mode(ld, HIETH_LITTLE_ENDIAN);

	hieth_set_linkstat(ld, 0);

	hieth_set_negmode(ld, HIETH_NEGMODE_CPUSET);

	/* MII or RMII mode */
	hieth_set_mii_mode(ld, UD_BIT_NAME(CONFIG_HIETH_MII_RMII_MODE));

	/*clear all interrupt status*/
	hieth_clear_irqstatus(ld, UD_BIT_NAME(BITS_IRQS_MASK));
	/*disable interrupts*/
	hieth_writel_bits(ld, 0, GLB_RW_IRQ_ENA, UD_BIT_NAME(BITS_IRQS_ENA));
	hieth_irq_disable(ld, UD_BIT_NAME(BITS_IRQS_MASK));

	/*disable vlan func*/
	hieth_writel_bits(ld, 0, GLB_FWCTRL, BITS_VLAN_ENABLE);

	/*enable UpEther<->CPU*/
	if (ld->port == UP_PORT) {
		hieth_writel_bits(ld, 1, GLB_FWCTRL, BITS_FW2CPU_ENA_UP);
		hieth_writel_bits(ld, 1, GLB_FWCTRL, BITS_FWALL2CPU_UP);
		hieth_writel_bits(ld, 1, GLB_MACTCTRL, BITS_BROAD2CPU_UP);
	} else if (ld->port == DOWN_PORT) {
		/*enable DownEther<->CPU*/
		hieth_writel_bits(ld, 1, GLB_FWCTRL, BITS_FW2CPU_ENA_DOWN);
		hieth_writel_bits(ld, 1, GLB_FWCTRL, BITS_FWALL2CPU_DOWN);
		hieth_writel_bits(ld, 1, GLB_MACTCTRL, BITS_BROAD2CPU_DOWN);
	} else {
		BUG();
	}

	hieth_set_mac_leadcode_cnt_limit(ld, 0);

	return 0;
}

/* vim: set ts=8 sw=8 tw=78: */
