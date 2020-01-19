#ifndef INCLUDE_zl70550_regs_H
#define INCLUDE_zl70550_regs_H

////////////////////////////
// Registers for ZL70550  //
////////////////////////////

// SPI Local Registers
//(Address range: hexadecimal 0x0–0x7. Addressing mode: short. Always-on: no.)
#define SPIR_CHIP_ID 0x0     // R 8'h47
#define SPIR_REVISION 0x1    // R 8'h11
#define SPIR_2V_RESET 0x2    // R/W 8'hCA
#define SPIR_SYS_RESET 0x3   // R/W 8'hBC
#define SPIR_PWRDWN_REQ 0x4  // R/W 8'hDE
#define SPIR_LOCAL_STAT 0x5  // R 7'h00
#define SPIR_CTRL 0x6        // R/W 5'h00

// System Bus Control and Status Registers
//(Address range: hexadecimal 0x100–0x14B. Addressing mode: long. Always-on:
// no.)
#define DP_CTRL0 0x100        // R/W 7'h00
#define RPHR_STAT 0x105       // R 8'h08
#define RXC_PLD_LEN0 0x106    // R 8'h00
#define RXC_PLD_LEN1 0x107    // R 1'h00
#define MSC_SPI_STAT 0x108    // R 4'h00
#define RXC_FRM_LEN0 0x109    // R 8'h00
#define RXC_FRM_LEN1 0x10A    // R 1'h00
#define RXC_FRM_CTRL0 0x10B   // R 8'h00
#define RXC_FRM_CTRL1 0x10C   // R 8'h00
#define RXC_SOURCE_SID 0x10D  // R 8'h00
#define RXC_NETWORK_ID 0x10E  // R 8'h00
#define RXC_DEST_SID 0x10F    // R 8'h00

#define RXC_DEST_LID0 0x110  // R 8'h00
#define RXC_DEST_LID1 0x111  // R 8'h00
#define RXC_DEST_LID2 0x112  // R 8'h00
#define RXC_DEST_LID3 0x113  // R 8'h00
#define RXC_DEST_LID4 0x114  // R 8'h00
#define RXC_DEST_LID5 0x115  // R 8'h00
#define RXC_DEST_LID6 0x116  // R 8'h00
#define RXC_DEST_LID7 0x117  // R 8'h00

#define RXC_SOURCE_LID0 0x118  // R 8'h00
#define RXC_SOURCE_LID1 0x119  // R 8'h00
#define RXC_SOURCE_LID2 0x11A  // R 8'h00
#define RXC_SOURCE_LID3 0x11B  // R 8'h00
#define RXC_SOURCE_LID4 0x11C  // R 8'h00
#define RXC_SOURCE_LID5 0x11D  // R 8'h00
#define RXC_SOURCE_LID6 0x11E  // R 8'h00
#define RXC_SOURCE_LID7 0x11F  // R 8'h00

#define RXC_FRM_STAT0 0x120   // R 7'h00
#define RXC_FRM_STAT1 0x121   // R 7'h00
#define MSC_FRM_STAT0 0x122   // R 8'h00
#define MSC_FRM_STAT1 0x123   // R 8'h00
#define MAN_GLOBAL_EN 0x126   // R/W 3'h00
#define MAN_TEST 0x12E        // R/W 4'h04
#define CLK_TEST 0x131        // R/W 8'h00
#define ADC_CONV_START 0x13A  // R/W 2'h00
#define ADC_MAX 0x13B         // R 7'h00
#define ADC_AVG 0x13C         // R 7'h00

#define ADC_AVG_TRMD_PKT_RSSI 0x13F  // R 7'h00
#define IRQ0 0x143                   // CoR 3'h00
#define IRQ1 0x144                   // CoR 8'h00
#define IRQ2 0x145                   // CoR 8'h00
#define TRIM_CMD 0x147               // R/W 4'h00
#define LNA_PEAK_ADC 0x14A           // R 5'h00
#define ANT_PEAK_ADC 0x14B           // R 5'h00

// Always-On System Bus Control and Status Registers
//(Address range: hexadecimal 0x200–0x29F. Addressing mode: long. Always-on:
// yes.)
#define VCO_FRQ_RX_TRIM_L 0x201       // R/W 8'h88
#define VCO_FRQ_RX_TRIM_H 0x202       // R/W 3'h00
#define VCO_FRQ_TXPAOFF_TRIM_L 0x203  // R/W 8'h97
#define VCO_FRQ_TXPAOFF_TRIM_H 0x204  // R/W 3'h00
#define VCO_FRQ_TXPAON_TRIM_L 0x205   // R/W 8'h98
#define VCO_FRQ_TXPAON_TRIM_H 0x206   // R/W 3'h00
#define VCO_FRQ_TRIM_L 0x207          // R 8'hFF
#define VCO_FRQ_TRIM_H 0x208          // R 3'h04
#define VCO_FRQ_BAND_TRIM 0x209       // R/W 4'h07
#define MOD_DAC_TRIM 0x20A            // R/W 5'h00
#define FSK_DEV_TRIM_TARGET \
  0x20C  // R/W 8'h66 // US: 8’h6F,EU: 8’h76,China: 8’h83
