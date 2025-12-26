#define I3G4250D_WHO_AM_I 0x0F
#define I3G4250D_CTRL_REG1 0x20
#define I3G4250D_CTRL_REG4 0x23
#define I3G4250D_OUT_X_L 0x28
#define SPI_READ 0x80
#define SPI_AUTO_INC 0x40

float gx_offset = 0, gy_offset = 0, gz_offset = 0;
float gx, gy, gz;
float gx_raw=0, gy_raw=0, gz_raw=0; // store raw readings

float gx_corrected ;
float gy_corrected ;
float gz_corrected ;


static void Gyro_CS_Low() {
    
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);
}

static void Gyro_CS_High() {
    
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET);
}

void I3G4250D_Write(uint8_t reg, uint8_t value){
    
    Gyro_CS_Low();
    uint8_t data[2] = { reg, value };
    HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    Gyro_CS_High();
}

void I3G4250D_Read(uint8_t reg, uint8_t *buffer, uint8_t len){
    
    Gyro_CS_Low();
    uint8_t address = reg | SPI_READ | SPI_AUTO_INC;
    HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, buffer, len, HAL_MAX_DELAY);
    Gyro_CS_High();
}

void I3G4250D_Init(void){

    // CTRL_REG1: enable X,Y,Z, set ODR=200 Hz, BW=25Hz
    I3G4250D_Write(I3G4250D_CTRL_REG1, 0x8F);
    // CTRL_REG4: ±245 dps full scale
    I3G4250D_Write(I3G4250D_CTRL_REG4, 0x00);
}

void I3G4250D_Read_Gyro(float *gx, float *gy, float *gz){

    uint8_t buffer[6];
    I3G4250D_Read(I3G4250D_OUT_X_L, buffer, 6);

    int16_t rawX = (buffer[1] << 8) | buffer[0];
    int16_t rawY = (buffer[3] << 8) | buffer[2];
    int16_t rawZ = (buffer[5] << 8) | buffer[4];

    *gx = rawX * 0.00875f;
    *gy = rawY * 0.00875f;
    *gz = rawZ * 0.00875f;
}


void I3G4250D_Calibrate(void)
{
    int samples = 100;
    float sum_x=0, sum_y=0, sum_z=0;
    for(int i=0; i<samples; i++)
    {
        float gx, gy, gz;
        I3G4250D_Read_Gyro(&gx, &gy, &gz);
        sum_x += gx;
        sum_y += gy;
        sum_z += gz;
    }
    gx_offset = sum_x / samples;
    gy_offset = sum_y / samples;
    gz_offset = sum_z / samples;
}
