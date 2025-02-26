# Športna Ura

## Opis Projekta
Ta projekt predstavlja prototip **športne ure**, ki meri **srčni utrip** in **nasičenost kisika v krvi (SpO2)** ter prikazuje podatke na **TFT LCD zaslonu**. 

### **Uporabljene komponente:**
- **Mikrokontroler:** STM32H750B-DK
- **Pulzni oksimeter:** Oximeter 5 Click (MAX30102)
- **LCD zaslon:** 1.28'' TFT GC9A01
- **IDE:** STM32CubeIDE

### **Funkcionalnosti:**
- Branje podatkov iz **Oximeter 5 Click** senzorja preko **I2C protokola**
- Izračunavanje **SpO2 in srčnega utripa** z algoritmi digitalne obdelave signalov (DPS)
- Prikaz podatkov na **TFT LCD zaslonu**
- Optimizirano risanje na zaslon s pomočjo **DMA** za hitrejši prikaz

---
## Namestitev in Uporaba

### **1. Priprava okolj in povezava**
1. **Povežite komponente**
   - Oximeter 5 Click na mikroBUS
   - LCD na SPI2 vhod STM32H750B-DK
   - Napajanje in GND povezave
2. **Odprite projekt v STM32CubeIDE**
3. **Zgradite in naložite firmware** na mikrokontroler
4. **Zaženite napravo** in spremljajte podatke na zaslonu

### **2. Inicializacija**
- **Oximeter 5 Click** inicializiramo z `oximeter5_init()`
- Konfiguracijo senzorja nastavimo z `oximeter5_default_cfg()`
- **LCD zaslon** inicializiramo z `GC9A01_Initial()`

### **3. Merjenje in prikaz podatkov**
- **Branje podatkov** iz oksimetra: `oximeter5_read_sensor_data()`
- **Izračunavanje srčnega utripa:** `oximeter5_get_heart_rate()`
- **Izračunavanje SpO2:** `oximeter5_get_oxygen_saturation()`
- **Prikaz podatkov na zaslon:** `showzifustr_scaled()`

---
## Implementacija
### **Komunikacija (I2C)**
Oximeter 5 Click uporablja **I2C** za komunikacijo:
- **Pisanje v registre:** `oximeter5_generic_write()` -> uporablja `HAL_I2C_Mem_Write()`
- **Branje iz registrov:** `oximeter5_generic_read()` -> uporablja `HAL_I2C_Mem_Read()`

### **Priprava podatkov**
- Podatki se shranjujejo v **FIFO buffer** (IR in RED)
- Uporabljene so metode za **odkrivanje vrhov signala**:
  - `dev_find_peaks()` (poišče pomembne vrhove signala)
  - `dev_peaks_above_min_height()` (odstrani premajhne vrhove)
  - `dev_remove_close_peaks()` (odstrani preblizu postavljene vrhove)

### **Obdelava in prikaz podatkov**
- Srčni utrip je določen z **mediano** vrednosti v tabeli meritev
- SpO2 je določen z **največjo vrednostjo** v tabeli meritev
- Podatki se prikažejo na LCD zaslonu z `showzifustr_scaled()`

---
## LCD Izris

### Specifikacije zaslona:
- **Velikost:** 1.28''
- **Ločljivost:** 240x240
- **Vmesnik:** SPI2

### Povezava na STM32H750B-DK:
| Pin | Priklop na STM32 |
|------|----------------|
| VCC | 3.3V |
| GND | GND |
| SCL | PD3 (SCK) |
| SDA | PB15 (MOSI) |
| DC  | PE6 |
| CS  | PI8 |
| RST | PE3 |

### Zapis na zaslon:
- `HAL_SPI_Transmit_DMA()` omogoča hitrejše pošiljanje podatkov
- **DMA** se uporablja za hitro risanje slik na zaslon
- `showzifustr_scaled()` zapiše podatke na zaslon

---
## Koda
Celotna implementacija je na voljo v **main.c**.
Glavne datoteke:
- **oximeter5.c** & **oximeter5.h** (komunikacija s senzorjem)
- **gc9a01.c** (upravljanje LCD zaslona)

---
## **Video**
Kratek posnetek o delovanju [ure](https://youtu.be/qiSMnL6MixI).