#define RCOSC_FREQ_TRIM 0x20D  // R/W 6'h00

#define LNA_FRQ_TRIM 0x20E   // R/W 5'h16
#define LNA_BIAS_TRIM 0x20F  // R/W 6'h05
#define IREF_TRIM 0x210      // R/W 5'h0F
#define XO_TRIM 0x211        // R/W 6'h26
#define GAUS_TRIM 0x214      // R/W 8'h62
#define VCO_AMP_TRIM 0x215   // R/W 6'h3F
#define ANT_TRIM 0x216       // R/W 5'h16
#define RX_PKDET_TRIM 0x217  // R/W 5'h09
#define DC_CNTR_TRIM 0x218   // R/W 8'h62

#define IRQ_EN0 0x219        // R/W 4'h08
#define IRQ_EN1 0x21A        // R/W 8'h00
#define IRQ_EN2 0x21B        // R/W 8'h00
#define PAD_EN0 0x21C        // R/W 8'h00
#define TX_CTRL0 0x21E       // R/W 8'h1B
#define TX_CTRL1 0x21F       // R/W 7'h01
#define TX_CTRL2 0x220       // R/W 8'hA2
#define TX_BUF_LEN0 0x221    // R/W 8'h00
#define TX_BUF_LEN1 0x222    // R/W 1'h00
#define TX_FRM_CTRL 0x223    // R/W 8'h01
#define TX_FRM_SEQ_NO 0x224  // R/W 4'h00
#define RX_CTRL0 0x225       // R/W 8'h0B
#define RX_CTRL1 0x226       // R/W 8'h12
#define RX_CTRL2 0x227       // R/W 8'h84
#define ADDR_MASK0 0x229     // R/W 8'hFF
#define ADDR_MASK1 0x22A     // R/W 8'hFF
#define RX_FRM_LEN0 0x22B    // R/W 8'h00
#define RX_FRM_LEN1 0x22C    // R/W 1'h01

#define RX_HDR_THRESH 0x22D  // R/W 8'h00
#define NETWORK_ID 0x22E     // R/W 8'h00
#define DEVICE_SID 0x22F     // R/W 8'h00
#define OTHER_SID 0x230      // R/W 8'h00
#define DEVICE_LID0 0x231    // R/W 8'h00
#define DEVICE_LID1 0x232    // R/W 8'h00
#define DEVICE_LID2 0x233    // R/W 8'h00
#define DEVICE_LID3 0x234    // R/W 8'h00
#define DEVICE_LID4 0x235    // R/W 8'h00
#define DEVICE_LID5 0x236    // R/W 8'h00
#define DEVICE_LID6 0x237    // R/W 8'h00
#define DEVICE_LID7 0x238    // R/W 8'h00
#define OTHER_LID0 0x239     // R/W 8'h00
#define OTHER_LID1 0x23A     // R/W 8'h00
#define OTHER_LID2 0x23B     // R/W 8'h00
#define OTHER_LID3 0x23C     // R/W 8'h00
#define OTHER_LID4 0x23D     // R/W 8'h00
#define OTHER_LID5 0x23E     // R/W 8'h00
#define OTHER_LID6 0x23F     // R/W 8'h00
#define OTHER_LID7 0x240     // R/W 8'h00
#define MAC_CTRL 0x241       // R/W 5'h00
#define CRC_POLY0 0x242      // R/W 8'h01
#define CRC_POLY1 0x243      // R/W 8'h8F
#define CRC_POLY2 0x244      // R/W 8'h00
#define CRC_POLY3 0x245      // R/W 8'h00
#define SPI_CTRL 0x24A       // R/W 4'h07
#define CSMA_CTRL 0x24B      // R/W 7'h45
#define CSMA_TIME0 0x24C     // R/W 8'h0A
#define CSMA_TIME1 0x24D     // R/W 4'h00

#define CSMA_MAX_RAND_BACKOFF0 0x24E   // R/W 8'h32
#define CSMA_MAX_RAND_BACKOFF1 0x24F   // R/W 4'h00
#define CSMA_RETRY_MIN_BACKOFF0 0x250  // R/W 8'h19
#define CSMA_RETRY_MIN_BACKOFF1 0x251  // R/W 4'h00

