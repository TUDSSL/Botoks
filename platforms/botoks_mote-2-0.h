#ifndef INCLUDE_BOTOKS_2_H
#define INCLUDE_BOTOKS_2_H

// For logic analyzer
#define PRF_PORT J
#define DELAY_PIN 1
#define RADIO_PIN 2
#define RX_PKT_PIN 3

#define DBG_PORT 1
#define DEBUG_B 0
#define DBG_PWR_PORT 1
#define DEBUG_POWER_PIN 1

// Timer pin definitions
// GPIO pins
#define ADC_PORT 1
#define ADC_PIN_0 2
#define ADC_PIN_1 5
#define ADC_PIN_2 3
#define ADC_PIN_3 4

#define ADC_CHANNEL_PREFIX ADC12INCH_
#define ADC_CHANNEL_0 CONCAT(ADC_CHANNEL_PREFIX, ADC_PIN_0)
#define ADC_CHANNEL_1 CONCAT(ADC_CHANNEL_PREFIX, ADC_PIN_1)
#define ADC_CHANNEL_2 CONCAT(ADC_CHANNEL_PREFIX, ADC_PIN_2)
#define ADC_CHANNEL_3 CONCAT(ADC_CHANNEL_PREFIX, ADC_PIN_3)

#define CHARGE_PORT 3
#define CHARGE_PIN_0 1
#define CHARGE_PIN_1 3
#define CHARGE_PIN_2 5
#define CHARGE_PIN_3 4

#define TIME_EN_PORT 3
#define TIME_EN_PIN_0 2
#define TIME_EN_PIN_1 0
#define TIME_EN_PIN_2 7
#define TIME_EN_PIN_3 6

#define CAS_PORT 4
#define CAS_CLK_PIN 1
#define CAS_EN_PIN 0

#define CAS_LDO_PORT 2
#define CAS_LDO_EN 4

#define CAS_EN_ADC_PORT 4
#define CAS_EN_ADC_EN 5

// Radio pin definitions
#define zl70550_POWER_PORT 4
#define zl70550_POWER_PIN 6
// CSn pin
#define zl70550_CSn_PORT 4
#define zl70550_CSn_PIN 4

#define zl70550_RESETn_PORT 2
#define zl70550_RESETn_PIN 3

#define zl70550_IRQ_PORT 2
#define zl70550_IRQ_PIN 7

#define SPI_MOSI_PORT 1
#define SPI_MOSI_PIN 6
#define SPI_MISO_PORT 1
#define SPI_MISO_PIN 7
#define SPI_CLK_PORT 2
#define SPI_CLK_PIN 2
#define SPI_CS_PORT 2
#define SPI_CS_PIN 4

#endif /* INCLUDE_BOTOKS_2_H */
