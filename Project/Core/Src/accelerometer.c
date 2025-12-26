#define LSM303AGR_ACC_I2C_ADDR_WRITE 0x32
#define LSM303AGR_ACC_I2C_ADDR_READ 0x33

#define CTRL_REG1_A 0x20
#define CTRL_REG4_A 0x23

#define OUT_X_L_A 0x28

int16_t ax_raw, ay_raw, az_raw;

float ax_g ;
float ay_g ;
float az_g ;

// --- Convert g → m/s² ---
float ax_ms2 ;
float ay_ms2 ;
float az_ms2 ;


void LSM303AGR_Init(void)
{
uint8_t ctrl1 = 0x67; // ODR=200Hz, all axes enabled
uint8_t ctrl4 = 0x00; // ±2g, high-resolution disabled

HAL_I2C_Mem_Write(&hi2c1, LSM303AGR_ACC_I2C_ADDR_WRITE,
CTRL_REG1_A, 1, &ctrl1, 1, HAL_MAX_DELAY);

HAL_I2C_Mem_Write(&hi2c1, LSM303AGR_ACC_I2C_ADDR_WRITE,
CTRL_REG4_A, 1, &ctrl4, 1, HAL_MAX_DELAY);
}


void LSM303AGR_Read_Accelerometer(int16_t *ax, int16_t *ay, int16_t *az)
{
uint8_t buffer[6];

HAL_I2C_Mem_Read(&hi2c1, LSM303AGR_ACC_I2C_ADDR_READ,
OUT_X_L_A | 0x80, 1, buffer, 6, HAL_MAX_DELAY);

int16_t rawX = (int16_t)((buffer[1] << 8) | buffer[0]);
int16_t rawY = (int16_t)((buffer[3] << 8) | buffer[2]);
int16_t rawZ = (int16_t)((buffer[5] << 8) | buffer[4]);

// Convert 16-bit to 10-bit (datasheet requirement)
*ax = rawX >> 6;
*ay = rawY >> 6;
*az = rawZ >> 6;
}