#define SYNC_THRESH 0x252     // R/W 6'h24 6’h15
#define PREAM_LEN 0x253       // R/W 8'h1A 8’h0A
#define TX_TRIG_CNT 0x254     // R/W 8'h14
#define ADJ_FAST 0x255        // R/W 8'h58 8’h25
#define ADJ_SLOW 0x256        // R/W 8'h58 8’h25
#define PREAM_LOCK_CNT 0x260  // R/W 8'h12 8’h07
#define GPIO_0_SEL 0x261      // R/W 8'h00
#define GPIO_1_SEL 0x262      // R/W 8'h00
#define GPIO_2_SEL 0x263      // R/W 8'h00
#define GPIO_3_SEL 0x264      // R/W 8'h00
#define SYNC_PTRN0 0x265      // R/W 8'h13
#define SYNC_PTRN1 0x266      // R/W 8'hDA
#define SYNC_PTRN2 0x267      // R/W 8'h32
#define SYNC_PTRN3 0x268      // R/W 8'hEC
#define SYNC_PTRN4 0x269      // R/W 8'h79
#define PLL_START_DLY 0x26A   // R/W 8'h14
#define TX_PLL_DLY_CNT 0x26B  // R/W 8'h1E 8’h0F
#define RX_PLL_DLY_CNT 0x26C  // R/W 8'h26 8’h16

#define CW_DLY_CNT 0x26D        // R/W 8'h26 8’h05
#define SLO_DLY_CNT 0x26E       // R/W 8'h44 8’hFF
#define PA_OFF_DLY_CNT 0x26F    // R/W 4'h03 4’h00
#define ACK_TIME_LIMIT 0x270    // R/W 8'h18 8’h0C
#define PKT_TIME_LIMIT 0x271    // R/W 8'h24 8’h0C
#define RX_FRM_SEQ_NO 0x272     // R/W 4'h00
#define TX_PKT_CNT 0x273        // WCOR 8'h00
#define TX_PKT_RETRY_CNT 0x274  // WCOR 8'h00
#define TX_PKT_DROP_CNT 0x275   // WCOR 8'h00
#define RX_PKT_CNT 0x276        // WCOR 8'h00
#define RX_ALL_PKT_CNT 0x277    // WCOR 8'h00
#define RX_PKT_DROP_CNT 0x278   // WCOR 8'h00
#define RX_SYNC_ERR_CNT 0x279   // WCOR 8'h00
#define RX_ALL_ERR_CNT 0x27A    // WCOR 8'h00
#define CSMA_RETRY_CNT 0x27B    // WCOR 8'h00
#define CSMA_FAIL_CNT 0x27C     // WCOR 8'h00
#define SYS_CLK_DIV 0x27D       // R/W 5'h16 5’h14
#define PLL_CLK_DIV_CNT 0x27E   // R/W 7'h51 7’h50

#define PHY_RX_MODE_SEL 0x282       // R/W 8'h45 0 for bit [6]
#define SYNC_DLY_CNT 0x283          // R/W 8'h22 8’h0F
#define PREAM_DET_CTRL 0x284        // R/W 8'h6B 8’h33
#define DPORT_CTRL 0x287            // R/W 6'h1D 6’h1B
#define PHY_TX_RAW_MODE_CTRL 0x288  // R/W 2'h00
#define RATE_CTRL 0x289             // R/W 8'h00
#define SYNTH_M_DIV 0x28A           // R/W 8'hB5
#define SYNTH_A_DIV 0x28B           // R/W 6'h07
#define ADC_MUX_IN_SEL 0x28E        // R/W 3'h00
#define ADC_POW_N_CONV 0x28F        // R/W 7'h30
#define ADC_RSSI_THRESH 0x293       // R/W 7'h00 7'h01
#define ADC_CSMA_THRESH 0x294       // R/W 7'h20
#define LNA_GAIN 0x295              // R/W 4'h07 4’h0F
#define ANA_CTRL0 0x297             // R/W 8'h80 8’h84
#define LOCK_PUMP_SEL 0x299         // R/W 6'h08
#define PA_PWR_LEVEL 0x29A          // R/W 8'h88
#define VCO_BUF_BIAS 0x29B          // R/W 4'h03
#define VCO_CTRL 0x29C              // R/W 7'h08
#define VCO_CMP_VREF_CTRL 0x29E     // R/W 2'h00 2'h01

#define rx_pkt_rdy_irq_mask 0b00000100
#define cmd_fail_irq_mask 0b00000010
#define cmd_done_irq_mask 0b00000001

#endif /* INCLUDE_zl70550_regs_H */
