
void Angleestimate()
{
// Convert raw to g
ax_g = ax_raw * 0.0039f;
ay_g = ay_raw * 0.0039f;
az_g = az_raw * 0.0039f;

// Convert to m/s^2
ax_ms2 = ax_g * 9.80665f;
ay_ms2 = ay_g * 9.80665f;
az_ms2 = az_g * 9.80665f;

// Gyro offsets removed
gx_corrected = gx_raw - gx_offset;
gy_corrected = gy_raw - gy_offset;
gz_corrected = gz_raw - gz_offset;

// --- Your chosen axes ---
float roll_acc = atan2f(-ax_ms2, az_ms2) * 57.2958f;

float roll_gyro = roll + gy_corrected * DT;

// Complementary filter
roll = 0.98f * roll_gyro + 0.02f * roll_acc;
}
